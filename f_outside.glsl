#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 WorldPos;
in float DistanceFromCamera;

uniform sampler2D floorDiffuse;
uniform sampler2D floorDisplacement;
uniform float time;
uniform vec3 cameraPos;

void main()
{
    // Sample the floor diffuse texture (tile/concrete/wood floor)
    vec4 diffuseColor = texture(floorDiffuse, TexCoord);
    
    // Sample displacement for surface detail
    float displacement = texture(floorDisplacement, TexCoord).r;
    
    // Normal mapping for floor surface
    vec2 texelSize = 1.0 / textureSize(floorDisplacement, 0);
    float heightL = texture(floorDisplacement, TexCoord - vec2(texelSize.x, 0.0)).r;
    float heightR = texture(floorDisplacement, TexCoord + vec2(texelSize.x, 0.0)).r;
    float heightD = texture(floorDisplacement, TexCoord - vec2(0.0, texelSize.y)).r;
    float heightU = texture(floorDisplacement, TexCoord + vec2(0.0, texelSize.y)).r;
    
    // Calculate normal for floor lighting
    vec3 normal;
    normal.x = heightL - heightR;
    normal.y = heightD - heightU;
    normal.z = 0.03; // Floor bumpiness
    normal = normalize(normal);
    
    // Bright room lighting setup
    vec3 lightDir1 = normalize(vec3(0.5, 1.0, 0.3)); // Main ceiling light
    vec3 lightColor1 = vec3(1.2, 1.15, 1.1); // Bright warm white light
    
    vec3 lightDir2 = normalize(vec3(-0.3, 0.8, -0.6)); // Secondary light
    vec3 lightColor2 = vec3(1.0, 1.05, 1.2); // Cool light
    
    // Bright ambient lighting (well-lit room)
    vec3 ambient = vec3(0.7, 0.7, 0.75); // Very bright ambient
    
    // Diffuse lighting from both lights
    float diff1 = max(dot(normal, lightDir1), 0.0);
    float diff2 = max(dot(normal, lightDir2), 0.0);
    vec3 diffuse = (diff1 * lightColor1 * 0.8) + (diff2 * lightColor2 * 0.5);
    
    // Specular highlights on floor
    vec3 viewDir = normalize(cameraPos - WorldPos);
    vec3 reflectDir1 = reflect(-lightDir1, normal);
    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), 32.0);
    vec3 specular = spec1 * lightColor1 * 0.3 * displacement;
    
    // Floor color variation
    vec3 colorVariation = mix(vec3(0.95, 0.95, 0.95), vec3(1.05, 1.05, 1.05), displacement);
    
    // No color tinting (normal room colors)
    vec3 roomTint = vec3(1.0, 1.0, 1.0);
    
    // Final lighting calculation - very bright
    vec3 finalColor = (ambient + diffuse + specular) * diffuseColor.rgb * colorVariation * roomTint;
    
    // NO FOG EFFECT - clear room atmosphere
    // Remove the fog completely for clear visibility
    
    FragColor = vec4(finalColor, 1.0);
}