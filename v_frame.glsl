#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 FragPos;
out vec3 WorldPos;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

void main()
{
    WorldPos = vec3(M * vec4(aPos, 1.0));
    FragPos = WorldPos;
    
    gl_Position = P * V * vec4(WorldPos, 1.0);
}