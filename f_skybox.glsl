#version 330 core

out vec4 FragColor;

in vec3 WorldPos;
in vec3 LocalPos;

uniform float time;
uniform vec3 cameraPos;

// Enhanced noise function for better water distortion
float noise(vec3 p) {
    return sin(p.x * 1.5 + time * 0.5) * cos(p.y * 1.3 + time * 0.3) * sin(p.z * 1.7 + time * 0.4);
}

// Multiple octaves of noise for complex water patterns
float fractalNoise(vec3 p) {
    float value = 0.0;
    float amplitude = 1.0;
    float frequency = 1.0;
    
    for (int i = 0; i < 5; i++) {
        value += amplitude * noise(p * frequency);
        amplitude *= 0.5;
        frequency *= 2.0;
    }
    
    return value;
}

// Simulate underwater water distortion
vec3 waterDistortion(vec3 pos, float time) {
    vec3 distorted = pos;
    
    // Multiple layers of wave distortion
    distorted.x += sin(pos.y * 3.0 + time * 2.0) * 0.1;
    distorted.y += cos(pos.x * 2.5 + time * 1.5) * 0.1;
    distorted.z += sin(pos.x * 4.0 + pos.y * 3.0 + time * 2.5) * 0.08;
    
    return distorted;
}

void main()
{
    vec3 pos = normalize(LocalPos);
    
    // Apply water distortion to the viewing direction
    vec3 distortedPos = waterDistortion(pos, time);
    distortedPos = normalize(distortedPos);
    
    // Brighter underwater colors - more natural aquarium lighting
    vec3 deepWater = vec3(0.1, 0.2, 0.35);        // Lighter deep blue
    vec3 midWater = vec3(0.2, 0.35, 0.5);         // Medium blue
    vec3 shallowWater = vec3(0.3, 0.5, 0.7);      // Light blue
    vec3 surfaceWater = vec3(0.4, 0.6, 0.8);      // Very light blue
    
    // Enhanced vertical gradient with more variation
    float heightFactor = distortedPos.y * 0.5 + 0.5;
    heightFactor = smoothstep(0.0, 1.0, heightFactor); // Smooth transition
    
    // Multi-level color mixing
    vec3 baseColor;
    if (heightFactor < 0.3) {
        baseColor = mix(deepWater, midWater, heightFactor / 0.3);
    } else if (heightFactor < 0.7) {
        baseColor = mix(midWater, shallowWater, (heightFactor - 0.3) / 0.4);
    } else {
        baseColor = mix(shallowWater, surfaceWater, (heightFactor - 0.7) / 0.3);
    }
    
    // Enhanced animated water distortions
    vec3 wavePos1 = distortedPos + vec3(time * 0.15, time * 0.08, time * 0.12);
    float wave1 = fractalNoise(wavePos1 * 2.5);
    
    vec3 wavePos2 = distortedPos + vec3(-time * 0.12, time * 0.1, -time * 0.09);
    float wave2 = fractalNoise(wavePos2 * 3.5);
    
    vec3 wavePos3 = distortedPos + vec3(time * 0.08, -time * 0.15, time * 0.11);
    float wave3 = fractalNoise(wavePos3 * 1.8);
    
    float totalWaves = (wave1 + wave2 * 0.7 + wave3 * 0.5) * 0.12;
    
    // Intense underwater caustics everywhere
    float causticScale = 12.0;
    float caustic1 = sin(distortedPos.x * causticScale + time * 3.0) * cos(distortedPos.z * causticScale * 0.8 + time * 2.0);
    float caustic2 = cos(distortedPos.x * causticScale * 1.3 + time * 2.5) * sin(distortedPos.z * causticScale * 1.1 + time * 3.5);
    float caustic3 = sin(distortedPos.y * causticScale * 0.6 + time * 1.8) * cos(distortedPos.x * causticScale * 0.9 + time * 2.8);
    
    float caustics = (caustic1 + caustic2 + caustic3) / 3.0;
    caustics = max(caustics, 0.0) * 0.2; // Brighter caustics everywhere
    
    // Floating particles/bubbles
    float particles = 0.0;
    for (int i = 0; i < 4; i++) {
        vec3 particlePos = distortedPos + vec3(float(i) * 2.5, time * (0.4 + float(i) * 0.15), float(i) * 1.8);
        float particle = smoothstep(0.98, 1.0, fractalNoise(particlePos * 20.0));
        particles += particle * 0.4;
    }
    
    // Strong volumetric light rays from above
    float lightRays = 0.0;
    if (distortedPos.y > -0.2) {
        float rayIntensity = max(0.0, distortedPos.y + 0.2);
        float rayPattern1 = sin(distortedPos.x * 15.0 + time * 0.8) * cos(distortedPos.z * 12.0 + time * 0.6);
        float rayPattern2 = cos(distortedPos.x * 8.0 + time * 1.2) * sin(distortedPos.z * 18.0 + time * 0.9);
        lightRays = max((rayPattern1 + rayPattern2) * 0.5, 0.0) * rayIntensity * 0.25;
    }
    
    // Additional side lighting for aquarium effect
    float sideLighting = 0.0;
    float sideIntensity = 1.0 - abs(distortedPos.y); // Stronger at sides
    sideLighting = sin(distortedPos.x * 8.0 + time * 1.5) * cos(distortedPos.z * 6.0 + time * 1.0) * sideIntensity * 0.1;
    sideLighting = max(sideLighting, 0.0);
    
    // Combine all effects
    vec3 finalColor = baseColor;
    finalColor += vec3(totalWaves * 0.3, totalWaves * 0.4, totalWaves * 0.5); // Wave distortion
    finalColor += vec3(caustics * 0.9, caustics * 1.1, caustics * 1.3); // Bright caustics
    finalColor += vec3(particles * 0.7, particles * 0.9, particles * 1.0); // Floating particles
    finalColor += vec3(lightRays * 1.3, lightRays * 1.2, lightRays * 1.0); // Strong light rays
    finalColor += vec3(sideLighting * 1.2, sideLighting * 1.1, sideLighting * 1.0); // Side lighting
    
    // Color temperature variation based on direction
    float warmth = dot(normalize(distortedPos), vec3(0.5, 0.8, 0.3));
    finalColor += vec3(0.05, 0.08, 0.1) * warmth;
    
    // Subtle depth variation - closer to camera is brighter
    float depthFactor = 1.0 + dot(normalize(distortedPos), vec3(0.0, 0.0, 1.0)) * 0.2;
    finalColor *= depthFactor;
    
    // Reduced vignette for better visibility
    float vignette = 1.0 - length(distortedPos.xy) * 0.15;
    finalColor *= vignette;
    
    FragColor = vec4(finalColor, 1.0);
}