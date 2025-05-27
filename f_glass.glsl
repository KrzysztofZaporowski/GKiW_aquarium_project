#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 WorldPos;
in vec3 Normal;
in float DistanceFromCamera;

uniform float time;
uniform vec3 cameraPos;

void main()
{
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(cameraPos - WorldPos);
    
    // Base glass color - very subtle blue-green tint
    vec3 glassColor = vec3(0.95, 0.98, 1.0);
    
    // Fresnel effect - more transparent when looking straight, more reflective at angles
    float fresnel = pow(1.0 - max(dot(viewDir, normal), 0.0), 2.0);
    fresnel = clamp(fresnel, 0.1, 0.8);
    
    // Fake reflection - simple environmental color
    vec3 reflectionColor = vec3(0.3, 0.5, 0.7); // Underwater environment color
    
    // Water distortion on glass surface
    float distortion = sin(WorldPos.y * 8.0 + time * 3.0) * cos(WorldPos.x * 6.0 + time * 2.0) * 0.1;
    distortion += cos(WorldPos.z * 10.0 + time * 2.5) * sin(WorldPos.y * 4.0 + time * 1.8) * 0.05;
    
    // Add subtle water droplets effect
    float droplets = sin(WorldPos.x * 20.0) * cos(WorldPos.y * 15.0) * sin(WorldPos.z * 18.0);
    droplets = smoothstep(0.7, 1.0, droplets) * 0.2;
    
    // Combine glass effects
    vec3 finalColor = mix(glassColor, reflectionColor, fresnel * 0.3);
    finalColor += vec3(distortion * 0.1, distortion * 0.15, distortion * 0.2);
    finalColor += vec3(droplets * 0.1);
    
    // Glass transparency - based on fresnel and distance
    float alpha = 0.15 + fresnel * 0.2;
    
    // Slight edge detection for glass thickness
    float edgeFactor = abs(dot(viewDir, normal));
    alpha += (1.0 - edgeFactor) * 0.3;
    
    FragColor = vec4(finalColor, alpha);
}