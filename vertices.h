#ifndef VERTICES_H
#define VERTICES_H

// Glass walls vertices (4 walls of aquarium) - 24 vertices * 8 floats each
extern float glassWallVertices[192];

// Frame vertices (metal/plastic edges) - 24 vertices * 3 floats each
extern float frameVertices[72];

// Sand floor vertices (fills entire aquarium glass area) - 6 vertices * 5 floats each
extern float sandVertices[30];

// Outside floor vertices (larger area around aquarium) - 6 vertices * 5 floats each
extern float outsideFloorVertices[30];

// Underwater skybox vertices (large cube around camera) - 36 vertices * 3 floats each
extern float skyboxVertices[108];

// Water fog box vertices (inside aquarium bounds) - 36 vertices * 3 floats each
extern float waterFogVertices[108];

#endif // VERTICES_H