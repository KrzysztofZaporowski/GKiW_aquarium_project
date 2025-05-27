#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <string>
#include <memory>

struct ModelData {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texCoords;
    GLuint VAO;
    GLuint VBO[3]; // vertices, normals, texCoords
    GLuint texture;
    size_t vertexCount;

    ModelData();
    ~ModelData();

    // Delete copy constructor and assignment operator to prevent issues with OpenGL resources
    ModelData(const ModelData&) = delete;
    ModelData& operator=(const ModelData&) = delete;

    // Move constructor and assignment operator
    ModelData(ModelData&& other) noexcept;
    ModelData& operator=(ModelData&& other) noexcept;
};

class ModelLoader {
public:
    ModelLoader() = default;
    ~ModelLoader() = default;

    // Load OBJ model with texture
    std::unique_ptr<ModelData> loadModel(const std::string& objPath, const std::string& texturePath);

    // Load OBJ model without texture
    std::unique_ptr<ModelData> loadModel(const std::string& objPath);

private:
    // Parse OBJ file
    bool parseOBJ(const std::string& filePath,
        std::vector<glm::vec3>& vertices,
        std::vector<glm::vec3>& normals,
        std::vector<glm::vec2>& texCoords);

    // Load texture from file
    GLuint loadTexture(const std::string& texturePath);

    // Setup OpenGL buffers
    void setupBuffers(ModelData& modelData);

    // Normalize model vertices to unit scale
    void normalizeVertices(std::vector<glm::vec3>& vertices);

    // Helper function to split string
    std::vector<std::string> split(const std::string& str, char delimiter);

    // Helper function to parse face indices
    void parseFaceIndices(const std::string& faceStr, int& vertexIdx, int& texIdx, int& normalIdx);
};

#endif // MODEL_LOADER_H