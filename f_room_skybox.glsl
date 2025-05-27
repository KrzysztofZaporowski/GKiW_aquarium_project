#version 330 core

out vec4 FragColor;

in vec3 WorldPos;
in vec3 LocalPos;

uniform float time;
uniform vec3 cameraPos;

void main()
{
    vec3 pos = normalize(LocalPos);
    
    // Bright room atmosphere - gradient from light walls to ceiling
    vec3 wallColor = vec3(0.95, 0.95, 0.97);      // Light gray walls
    vec3 ceilingColor = vec3(1.0, 1.0, 1.0);      // White ceiling
    vec3 floorColor = vec3(0.85, 0.85, 0.9);      // Slightly darker floor area
    
    // Vertical gradient based on Y position
    float heightFactor = pos.y * 0.5 + 0.5; // Convert from [-1,1] to [0,1]
    
    vec3 baseColor;
    if (heightFactor > 0.7) {
        // Upper area - ceiling
        float ceilingBlend = (heightFactor - 0.7) / 0.3;
        baseColor = mix(wallColor, ceilingColor, ceilingBlend);
    } else if (heightFactor < 0.3) {
        // Lower area - floor direction
        float floorBlend = (0.3 - heightFactor) / 0.3;
        baseColor = mix(wallColor, floorColor, floorBlend);
    } else {
        // Middle area - walls
        baseColor = wallColor;
    }
    
    // Add subtle room lighting variation
    float lightVariation = 0.95 + 0.05 * sin(pos.x * 2.0) * cos(pos.z * 1.5);
    baseColor *= lightVariation;
    
    // Subtle warm lighting
    vec3 warmTint = vec3(1.02, 1.01, 0.99);
    baseColor *= warmTint;
    
    FragColor = vec4(baseColor, 1.0);
}