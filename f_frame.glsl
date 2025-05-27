#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 WorldPos;

uniform float time;
uniform vec3 cameraPos;

void main()
{
    // Metallic frame color - dark gray/black metal
    vec3 baseColor = vec3(0.2, 0.2, 0.25); // Dark metallic gray
    
    // Add metallic reflection based on viewing angle
    vec3 viewDir = normalize(cameraPos - WorldPos);
    float metallic = abs(sin(WorldPos.x * 10.0) * cos(WorldPos.y * 8.0) * sin(WorldPos.z * 12.0));
    metallic = smoothstep(0.3, 0.8, metallic) * 0.4;
    
    // Underwater lighting on frame
    vec3 lightDir = normalize(vec3(0.3, 1.0, 0.2));
    float lightIntensity = max(dot(lightDir, vec3(0.0, 1.0, 0.0)), 0.3); // Assume upward normal for simplicity
    
    // Add some underwater caustics reflection on metal
    float caustic = sin(WorldPos.x * 6.0 + time * 2.0) * cos(WorldPos.z * 4.0 + time * 1.5);
    caustic = max(caustic, 0.0) * 0.15;
    
    // Subtle oxidation/corrosion effect
    float corrosion = sin(WorldPos.x * 15.0) * cos(WorldPos.y * 12.0) * sin(WorldPos.z * 18.0);
    corrosion = smoothstep(0.6, 1.0, corrosion);
    vec3 corrosionColor = vec3(0.4, 0.3, 0.2); // Rust-like color
    
    // Combine effects
    vec3 finalColor = baseColor * lightIntensity;
    finalColor += vec3(metallic); // Metallic highlights
    finalColor += vec3(caustic * 0.2, caustic * 0.3, caustic * 0.4); // Underwater reflections
    finalColor = mix(finalColor, corrosionColor, corrosion * 0.3); // Add corrosion
    
    FragColor = vec4(finalColor, 1.0);
}