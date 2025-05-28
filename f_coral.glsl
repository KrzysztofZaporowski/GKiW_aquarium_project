// f_coral.glsl - Fragment shader for corals
#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D coralTexture;
uniform float time;
uniform vec3 cameraPos;

void main() {
    vec4 texColor = texture(coralTexture, TexCoord);
    
    if(texColor.a < 0.1)
        discard;
    
    vec3 norm = normalize(Normal);
    
    // Simple lighting similar to stones but slightly more colorful for corals
    vec3 lightDir = normalize(vec3(0.2, 1.0, 0.3));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    
    vec3 ambient = vec3(0.35, 0.4, 0.5); // Slightly brighter ambient for colorful corals
    vec3 result = (ambient + diffuse * 0.7) * texColor.rgb;
    
    // Light underwater fog effect (same as fish)
    float distance = length(cameraPos - FragPos);
    float fogFactor = exp(-distance * 0.008);
    vec3 fogColor = vec3(0.05, 0.1, 0.15);
    
    vec3 finalColor = mix(fogColor, result, fogFactor);
    
    FragColor = vec4(finalColor, texColor.a);
}