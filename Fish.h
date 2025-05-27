#ifndef FISH_H
#define FISH_H

#include "ModelLoader.h"
#include "shaderprogram.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <random>
#include <vector>

class Fish {
private:
    std::unique_ptr<ModelData> modelData;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    // Movement properties
    glm::vec3 velocity;
    glm::vec3 targetPosition;
    float speed;
    float rotationSpeed;
    float swimRadius;
    float changeDirectionTimer;
    float maxChangeDirectionTime;

    // Aquarium bounds
    static constexpr float AQUARIUM_MIN_X = -7.5f;
    static constexpr float AQUARIUM_MAX_X = 7.5f;
    static constexpr float AQUARIUM_MIN_Y = -0.5f;
    static constexpr float AQUARIUM_MAX_Y = 3.5f;
    static constexpr float AQUARIUM_MIN_Z = -5.5f;
    static constexpr float AQUARIUM_MAX_Z = 5.5f;

    // Helper methods
    void generateNewTarget();
    void updateMovement(float deltaTime);
    bool isPositionValid(const glm::vec3& pos) const;

public:
    Fish(const std::string& objPath, const std::string& texturePath,
        glm::vec3 pos, glm::vec3 rot = glm::vec3(0.0f), glm::vec3 scl = glm::vec3(1.0f));

    void update(float deltaTime);
    void draw(ShaderProgram* shader, const glm::mat4& projection, const glm::mat4& view, float time);

    // Getters
    glm::vec3 getPosition() const { return position; }
    glm::vec3 getScale() const { return scale; }

    // Static method to create random fish in aquarium
    static std::vector<std::unique_ptr<Fish>> createRandomFish();
};

#endif // FISH_H