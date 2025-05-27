#include "Fish.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

Fish::Fish(const std::string& objPath, const std::string& texturePath,
    glm::vec3 pos, glm::vec3 rot, glm::vec3 scl)
    : position(pos), rotation(rot), scale(scl) {

    ModelLoader loader;
    modelData = loader.loadModel(objPath, texturePath);

    if (!modelData) {
        std::cerr << "Failed to load fish model: " << objPath << std::endl;
    }

    // Initialize movement properties
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> speedDist(0.8f, 1.5f);
    std::uniform_real_distribution<float> radiusDist(1.0f, 3.0f);
    std::uniform_real_distribution<float> timeDist(2.0f, 5.0f);

    speed = speedDist(gen);
    rotationSpeed = 45.0f; // degrees per second
    swimRadius = radiusDist(gen);
    changeDirectionTimer = 0.0f;
    maxChangeDirectionTime = timeDist(gen);

    velocity = glm::vec3(0.0f);
    generateNewTarget();
}

void Fish::generateNewTarget() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(AQUARIUM_MIN_X, AQUARIUM_MAX_X);
    std::uniform_real_distribution<float> yDist(AQUARIUM_MIN_Y, AQUARIUM_MAX_Y);
    std::uniform_real_distribution<float> zDist(AQUARIUM_MIN_Z, AQUARIUM_MAX_Z);

    // Generate target within swim radius or anywhere in aquarium
    glm::vec3 newTarget;
    int attempts = 0;

    do {
        newTarget = glm::vec3(
            xDist(gen),
            yDist(gen),
            zDist(gen)
        );
        attempts++;
    } while (!isPositionValid(newTarget) && attempts < 10);

    targetPosition = newTarget;

    // Reset timer
    std::uniform_real_distribution<float> timeDist(2.0f, 5.0f);
    maxChangeDirectionTime = timeDist(gen);
    changeDirectionTimer = 0.0f;
}

bool Fish::isPositionValid(const glm::vec3& pos) const {
    return pos.x >= AQUARIUM_MIN_X && pos.x <= AQUARIUM_MAX_X &&
        pos.y >= AQUARIUM_MIN_Y && pos.y <= AQUARIUM_MAX_Y &&
        pos.z >= AQUARIUM_MIN_Z && pos.z <= AQUARIUM_MAX_Z;
}

void Fish::updateMovement(float deltaTime) {
    changeDirectionTimer += deltaTime;

    // Change direction periodically or if reached target
    float distanceToTarget = glm::length(targetPosition - position);
    if (changeDirectionTimer >= maxChangeDirectionTime || distanceToTarget < 0.5f) {
        generateNewTarget();
    }

    // Calculate direction to target
    glm::vec3 direction = glm::normalize(targetPosition - position);

    // Smooth velocity change
    glm::vec3 targetVelocity = direction * speed;
    velocity = glm::mix(velocity, targetVelocity, deltaTime * 2.0f);

    // Update position
    glm::vec3 newPosition = position + velocity * deltaTime;

    // Clamp position to aquarium bounds
    newPosition.x = std::max(AQUARIUM_MIN_X, std::min(newPosition.x, AQUARIUM_MAX_X));
    newPosition.y = std::max(AQUARIUM_MIN_Y, std::min(newPosition.y, AQUARIUM_MAX_Y));
    newPosition.z = std::max(AQUARIUM_MIN_Z, std::min(newPosition.z, AQUARIUM_MAX_Z));

    position = newPosition;

    // Update rotation to face movement direction
    if (glm::length(velocity) > 0.1f) {
        float targetYaw = atan2(velocity.x, velocity.z) * 180.0f / (float)M_PI;
        float targetPitch = -asin(velocity.y / glm::length(velocity)) * 180.0f / (float)M_PI;

        // Smooth rotation
        rotation.y = rotation.y + (targetYaw - rotation.y) * deltaTime * (rotationSpeed / 45.0f);
        rotation.x = rotation.x + (targetPitch - rotation.x) * deltaTime * (rotationSpeed / 45.0f);
    }
}

void Fish::update(float deltaTime) {
    updateMovement(deltaTime);
}

void Fish::draw(ShaderProgram* shader, const glm::mat4& projection, const glm::mat4& view, float time) {
    if (!modelData || !shader) return;

    shader->use();

    // Create model matrix
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, scale);

    // Send matrices to shader
    glUniformMatrix4fv(shader->u("P"), 1, false, glm::value_ptr(projection));
    glUniformMatrix4fv(shader->u("V"), 1, false, glm::value_ptr(view));
    glUniformMatrix4fv(shader->u("M"), 1, false, glm::value_ptr(model));
    glUniform1f(shader->u("time"), time);
    glUniform3fv(shader->u("cameraPos"), 1, glm::value_ptr(glm::vec3(0.0f))); // Will be set properly in main

    // Bind texture if available
    if (modelData->texture != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, modelData->texture);
        glUniform1i(shader->u("fishTexture"), 0);
    }

    // Draw the fish
    glBindVertexArray(modelData->VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(modelData->vertexCount));
    glBindVertexArray(0);
}

std::vector<std::unique_ptr<Fish>> Fish::createRandomFish() {
    std::vector<std::unique_ptr<Fish>> fish;

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(AQUARIUM_MIN_X, AQUARIUM_MAX_X);
    std::uniform_real_distribution<float> yDist(AQUARIUM_MIN_Y, AQUARIUM_MAX_Y);
    std::uniform_real_distribution<float> zDist(AQUARIUM_MIN_Z, AQUARIUM_MAX_Z);
    std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);
    std::uniform_real_distribution<float> scaleDist(0.15f, 0.25f);

    // Fish types with specific corrections for problematic models
    struct FishConfig {
        std::string name;
        glm::vec3 rotationOffset;  // Additional rotation to fix orientation
        glm::vec3 scaleMultiplier; // Scale adjustments if needed
    };

    std::vector<FishConfig> fishConfigs = {
        {"TropicalFish01", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
        {"TropicalFish02", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
        {"TropicalFish03", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)},
        {"TropicalFish12", glm::vec3(0.0f, 0.0f, 180.0f), glm::vec3(1.0f, 1.0f, 1.0f)}, // Flip upside down
        {"TropicalFish15", glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)}
    };

    // Create 8-12 fish (1-3 of each type)
    std::uniform_int_distribution<int> fishPerTypeDist(1, 3);

    for (const auto& fishConfig : fishConfigs) {
        int fishCount = fishPerTypeDist(gen);

        for (int i = 0; i < fishCount; ++i) {
            // Generate random position in aquarium
            glm::vec3 position;
            bool validPosition = false;
            int attempts = 0;

            // Try to find a position that doesn't overlap with existing fish
            while (!validPosition && attempts < 20) {
                position = glm::vec3(
                    xDist(gen),
                    yDist(gen),
                    zDist(gen)
                );

                // Check distance from existing fish
                validPosition = true;
                for (const auto& existingFish : fish) {
                    float distance = glm::length(position - existingFish->getPosition());
                    if (distance < 1.5f) { // Minimum 1.5 units apart
                        validPosition = false;
                        break;
                    }
                }
                attempts++;
            }

            // Random rotation with model-specific corrections
            glm::vec3 rotation(
                rotDist(gen) + fishConfig.rotationOffset.x,
                rotDist(gen) + fishConfig.rotationOffset.y,
                fishConfig.rotationOffset.z  // Apply fixed roll correction
            );

            float uniformScale = scaleDist(gen);
            glm::vec3 scale = glm::vec3(uniformScale, uniformScale, uniformScale) * fishConfig.scaleMultiplier;

            // Create fish paths
            std::string objPath = fishConfig.name + ".obj";
            std::string texPath = fishConfig.name + ".png";

            // Create and add fish
            auto newFish = std::make_unique<Fish>(objPath, texPath, position, rotation, scale);
            fish.push_back(std::move(newFish));

            std::cout << "Created " << fishConfig.name << " at position ("
                << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
        }
    }

    return fish;
}