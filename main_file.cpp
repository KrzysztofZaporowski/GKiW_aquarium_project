#define GLM_FORCE_RADIANS
#define GLM_FORCE_SWIZZLE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "constants.h"
#include "lodepng.h"
#include "shaderprogram.h"
#include "vertices.h"
#include "Stone.h"
#include "Fish.h"
#include "Coral.h"

// Camera variables
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);  // Start underwater level
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
float cameraSpeed = 2.5f;

// Mouse control variables
float yaw = -90.0f;    // Obrót w poziomie
float pitch = 0.0f;    // Obrót w pionie  
float fov = 45.0f;     // Field of view dla zoom
float lastX = 400.0f;
float lastY = 300.0f;
bool firstMouse = true;
bool mousePressed = false;

float aspectRatio = 1;
bool keys[1024];

ShaderProgram* sp;
ShaderProgram* skyboxShader;
ShaderProgram* roomSkyboxShader;
ShaderProgram* glassShader;
ShaderProgram* frameShader;
ShaderProgram* outsideShader;
ShaderProgram* waterFogShader;
ShaderProgram* stoneShader;
ShaderProgram* fishShader;
ShaderProgram* coralShader;

GLuint sandVAO, sandVBO;
GLuint skyboxVAO, skyboxVBO;
GLuint glassVAO, glassVBO;
GLuint frameVAO, frameVBO;
GLuint outsideFloorVAO, outsideFloorVBO;
GLuint waterFogVAO, waterFogVBO;
GLuint sandDiffuseTexture, sandDisplacementTexture;
GLuint floorDiffuseTexture, floorDisplacementTexture;

// Objects
std::vector<std::unique_ptr<Stone>> aquarium_stones;
std::vector<std::unique_ptr<Fish>> aquarium_fish;
std::vector<std::unique_ptr<Coral>> aquarium_corals;

// Texture loading function
GLuint loadTexture(const char* path) {
    GLuint textureID;
    glGenTextures(1, &textureID);

    std::vector<unsigned char> image;
    unsigned width, height;
    unsigned error = lodepng::decode(image, width, height, path);

    if (error) {
        std::cout << "Error loading texture " << path << ": " << lodepng_error_text(error) << std::endl;
        return 0;
    }

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, &image[0]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    return textureID;
}

// Error callback
void error_callback(int error, const char* description) {
    fputs(description, stderr);
}

// Mouse position callback
void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    if (mousePressed) {
        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos; // odwrócone bo y-coordinates id¹ od do³u do góry

        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        // Ograniczenia pitch
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // Aktualizacja wektora front kamery
        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }
    else {
        lastX = xpos;
        lastY = ypos;
    }
}

// Mouse button callback
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS)
            mousePressed = true;
        else if (action == GLFW_RELEASE)
            mousePressed = false;
    }
}

// Scroll callback for zoom
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    fov -= (float)yoffset;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

// Key callback with WSAD movement
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key >= 0 && key < 1024) {
        if (action == GLFW_PRESS)
            keys[key] = true;
        else if (action == GLFW_RELEASE)
            keys[key] = false;
    }
}

// Process continuous key input
void processInput(float deltaTime) {
    float velocity = cameraSpeed * deltaTime;

    if (keys[GLFW_KEY_W])
        cameraPos += cameraFront * velocity;
    if (keys[GLFW_KEY_S])
        cameraPos -= cameraFront * velocity;
    if (keys[GLFW_KEY_A])
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;
    if (keys[GLFW_KEY_D])
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * velocity;

    // Keep camera above floors but allow movement outside aquarium
    if (cameraPos.y < -0.8f)
        cameraPos.y = -0.8f;
    if (cameraPos.y > 8.0f)  // Higher ceiling
        cameraPos.y = 8.0f;
}

void windowResizeCallback(GLFWwindow* window, int width, int height) {
    if (height == 0) return;
    aspectRatio = (float)width / (float)height;
    glViewport(0, 0, width, height);
}

// Setup glass walls geometry
void setupGlassWalls() {
    glGenVertexArrays(1, &glassVAO);
    glGenBuffers(1, &glassVBO);

    glBindVertexArray(glassVAO);

    glBindBuffer(GL_ARRAY_BUFFER, glassVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glassWallVertices), glassWallVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // Normal attribute
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

// Setup frame geometry
void setupFrame() {
    glGenVertexArrays(1, &frameVAO);
    glGenBuffers(1, &frameVBO);

    glBindVertexArray(frameVAO);

    glBindBuffer(GL_ARRAY_BUFFER, frameVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(frameVertices), frameVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

// Setup underwater skybox geometry
void setupSkybox() {
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);

    glBindVertexArray(skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

// Setup sand floor geometry
void setupSandFloor() {
    glGenVertexArrays(1, &sandVAO);
    glGenBuffers(1, &sandVBO);

    glBindVertexArray(sandVAO);

    glBindBuffer(GL_ARRAY_BUFFER, sandVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(sandVertices), sandVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

// Setup outside floor geometry
void setupOutsideFloor() {
    glGenVertexArrays(1, &outsideFloorVAO);
    glGenBuffers(1, &outsideFloorVBO);

    glBindVertexArray(outsideFloorVAO);

    glBindBuffer(GL_ARRAY_BUFFER, outsideFloorVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(outsideFloorVertices), outsideFloorVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Texture coordinate attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

// Setup water fog geometry
void setupWaterFog() {
    glGenVertexArrays(1, &waterFogVAO);
    glGenBuffers(1, &waterFogVBO);

    glBindVertexArray(waterFogVAO);

    glBindBuffer(GL_ARRAY_BUFFER, waterFogVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(waterFogVertices), waterFogVertices, GL_STATIC_DRAW);

    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

// Initialization procedure
void initOpenGLProgram(GLFWwindow* window) {
    glClearColor(0.08f, 0.18f, 0.25f, 1.0f); // blue room lighting
    glEnable(GL_DEPTH_TEST);

    // Enable blending for transparent glass
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glfwSetWindowSizeCallback(window, windowResizeCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Create shader programs
    sp = new ShaderProgram("v_sand.glsl", NULL, "f_sand.glsl");
    skyboxShader = new ShaderProgram("v_skybox.glsl", NULL, "f_skybox.glsl");
    roomSkyboxShader = new ShaderProgram("v_room_skybox.glsl", NULL, "f_room_skybox.glsl");
    glassShader = new ShaderProgram("v_glass.glsl", NULL, "f_glass.glsl");
    frameShader = new ShaderProgram("v_frame.glsl", NULL, "f_frame.glsl");
    outsideShader = new ShaderProgram("v_outside.glsl", NULL, "f_outside.glsl");
    waterFogShader = new ShaderProgram("v_water_fog.glsl", NULL, "f_water_fog.glsl");
    stoneShader = new ShaderProgram("v_stone.glsl", NULL, "f_stone.glsl");
    fishShader = new ShaderProgram("v_fish.glsl", NULL, "f_fish.glsl");
    coralShader = new ShaderProgram("v_coral.glsl", NULL, "f_coral.glsl");

    // Setup geometry
    setupSandFloor();
    setupSkybox();
    setupGlassWalls();
    setupFrame();
    setupOutsideFloor();
    setupWaterFog();

    // Create stones
    aquarium_stones = Stone::createRandomStones();
    std::cout << "Created " << aquarium_stones.size() << " stones in aquarium" << std::endl;

    // Create corals (after stones to avoid collisions)
    aquarium_corals = Coral::createRandomCorals(aquarium_stones);
    std::cout << "Created " << aquarium_corals.size() << " corals in aquarium" << std::endl;

    // Create fish
    aquarium_fish = Fish::createRandomFish();
    std::cout << "Created " << aquarium_fish.size() << " fish in aquarium" << std::endl;

    // Load textures
    sandDiffuseTexture = loadTexture("sand_diff.png");
    sandDisplacementTexture = loadTexture("sand_disp.png");
    floorDiffuseTexture = loadTexture("floor_diff.png");
    floorDisplacementTexture = loadTexture("floor_disp.png");

    if (sandDiffuseTexture == 0 || sandDisplacementTexture == 0 ||
        floorDiffuseTexture == 0 || floorDisplacementTexture == 0) {
        std::cout << "Failed to load textures!" << std::endl;
    }
}

// Free resources
void freeOpenGLProgram(GLFWwindow* window) {
    glDeleteVertexArrays(1, &sandVAO);
    glDeleteBuffers(1, &sandVBO);
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteVertexArrays(1, &glassVAO);
    glDeleteBuffers(1, &glassVBO);
    glDeleteVertexArrays(1, &frameVAO);
    glDeleteBuffers(1, &frameVBO);
    glDeleteVertexArrays(1, &outsideFloorVAO);
    glDeleteBuffers(1, &outsideFloorVBO);
    glDeleteVertexArrays(1, &waterFogVAO);
    glDeleteBuffers(1, &waterFogVBO);
    glDeleteTextures(1, &sandDiffuseTexture);
    glDeleteTextures(1, &sandDisplacementTexture);
    glDeleteTextures(1, &floorDiffuseTexture);
    glDeleteTextures(1, &floorDisplacementTexture);
    delete sp;
    delete skyboxShader;
    delete roomSkyboxShader;
    delete glassShader;
    delete frameShader;
    delete outsideShader;
    delete waterFogShader;
    delete stoneShader;
    delete fishShader;
    delete coralShader;
}

// Draw scene
void drawScene(GLFWwindow* window, float deltaTime) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Process input
    processInput(deltaTime);

    // View matrix (camera)
    glm::mat4 V = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // Projection matrix with variable FOV for zoom
    glm::mat4 P = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 100.0f);

    float time = glfwGetTime();
    glm::mat4 M = glm::mat4(1.0f);

    // Check if camera is inside aquarium for different skybox
    bool insideAquarium = (cameraPos.x > -8.0f && cameraPos.x < 8.0f &&
        cameraPos.z > -6.0f && cameraPos.z < 6.0f &&
        cameraPos.y > -1.0f && cameraPos.y < 4.0f);

    // === DRAW OUTSIDE FLOOR FIRST ===
    outsideShader->use();
    glUniformMatrix4fv(outsideShader->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(outsideShader->u("V"), 1, false, glm::value_ptr(V));
    glUniformMatrix4fv(outsideShader->u("M"), 1, false, glm::value_ptr(M));
    glUniform1f(outsideShader->u("time"), time);
    glUniform3fv(outsideShader->u("cameraPos"), 1, glm::value_ptr(cameraPos));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, floorDiffuseTexture);
    glUniform1i(outsideShader->u("floorDiffuse"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, floorDisplacementTexture);
    glUniform1i(outsideShader->u("floorDisplacement"), 1);

    glBindVertexArray(outsideFloorVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // === DRAW SKYBOX (different based on location) ===
    glDepthMask(GL_FALSE);
    glm::mat4 skyboxView = glm::mat4(glm::mat3(V));

    if (insideAquarium) {
        // Underwater skybox
        skyboxShader->use();
        glUniformMatrix4fv(skyboxShader->u("P"), 1, false, glm::value_ptr(P));
        glUniformMatrix4fv(skyboxShader->u("V"), 1, false, glm::value_ptr(skyboxView));
        glUniform1f(skyboxShader->u("time"), time);
        glUniform3fv(skyboxShader->u("cameraPos"), 1, glm::value_ptr(cameraPos));
    }
    else {
        // Bright room skybox
        roomSkyboxShader->use();
        glUniformMatrix4fv(roomSkyboxShader->u("P"), 1, false, glm::value_ptr(P));
        glUniformMatrix4fv(roomSkyboxShader->u("V"), 1, false, glm::value_ptr(skyboxView));
        glUniform1f(roomSkyboxShader->u("time"), time);
        glUniform3fv(roomSkyboxShader->u("cameraPos"), 1, glm::value_ptr(cameraPos));
    }

    glBindVertexArray(skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    glBindVertexArray(0);

    glDepthMask(GL_TRUE);

    // === DRAW SAND FLOOR (inside aquarium only) ===
    sp->use();
    glUniformMatrix4fv(sp->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(sp->u("V"), 1, false, glm::value_ptr(V));
    glUniformMatrix4fv(sp->u("M"), 1, false, glm::value_ptr(M));
    glUniform1f(sp->u("time"), time);
    glUniform3fv(sp->u("cameraPos"), 1, glm::value_ptr(cameraPos));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, sandDiffuseTexture);
    glUniform1i(sp->u("sandDiffuse"), 0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, sandDisplacementTexture);
    glUniform1i(sp->u("sandDisplacement"), 1);

    glBindVertexArray(sandVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

    // === DRAW STONES ===
    for (auto& stone : aquarium_stones) {
        stone->draw(stoneShader, P, V, time);
    }

    // === DRAW CORALS ===
    coralShader->use();
    glUniform3fv(coralShader->u("cameraPos"), 1, glm::value_ptr(cameraPos));

    for (auto& coral : aquarium_corals) {
        coral->draw(coralShader, P, V, time);
    }

    // === UPDATE AND DRAW FISH ===
    for (auto& fish : aquarium_fish) {
        fish->update(deltaTime);
        fish->draw(fishShader, P, V, time);
    }

    // === DRAW AQUARIUM FRAME ===
    frameShader->use();
    glUniformMatrix4fv(frameShader->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(frameShader->u("V"), 1, false, glm::value_ptr(V));
    glUniformMatrix4fv(frameShader->u("M"), 1, false, glm::value_ptr(M));
    glUniform1f(frameShader->u("time"), time);
    glUniform3fv(frameShader->u("cameraPos"), 1, glm::value_ptr(cameraPos));

    glBindVertexArray(frameVAO);
    glLineWidth(8.0f); // Thick frame lines
    glDrawArrays(GL_LINES, 0, 16); // 12 edges: 8 horizontal + 4 vertical
    glBindVertexArray(0);

    // === DRAW GLASS WALLS ===
    glassShader->use();
    glUniformMatrix4fv(glassShader->u("P"), 1, false, glm::value_ptr(P));
    glUniformMatrix4fv(glassShader->u("V"), 1, false, glm::value_ptr(V));
    glUniformMatrix4fv(glassShader->u("M"), 1, false, glm::value_ptr(M));
    glUniform1f(glassShader->u("time"), time);
    glUniform3fv(glassShader->u("cameraPos"), 1, glm::value_ptr(cameraPos));

    glBindVertexArray(glassVAO);
    glDrawArrays(GL_TRIANGLES, 0, 24);
    glBindVertexArray(0);

    // === DRAW WATER FOG (only when inside aquarium) ===
    if (insideAquarium) {
        // Disable depth writing for fog volume
        glDepthMask(GL_FALSE);

        waterFogShader->use();
        glUniformMatrix4fv(waterFogShader->u("P"), 1, false, glm::value_ptr(P));
        glUniformMatrix4fv(waterFogShader->u("V"), 1, false, glm::value_ptr(V));
        glUniformMatrix4fv(waterFogShader->u("M"), 1, false, glm::value_ptr(M));
        glUniform1f(waterFogShader->u("time"), time);
        glUniform3fv(waterFogShader->u("cameraPos"), 1, glm::value_ptr(cameraPos));

        glBindVertexArray(waterFogVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);

        // Re-enable depth writing
        glDepthMask(GL_TRUE);
    }

    glfwSwapBuffers(window);
}

int main(void) {
    GLFWwindow* window;

    glfwSetErrorCallback(error_callback);

    if (!glfwInit()) {
        fprintf(stderr, "Cannot initialize GLFW.\n");
        exit(EXIT_FAILURE);
    }

    window = glfwCreateWindow(800, 600, "Aquarium with Fish and Corals", NULL, NULL);

    if (!window) {
        fprintf(stderr, "Cannot create window.\n");
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Cannot initialize GLEW.\n");
        exit(EXIT_FAILURE);
    }

    initOpenGLProgram(window);

    // Main loop with delta time
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        drawScene(window, deltaTime);
        glfwPollEvents();
    }

    freeOpenGLProgram(window);

    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}