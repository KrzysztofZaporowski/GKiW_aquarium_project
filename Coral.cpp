#include "Coral.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <algorithm>

Coral::Coral(const std::string& objPath, const std::string& texturePath,
    glm::vec3 pos, glm::vec3 rot, glm::vec3 scl)
    : position(pos), rotation(rot), scale(scl) {

    ModelLoader loader;
    modelData = loader.loadModel(objPath, texturePath);

    if (!modelData) {
        std::cerr << "Failed to load coral model: " << objPath << std::endl;
    }

    // Initialize random sway properties for each coral
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> speedDist(0.6f, 1.2f);
    std::uniform_real_distribution<float> ampDist(0.8f, 1.5f);

    swaySpeed = speedDist(gen);
    swayAmplitude = ampDist(gen);
}

void Coral::draw(ShaderProgram* shader, const glm::mat4& projection, const glm::mat4& view, float time) {
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
    glUniform1f(shader->u("time"), time * swaySpeed);  // Individual sway timing
    glUniform1f(shader->u("swayAmplitude"), swayAmplitude);  // Individual sway strength
    glUniform3fv(shader->u("cameraPos"), 1, glm::value_ptr(glm::vec3(0.0f))); // Will be set properly in main

    // Bind texture if available
    if (modelData->texture != 0) {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, modelData->texture);
        glUniform1i(shader->u("coralTexture"), 0);
    }

    // Draw the coral
    glBindVertexArray(modelData->VAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(modelData->vertexCount));
    glBindVertexArray(0);
}

std::vector<std::unique_ptr<Coral>> Coral::createRandomCorals(
    const std::vector<std::unique_ptr<Stone>>& existingStones) {

    std::vector<std::unique_ptr<Coral>> corals;

    // Random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> xDist(-6.0f, 6.0f);  // Inside aquarium X bounds (with margin)
    std::uniform_real_distribution<float> zDist(-4.0f, 4.0f);  // Inside aquarium Z bounds (with margin)
    std::uniform_real_distribution<float> scaleDist(0.4f, 0.8f);  // Similar to stones
    std::uniform_real_distribution<float> yRotDist(0.0f, 360.0f); // Only Y rotation for upright corals

    // Create 3-5 corals
    std::uniform_int_distribution<int> countDist(3, 5);
    int coralCount = countDist(gen);

    for (int i = 0; i < coralCount; ++i) {
        glm::vec3 position;
        bool validPosition = false;
        int attempts = 0;

        // Try to find a position that doesn't overlap with stones or other corals
        while (!validPosition && attempts < 30) {
            position = glm::vec3(
                xDist(gen),     // Random X
                -0.95f,         // On the sand floor (same as stones)
                zDist(gen)      // Random Z
            );

            // Check distance from existing stones
            validPosition = true;
            for (const auto& stone : existingStones) {
                float distance = glm::length(position - stone->getPosition());
                if (distance < 2.5f) { // Slightly more space than stones need
                    validPosition = false;
                    break;
                }
            }

            // Check distance from existing corals
            if (validPosition) {
                for (const auto& coral : corals) {
                    float distance = glm::length(position - coral->getPosition());
                    if (distance < 2.0f) { // Minimum distance between corals
                        validPosition = false;
                        break;
                    }
                }
            }

            attempts++;
        }

        // If we found a valid position, create the coral
        if (validPosition) {
            // Corals always stand upright - only rotate around Y axis
            glm::vec3 rotation(
                0.0f,               // No X rotation (keep upright)
                yRotDist(gen),      // Random Y rotation
                0.0f                // No Z rotation (keep upright)
            );

            float uniformScale = scaleDist(gen);
            glm::vec3 scale(uniformScale, uniformScale, uniformScale);

            // Create coral
            auto coral = std::make_unique<Coral>("coral.obj", "coral.png", position, rotation, scale);
            corals.push_back(std::move(coral));

            std::cout << "Created coral at position ("
                << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
        }
        else {
            std::cout << "Could not find valid position for coral after " << attempts << " attempts" << std::endl;
        }
    }

    return corals;
}