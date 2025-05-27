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

void main() {
    FragPos = vec3(M * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(M))) * aNormal;
    TexCoord = aTexCoord;
    
    gl_Position = P * V * vec4(FragPos, 1.0);
}