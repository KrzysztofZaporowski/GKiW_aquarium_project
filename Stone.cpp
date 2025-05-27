#include "Stone.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>  // Add this line for glm::value_ptr

Stone::Stone(const std::string& objPath, const std::string& texturePath,
    glm::vec3 pos, glm::vec3 rot, glm::vec3 scl)
    : position(pos), rotation(rot), scale(scl) {

    ModelLoader loader;
    modelData = loader.loadModel(objPath, texturePath);

    if (!modelData) {
        std::cerr << "Failed to load stone model: " << objPath << std::endl;
    }
}

void Stone::draw(ShaderProgram* shader, const glm::mat4& projection, const glm::mat4& view, float time) {
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
        glUniform1i(shader->u("stoneTexture"), 0);
    }

    // Draw the stone
    glBindVertexArray(modelData->VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(modelData->vertexCount));
    glBindVertexArray(0);
}

std::vector<std::unique_ptr<Stone>> Stone::createRandomStones() {
    std::vector<std::unique_ptr<Stone>> stones;

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(-6.5f, 6.5f);  // Inside aquarium X bounds
    std::uniform_real_distribution<float> zDist(-4.5f, 4.5f);  // Inside aquarium Z bounds
    std::uniform_real_distribution<float> rotDist(0.0f, 360.0f);
    std::uniform_real_distribution<float> scaleDist(0.3f, 0.8f);

    // Stone types
    std::vector<std::string> stoneTypes = { "Rock01", "Rock02" };

    // Create 4 stones: 2 of each type
    for (int type = 0; type < 2; ++type) {
        for (int i = 0; i < 3; ++i) {
            // Generate random position on aquarium floor
            glm::vec3 position;
            bool validPosition = false;
            int attempts = 0;

            // Try to find a position that doesn't overlap with existing stones
            while (!validPosition && attempts < 20) {
                position = glm::vec3(
                    xDist(gen),     // Random X
                    -0.95f,         // On the sand floor
                    zDist(gen)      // Random Z
                );

                // Check distance from existing stones
                validPosition = true;
                for (const auto& existingStone : stones) {
                    float distance = glm::length(position - existingStone->getPosition());
                    if (distance < 2.0f) { // Minimum 2 units apart
                        validPosition = false;
                        break;
                    }
                }
                attempts++;
            }

            // Random rotation and scale
            glm::vec3 rotation(
                rotDist(gen),
                rotDist(gen),
                rotDist(gen)
            );

            float uniformScale = scaleDist(gen);
            glm::vec3 scale(uniformScale, uniformScale, uniformScale);

            // Create stone paths
            std::string objPath = stoneTypes[type] + ".obj";
            std::string texPath = stoneTypes[type] + ".png";

            // Create and add stone
            auto stone = std::make_unique<Stone>(objPath, texPath, position, rotation, scale);
            stones.push_back(std::move(stone));

            std::cout << "Created " << stoneTypes[type] << " at position ("
                << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
        }
    }

    return stones;
}