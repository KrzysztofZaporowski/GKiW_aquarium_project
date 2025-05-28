// v_coral.glsl - Vertex shader for animated corals
#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 P;
uniform mat4 V;
uniform mat4 M;
uniform float time;
uniform float swayAmplitude;

void main() {
    vec3 pos = aPos;
    
    // Calculate height factor (0 at bottom, 1 at top)
    // Assuming coral grows upward from origin
    float heightFactor = max(0.0, pos.y + 1.0) / 4.0; // Adjust based on coral model
    heightFactor = clamp(heightFactor, 0.0, 1.0);
    
    // Gentle swaying animation - more movement at the top
    float swayAmountX = sin(time * 0.8) * 0.15 * heightFactor * swayAmplitude;  // Left-right sway
    float swayAmountZ = cos(time * 0.6) * 0.08 * heightFactor * swayAmplitude;  // Forward-back sway (smaller)
    
    // Apply swaying to position
    pos.x += swayAmountX;
    pos.z += swayAmountZ;
    
    // Transform position
    vec4 worldPos = M * vec4(pos, 1.0);
    FragPos = worldPos.xyz;
    
    // Transform normal (adjust for swaying)
    Normal = mat3(transpose(inverse(M))) * aNormal;
    TexCoord = aTexCoord;
    
    gl_Position = P * V * worldPos;
}
