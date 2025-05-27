#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;

out vec2 TexCoord;
out vec3 FragPos;
out vec3 WorldPos;
out vec3 Normal;
out float DistanceFromCamera;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 cameraPos;

void main()
{
    WorldPos = vec3(M * vec4(aPos, 1.0));
    FragPos = WorldPos;
    TexCoord = aTexCoord;
    Normal = mat3(transpose(inverse(M))) * aNormal;
    
    DistanceFromCamera = length(cameraPos - WorldPos);
    
    gl_Position = P * V * vec4(WorldPos, 1.0);
}