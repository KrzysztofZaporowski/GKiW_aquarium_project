#ifndef CORAL_H
#define CORAL_H

#include "ModelLoader.h"
#include "shaderprogram.h"
#include "Stone.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <random>

class Coral {
private:
    std::unique_ptr<ModelData> modelData;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    float swaySpeed;      // Individual sway speed for each coral
    float swayAmplitude;  // Individual sway amplitude

public:
    Coral(const std::string& objPath, const std::string& texturePath,
        glm::vec3 pos, glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scl = glm::vec3(1.0f));

    void draw(ShaderProgram* shader, const glm::mat4& projection, const glm::mat4& view, float time);

    // Getters
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getScale() const { return scale; }
    float getSwaySpeed() const { return swaySpeed; }
    float getSwayAmplitude() const { return swayAmplitude; }

    // Static method to create random corals avoiding stones
    static std::vector<std::unique_ptr<Coral>> createRandomCorals(
        const std::vector<std::unique_ptr<Stone>>& existingStones);
};

#endif // CORAL_H