#include "vertices.h"

// Glass walls vertices (4 walls of aquarium) - 24 vertices * 8 floats each = 192 floats
float glassWallVertices[192] = {
    // Front wall (glass) - closer to camera
    -8.0f, -1.0f,  6.0f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f, // bottom left
     8.0f, -1.0f,  6.0f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f, // bottom right  
     8.0f,  4.0f,  6.0f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f, // top right
     8.0f,  4.0f,  6.0f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f, // top right
    -8.0f,  4.0f,  6.0f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f, // top left
    -8.0f, -1.0f,  6.0f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f, // bottom left

    // Back wall (glass)
    -8.0f, -1.0f, -6.0f,  0.0f, 0.0f,  0.0f, 0.0f,  1.0f, // bottom left
     8.0f, -1.0f, -6.0f,  1.0f, 0.0f,  0.0f, 0.0f,  1.0f, // bottom right
     8.0f,  4.0f, -6.0f,  1.0f, 1.0f,  0.0f, 0.0f,  1.0f, // top right
     8.0f,  4.0f, -6.0f,  1.0f, 1.0f,  0.0f, 0.0f,  1.0f, // top right
    -8.0f,  4.0f, -6.0f,  0.0f, 1.0f,  0.0f, 0.0f,  1.0f, // top left
    -8.0f, -1.0f, -6.0f,  0.0f, 0.0f,  0.0f, 0.0f,  1.0f, // bottom left

    // Left wall (glass)
    -8.0f, -1.0f, -6.0f,  0.0f, 0.0f,  1.0f, 0.0f,  0.0f, // bottom back
    -8.0f, -1.0f,  6.0f,  1.0f, 0.0f,  1.0f, 0.0f,  0.0f, // bottom front
    -8.0f,  4.0f,  6.0f,  1.0f, 1.0f,  1.0f, 0.0f,  0.0f, // top front
    -8.0f,  4.0f,  6.0f,  1.0f, 1.0f,  1.0f, 0.0f,  0.0f, // top front
    -8.0f,  4.0f, -6.0f,  0.0f, 1.0f,  1.0f, 0.0f,  0.0f, // top back
    -8.0f, -1.0f, -6.0f,  0.0f, 0.0f,  1.0f, 0.0f,  0.0f, // bottom back

    // Right wall (glass)
     8.0f, -1.0f, -6.0f,  0.0f, 0.0f, -1.0f, 0.0f,  0.0f, // bottom back
     8.0f, -1.0f,  6.0f,  1.0f, 0.0f, -1.0f, 0.0f,  0.0f, // bottom front
     8.0f,  4.0f,  6.0f,  1.0f, 1.0f, -1.0f, 0.0f,  0.0f, // top front
     8.0f,  4.0f,  6.0f,  1.0f, 1.0f, -1.0f, 0.0f,  0.0f, // top front
     8.0f,  4.0f, -6.0f,  0.0f, 1.0f, -1.0f, 0.0f,  0.0f, // top back
     8.0f, -1.0f, -6.0f,  0.0f, 0.0f, -1.0f, 0.0f,  0.0f  // bottom back
};

// Frame vertices (metal/plastic edges) - 16 vertices * 3 floats each = 48 floats
float frameVertices[72] = {
    // Bottom frame - rectangular tubes around glass edges
    -8.0f, -1.0f, -6.0f,   8.0f, -1.0f, -6.0f,  // bottom back edge
     8.0f, -1.0f, -6.0f,   8.0f, -1.0f,  6.0f,  // bottom right edge
     8.0f, -1.0f,  6.0f,  -8.0f, -1.0f,  6.0f,  // bottom front edge
    -8.0f, -1.0f,  6.0f,  -8.0f, -1.0f, -6.0f,  // bottom left edge

    // Top frame - rectangular tubes around glass edges
    -8.0f,  4.0f, -6.0f,   8.0f,  4.0f, -6.0f,  // top back edge
     8.0f,  4.0f, -6.0f,   8.0f,  4.0f,  6.0f,  // top right edge
     8.0f,  4.0f,  6.0f,  -8.0f,  4.0f,  6.0f,  // top front edge
    -8.0f,  4.0f,  6.0f,  -8.0f,  4.0f, -6.0f,  // top left edge

    // Vertical frame posts (corner columns) - exactly at glass corners
    -8.0f, -1.0f, -6.0f,  -8.0f,  4.0f, -6.0f,  // back left post
     8.0f, -1.0f, -6.0f,   8.0f,  4.0f, -6.0f,  // back right post
     8.0f, -1.0f,  6.0f,   8.0f,  4.0f,  6.0f,  // front right post
    -8.0f, -1.0f,  6.0f,  -8.0f,  4.0f,  6.0f   // front left post
};

// Sand floor vertices (fills entire aquarium glass area) - 6 vertices * 5 floats each = 30 floats
float sandVertices[30] = {
    // positions          // texture coords (exactly to glass edges)
    -8.0f, -1.0f, -6.0f,  0.0f, 6.0f,
     8.0f, -1.0f, -6.0f,  8.0f, 6.0f,
     8.0f, -1.0f,  6.0f,  8.0f, 0.0f,
     8.0f, -1.0f,  6.0f,  8.0f, 0.0f,
    -8.0f, -1.0f,  6.0f,  0.0f, 0.0f,
    -8.0f, -1.0f, -6.0f,  0.0f, 6.0f
};

// Outside floor vertices (larger area around aquarium) - 6 vertices * 5 floats each = 30 floats
float outsideFloorVertices[30] = {
    // Large floor extending beyond aquarium
    -30.0f, -1.2f, -30.0f,  0.0f, 30.0f,
     30.0f, -1.2f, -30.0f, 30.0f, 30.0f,
     30.0f, -1.2f,  30.0f, 30.0f,  0.0f,
     30.0f, -1.2f,  30.0f, 30.0f,  0.0f,
    -30.0f, -1.2f,  30.0f,  0.0f,  0.0f,
    -30.0f, -1.2f, -30.0f,  0.0f, 30.0f
};

// Underwater skybox vertices (large cube around camera) - 36 vertices * 3 floats each = 108 floats
float skyboxVertices[108] = {
    // positions          
    -50.0f,  50.0f, -50.0f,
    -50.0f, -50.0f, -50.0f,
     50.0f, -50.0f, -50.0f,
     50.0f, -50.0f, -50.0f,
     50.0f,  50.0f, -50.0f,
    -50.0f,  50.0f, -50.0f,

    -50.0f, -50.0f,  50.0f,
    -50.0f, -50.0f, -50.0f,
    -50.0f,  50.0f, -50.0f,
    -50.0f,  50.0f, -50.0f,
    -50.0f,  50.0f,  50.0f,
    -50.0f, -50.0f,  50.0f,

     50.0f, -50.0f, -50.0f,
     50.0f, -50.0f,  50.0f,
     50.0f,  50.0f,  50.0f,
     50.0f,  50.0f,  50.0f,
     50.0f,  50.0f, -50.0f,
     50.0f, -50.0f, -50.0f,

    -50.0f, -50.0f,  50.0f,
    -50.0f,  50.0f,  50.0f,
     50.0f,  50.0f,  50.0f,
     50.0f,  50.0f,  50.0f,
     50.0f, -50.0f,  50.0f,
    -50.0f, -50.0f,  50.0f,

    -50.0f,  50.0f, -50.0f,
     50.0f,  50.0f, -50.0f,
     50.0f,  50.0f,  50.0f,
     50.0f,  50.0f,  50.0f,
    -50.0f,  50.0f,  50.0f,
    -50.0f,  50.0f, -50.0f,

    -50.0f, -50.0f, -50.0f,
    -50.0f, -50.0f,  50.0f,
     50.0f, -50.0f, -50.0f,
     50.0f, -50.0f, -50.0f,
    -50.0f, -50.0f,  50.0f,
     50.0f, -50.0f,  50.0f
};

// Water fog box vertices (inside aquarium bounds) - 36 vertices * 3 floats each = 108 floats
float waterFogVertices[108] = {
    // Cube that exactly fits inside aquarium bounds (-7.9 to 7.9, -0.9 to 3.9, -5.9 to 5.9)
    -7.9f,  3.9f, -5.9f,
    -7.9f, -0.9f, -5.9f,
     7.9f, -0.9f, -5.9f,
     7.9f, -0.9f, -5.9f,
     7.9f,  3.9f, -5.9f,
    -7.9f,  3.9f, -5.9f,

    -7.9f, -0.9f,  5.9f,
    -7.9f, -0.9f, -5.9f,
    -7.9f,  3.9f, -5.9f,
    -7.9f,  3.9f, -5.9f,
    -7.9f,  3.9f,  5.9f,
    -7.9f, -0.9f,  5.9f,

     7.9f, -0.9f, -5.9f,
     7.9f, -0.9f,  5.9f,
     7.9f,  3.9f,  5.9f,
     7.9f,  3.9f,  5.9f,
     7.9f,  3.9f, -5.9f,
     7.9f, -0.9f, -5.9f,

    -7.9f, -0.9f,  5.9f,
    -7.9f,  3.9f,  5.9f,
     7.9f,  3.9f,  5.9f,
     7.9f,  3.9f,  5.9f,
     7.9f, -0.9f,  5.9f,
    -7.9f, -0.9f,  5.9f,

    -7.9f,  3.9f, -5.9f,
     7.9f,  3.9f, -5.9f,
     7.9f,  3.9f,  5.9f,
     7.9f,  3.9f,  5.9f,
    -7.9f,  3.9f,  5.9f,
    -7.9f,  3.9f, -5.9f,

    -7.9f, -0.9f, -5.9f,
    -7.9f, -0.9f,  5.9f,
     7.9f, -0.9f, -5.9f,
     7.9f, -0.9f, -5.9f,
    -7.9f, -0.9f,  5.9f,
     7.9f, -0.9f,  5.9f
};