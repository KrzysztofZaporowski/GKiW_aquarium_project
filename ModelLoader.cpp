#include "ModelLoader.h"
#include "lodepng.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

// ModelData implementation
ModelData::ModelData() : VAO(0), texture(0), vertexCount(0) {
    VBO[0] = VBO[1] = VBO[2] = 0;
}

ModelData::~ModelData() {
    if (VAO != 0) {
        glDeleteVertexArrays(1, &VAO);
    }
    if (VBO[0] != 0 || VBO[1] != 0 || VBO[2] != 0) {
        glDeleteBuffers(3, VBO);
    }
    if (texture != 0) {
        glDeleteTextures(1, &texture);
    }
}

ModelData::ModelData(ModelData&& other) noexcept
    : vertices(std::move(other.vertices))
    , normals(std::move(other.normals))
    , texCoords(std::move(other.texCoords))
    , VAO(other.VAO)
    , texture(other.texture)
    , vertexCount(other.vertexCount) {

    VBO[0] = other.VBO[0];
    VBO[1] = other.VBO[1];
    VBO[2] = other.VBO[2];

    // Reset other object
    other.VAO = 0;
    other.texture = 0;
    other.VBO[0] = other.VBO[1] = other.VBO[2] = 0;
    other.vertexCount = 0;
}

ModelData& ModelData::operator=(ModelData&& other) noexcept {
    if (this != &other) {
        // Clean up current resources
        if (VAO != 0) glDeleteVertexArrays(1, &VAO);
        if (VBO[0] != 0 || VBO[1] != 0 || VBO[2] != 0) glDeleteBuffers(3, VBO);
        if (texture != 0) glDeleteTextures(1, &texture);

        // Move data
        vertices = std::move(other.vertices);
        normals = std::move(other.normals);
        texCoords = std::move(other.texCoords);
        VAO = other.VAO;
        texture = other.texture;
        vertexCount = other.vertexCount;
        VBO[0] = other.VBO[0];
        VBO[1] = other.VBO[1];
        VBO[2] = other.VBO[2];

        // Reset other object
        other.VAO = 0;
        other.texture = 0;
        other.VBO[0] = other.VBO[1] = other.VBO[2] = 0;
        other.vertexCount = 0;
    }
    return *this;
}

// ModelLoader implementation
std::unique_ptr<ModelData> ModelLoader::loadModel(const std::string& objPath, const std::string& texturePath) {
    auto modelData = std::make_unique<ModelData>();

    if (!parseOBJ(objPath, modelData->vertices, modelData->normals, modelData->texCoords)) {
        std::cerr << "Failed to load OBJ file: " << objPath << std::endl;
        return nullptr;
    }

    modelData->texture = loadTexture(texturePath);
    if (modelData->texture == 0) {
        std::cerr << "Failed to load texture: " << texturePath << std::endl;
    }

    setupBuffers(*modelData);
    return modelData;
}

std::unique_ptr<ModelData> ModelLoader::loadModel(const std::string& objPath) {
    auto modelData = std::make_unique<ModelData>();

    if (!parseOBJ(objPath, modelData->vertices, modelData->normals, modelData->texCoords)) {
        std::cerr << "Failed to load OBJ file: " << objPath << std::endl;
        return nullptr;
    }

    setupBuffers(*modelData);
    return modelData;
}

bool ModelLoader::parseOBJ(const std::string& filePath,
    std::vector<glm::vec3>& vertices,
    std::vector<glm::vec3>& normals,
    std::vector<glm::vec2>& texCoords) {

    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Cannot open OBJ file: " << filePath << std::endl;
        return false;
    }

    std::vector<glm::vec3> tempVertices;
    std::vector<glm::vec3> tempNormals;
    std::vector<glm::vec2> tempTexCoords;

    std::vector<unsigned int> vertexIndices, normalIndices, texCoordIndices;

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "v") {
            // Vertex
            glm::vec3 vertex;
            iss >> vertex.x >> vertex.y >> vertex.z;
            tempVertices.push_back(vertex);
        }
        else if (prefix == "vn") {
            // Normal
            glm::vec3 normal;
            iss >> normal.x >> normal.y >> normal.z;
            tempNormals.push_back(normal);
        }
        else if (prefix == "vt") {
            // Texture coordinate
            glm::vec2 texCoord;
            iss >> texCoord.x >> texCoord.y;
            tempTexCoords.push_back(texCoord);
        }
        else if (prefix == "f") {
            // Face
            std::string vertex1, vertex2, vertex3;
            iss >> vertex1 >> vertex2 >> vertex3;

            int v1, v2, v3, t1, t2, t3, n1, n2, n3;

            parseFaceIndices(vertex1, v1, t1, n1);
            parseFaceIndices(vertex2, v2, t2, n2);
            parseFaceIndices(vertex3, v3, t3, n3);

            vertexIndices.push_back(v1 - 1);
            vertexIndices.push_back(v2 - 1);
            vertexIndices.push_back(v3 - 1);

            if (t1 > 0 && t2 > 0 && t3 > 0) {
                texCoordIndices.push_back(t1 - 1);
                texCoordIndices.push_back(t2 - 1);
                texCoordIndices.push_back(t3 - 1);
            }

            if (n1 > 0 && n2 > 0 && n3 > 0) {
                normalIndices.push_back(n1 - 1);
                normalIndices.push_back(n2 - 1);
                normalIndices.push_back(n3 - 1);
            }
        }
    }

    file.close();

    // Build final vertex arrays
    for (size_t i = 0; i < vertexIndices.size(); ++i) {
        vertices.push_back(tempVertices[vertexIndices[i]]);
    }

    for (size_t i = 0; i < normalIndices.size(); ++i) {
        normals.push_back(tempNormals[normalIndices[i]]);
    }

    for (size_t i = 0; i < texCoordIndices.size(); ++i) {
        texCoords.push_back(tempTexCoords[texCoordIndices[i]]);
    }

    // Normalize vertices to unit scale
    normalizeVertices(vertices);

    return true;
}

GLuint ModelLoader::loadTexture(const std::string& texturePath) {
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Using lodepng (same as your existing texture loading)
    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, texturePath);

    if (!error && !image.empty()) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cerr << "LodePNG failed to load texture: " << texturePath;
        if (error) {
            std::cerr << " Error: " << lodepng_error_text(error);
        }
        std::cerr << std::endl;
        glDeleteTextures(1, &textureID);
        textureID = 0;
    }

    return textureID;
}

void ModelLoader::setupBuffers(ModelData& modelData) {
    modelData.vertexCount = modelData.vertices.size();

    glGenVertexArrays(1, &modelData.VAO);
    glGenBuffers(3, modelData.VBO);

    glBindVertexArray(modelData.VAO);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, modelData.VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, modelData.vertices.size() * sizeof(glm::vec3),
        modelData.vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
    glEnableVertexAttribArray(0);

    // Normals
    if (!modelData.normals.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, modelData.VBO[1]);
        glBufferData(GL_ARRAY_BUFFER, modelData.normals.size() * sizeof(glm::vec3),
            modelData.normals.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
        glEnableVertexAttribArray(1);
    }

    // Texture coordinates
    if (!modelData.texCoords.empty()) {
        glBindBuffer(GL_ARRAY_BUFFER, modelData.VBO[2]);
        glBufferData(GL_ARRAY_BUFFER, modelData.texCoords.size() * sizeof(glm::vec2),
            modelData.texCoords.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), nullptr);
        glEnableVertexAttribArray(2);
    }

    glBindVertexArray(0);
}

void ModelLoader::normalizeVertices(std::vector<glm::vec3>& vertices) {
    if (vertices.empty()) return;

    float maxComponent = 0.0f;
    for (const auto& vertex : vertices) {
        maxComponent = std::max(maxComponent, std::abs(vertex.x));
        maxComponent = std::max(maxComponent, std::abs(vertex.y));
        maxComponent = std::max(maxComponent, std::abs(vertex.z));
    }

    if (maxComponent > 0.0f) {
        for (auto& vertex : vertices) {
            vertex /= maxComponent;
        }
    }
}

std::vector<std::string> ModelLoader::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream iss(str);
    std::string token;

    while (std::getline(iss, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

void ModelLoader::parseFaceIndices(const std::string& faceStr, int& vertexIdx, int& texIdx, int& normalIdx) {
    auto tokens = split(faceStr, '/');

    vertexIdx = !tokens[0].empty() ? std::stoi(tokens[0]) : 0;
    texIdx = (tokens.size() > 1 && !tokens[1].empty()) ? std::stoi(tokens[1]) : 0;
    normalIdx = (tokens.size() > 2 && !tokens[2].empty()) ? std::stoi(tokens[2]) : 0;
}