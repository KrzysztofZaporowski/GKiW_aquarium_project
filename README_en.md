# 🐠 Aquarium 3D – OpenGL Project

## 📖 Project Description

An interactive 3D aquarium featuring fish, corals, and rocks, rendered in real-time using OpenGL. The project simulates an underwater environment with realistic visual effects, animations, and dynamic lighting.

### 🔑 Key Features

- 🐟 **Animated fish** with movement AI (swim randomly within the tank)
- 🪨 **Rocks** with procedural placement
- 🌿 **Corals** with swaying animation
- 💧 **Water effects** including caustics, fog, and shimmer
- 🏠 **Aquarium environment** with glass walls, metal frame, and sandy floor
- 🌅 **Dynamic skybox** (switches between underwater and room view)
- 🎮 **Camera control** using keyboard (WASD) and mouse

---

## 🎮 Camera Controls

| Key/Action     | Function                  |
|----------------|---------------------------|
| **W**          | Move forward              |
| **S**          | Move backward             |
| **A**          | Move left                 |
| **D**          | Move right                |
| **Mouse Drag** | Rotate view (hold LMB)    |
| **Scroll**     | Zoom in/out (adjust FOV)  |

**Camera Limits:**
- Height range: -0.8 to 8.0 units
- Automatic skybox switch based on camera position
- Movement speed: 2.5 units/second
- Mouse sensitivity: 0.1 (can be changed in code)

---

## 📁 Project Structure

### 🔧 Core Files

| File               | Description                                           |
|--------------------|-------------------------------------------------------|
| `main_file.cpp`    | Main application loop, OpenGL initialization, camera |
| `shaderprogram.*`  | Shader loader and manager                            |
| `vertices.*`       | Basic geometry definitions for glass, frame, skybox  |

### 🎨 Object Classes

| File             | Description                                           |
|------------------|-------------------------------------------------------|
| `ModelLoader.*`  | OBJ model loader with PNG texture support             |
| `Stone.*`        | Rock class with random placement and rendering        |
| `Fish.*`         | Fish class with AI, movement, and boundary handling   |
| `Coral.*`        | Coral class with swaying animation and collision logic|

### 🖼️ Libraries

| File             | Description                        |
|------------------|------------------------------------|
| `lodepng.*`      | PNG texture loading (zlib license) |
| `constants.h`    | Math constants (e.g., PI)          |

### 🎭 Shaders – Vertex

Includes specialized shaders for each object (e.g., `v_fish.glsl`, `v_coral.glsl`) with animations, reflections, fog, and lighting logic.

### 🌈 Shaders – Fragment

Each object has its own lighting model: caustics, ambient fog, shimmer, and Fresnel effects. Full list in the original document.

---

## 🧪 Customization

You can easily modify colors, lighting, and effects by editing the GLSL shader files. The README includes:
- Code snippets for adjusting underwater colors
- How to disable specific light sources or effects
- Changing fog density, caustic intensity, and background color

---

## 🛠️ Compilation

### Requirements:
- **OpenGL 3.3+**
- **GLFW** – window/input handling
- **GLEW** – OpenGL extensions
- **GLM** – vector/matrix math

### Compile on Linux/macOS:
```bash
g++ -std=c++17 main_file.cpp shaderprogram.cpp vertices.cpp lodepng.cpp \
    ModelLoader.cpp Stone.cpp Fish.cpp Coral.cpp \
    -lGL -lGLEW -lglfw -o aquarium
```

### Compile on Windows (MinGW):
```bash
g++ -std=c++17 main_file.cpp shaderprogram.cpp vertices.cpp lodepng.cpp \
    ModelLoader.cpp Stone.cpp Fish.cpp Coral.cpp \
    -lopengl32 -lglfw3 -lglew32 -o aquarium.exe
```

---

## 📐 Aquarium Dimensions

| Component       | Values                                  |
|-----------------|------------------------------------------|
| Aquarium Size   | 16×5×12 units (X: -8 to 8, Y: -1 to 4, Z: -6 to 6) |
| Rocks           | 6 total, scale 0.3–0.8, min. 2 units apart |
| Corals          | 3–5, animated swaying, scale 0.4–0.8     |
| Fish            | 8–12, 5 types, scale 0.15–0.25, AI-driven |
| Glass Walls     | 15–35% transparency, Fresnel effect      |
| Frame           | 8.0f thick, metal-like material          |

---

## 🚀 Potential Extensions

- More coral types with unique animations
- Fish of varying sizes
- Day/night lighting transitions
- Particle system (bubbles, plankton)
- Real-time GUI for user settings
- Touch controls for mobile support

---

## 📝 Author

3D Aquarium project for OpenGL graphics course – Poznan University of Technology

---

## 📄 License

- **LodePNG**: zlib License (by Lode Vandevenne)  
- **All other code**: MIT License
