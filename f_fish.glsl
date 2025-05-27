#version 330 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D fishTexture;
uniform float time;
uniform vec3 cameraPos;

void main() {
    vec4 texColor = texture(fishTexture, TexCoord);
    
    if(texColor.a < 0.1)
        discard;
    
    vec3 norm = normalize(Normal);
    
    // Simple lighting without spotlights
    vec3 lightDir = normalize(vec3(0.2, 1.0, 0.3));
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0, 1.0, 1.0);
    
    vec3 ambient = vec3(0.3, 0.4, 0.5); // Underwater ambient
    vec3 result = (ambient + diffuse * 0.7) * texColor.rgb;
    
    // Subtle underwater fog effect
    float distance = length(cameraPos - FragPos);
    float fogFactor = exp(-distance * 0.03);
    vec3 fogColor = vec3(0.1, 0.2, 0.3);
    
    vec3 finalColor = mix(fogColor, result, fogFactor);
    
    FragColor = vec4(finalColor, texColor.a);
}