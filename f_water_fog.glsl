#version 330 core

out vec4 FragColor;

in vec3 WorldPos;
in vec3 ViewPos;
in float DistanceFromCamera;

uniform float time;
uniform vec3 cameraPos;

// 3D noise function for volumetric fog
float noise3D(vec3 p) {
    return sin(p.x * 2.1) * cos(p.y * 1.7) * sin(p.z * 2.3);
}

float fractalNoise3D(vec3 p) {
    float value = 0.0;
    float amplitude = 1.0;
    float frequency = 1.0;
    
    for (int i = 0; i < 4; i++) {
        value += amplitude * noise3D(p * frequency);
        amplitude *= 0.5;
        frequency *= 2.0;
    }
    
    return value;
}

void main()
{
    // Base fog color - subtle blue-green underwater tint
    vec3 fogColor = vec3(0.2, 0.4, 0.6);
    
    // Animated fog position for movement
    vec3 fogPos = WorldPos + vec3(time * 0.1, time * 0.05, time * 0.08);
    
    // Multiple layers of 3D noise for realistic fog volume
    float fog1 = fractalNoise3D(fogPos * 0.8);
    float fog2 = fractalNoise3D(fogPos * 1.2 + vec3(100.0));
    float fog3 = fractalNoise3D(fogPos * 0.5 + vec3(200.0));
    
    // Combine fog layers
    float fogDensity = (fog1 + fog2 * 0.7 + fog3 * 0.5) / 3.0;
    fogDensity = (fogDensity + 1.0) * 0.5; // Normalize to [0,1]
    
    // Add depth-based fog variation
    float depthFactor = (WorldPos.y + 1.0) / 5.0; // From bottom (-1) to top (4)
    depthFactor = clamp(depthFactor, 0.0, 1.0);
    
    // More fog at the bottom, less at the top
    fogDensity *= (1.0 - depthFactor * 0.3);
    
    // Distance-based fog intensity
    float distanceFog = 1.0 - exp(-DistanceFromCamera * 0.08);
    fogDensity *= distanceFog;
    
    // Subtle particle effect
    float particles = sin(WorldPos.x * 15.0 + time * 3.0) * 
                     cos(WorldPos.y * 12.0 + time * 2.0) * 
                     sin(WorldPos.z * 18.0 + time * 2.5);
    particles = smoothstep(0.6, 1.0, particles) * 0.1;
    
    // Light ray simulation through fog
    vec3 lightDir = normalize(vec3(0.3, 1.0, 0.2));
    float lightRays = max(0.0, dot(normalize(ViewPos), lightDir));
    lightRays = pow(lightRays, 4.0) * 0.15;
    
    // Combine all fog effects
    fogDensity = clamp(fogDensity * 0.3 + particles + lightRays, 0.0, 0.4);
    
    // Add subtle color variation based on position
    vec3 colorVariation = fogColor;
    colorVariation += vec3(0.1, 0.05, 0.0) * sin(WorldPos.x * 0.5);
    colorVariation += vec3(0.05, 0.1, 0.1) * cos(WorldPos.z * 0.3);
    
    // Final fog with transparency
    FragColor = vec4(colorVariation, fogDensity);
}