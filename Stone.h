#ifndef STONE_H
#define STONE_H

#include "ModelLoader.h"
#include "shaderprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <random>

class Stone {
private:
    std::unique_ptr<ModelData> modelData;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

public:
    Stone(const std::string& objPath, const std::string& texturePath,
        glm::vec3 pos, glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scl = glm::vec3(1.0f));

    void draw(ShaderProgram* shader, const glm::mat4& projection, const glm::mat4& view, float time);

    // Getters
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getScale() const { return scale; }

    // Static method to create random stones in aquarium
    static std::vector<std::unique_ptr<Stone>> createRandomStones();
};

#endif // STONE_H