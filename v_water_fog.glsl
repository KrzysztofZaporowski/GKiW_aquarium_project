#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 WorldPos;
out vec3 ViewPos;
out float DistanceFromCamera;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
uniform vec3 cameraPos;

void main()
{
    WorldPos = vec3(M * vec4(aPos, 1.0));
    ViewPos = vec3(V * vec4(WorldPos, 1.0));
    
    DistanceFromCamera = length(cameraPos - WorldPos);
    
    gl_Position = P * V * vec4(WorldPos, 1.0);
}