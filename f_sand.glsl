#version 330 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 FragPos;
in vec3 WorldPos;
in float DistanceFromCamera;

uniform sampler2D sandDiffuse;
uniform sampler2D sandDisplacement;
uniform float time;
uniform vec3 cameraPos;

void main()
{
    // Sample the diffuse texture (kolor bazowy piasku)
    vec4 diffuseColor = texture(sandDiffuse, TexCoord);
    
    // Sample displacement for height variation and detail
    float displacement = texture(sandDisplacement, TexCoord).r;
    
    // U¿yj displacement do stworzenia fake normal mapping effect
    vec2 texelSize = 1.0 / textureSize(sandDisplacement, 0);
    float heightL = texture(sandDisplacement, TexCoord - vec2(texelSize.x, 0.0)).r;
    float heightR = texture(sandDisplacement, TexCoord + vec2(texelSize.x, 0.0)).r;
    float heightD = texture(sandDisplacement, TexCoord - vec2(0.0, texelSize.y)).r;
    float heightU = texture(sandDisplacement, TexCoord + vec2(0.0, texelSize.y)).r;
    
    // Oblicz gradient do fake normal mapping
    vec3 normal;
    normal.x = heightL - heightR;
    normal.y = heightD - heightU;
    normal.z = 0.02; // Increased for better normal visibility
    normal = normalize(normal);
    
    // Dual light setup - main sun light and secondary ambient light
    
    // Main underwater sunlight (filtered from above)
    vec3 lightDir1 = normalize(vec3(0.3, 1.0, 0.2));
    vec3 lightColor1 = vec3(0.9, 1.0, 1.2); // Bright blue-white light
    
    // Secondary underwater light (bounce light/ambient)
    vec3 lightDir2 = normalize(vec3(-0.5, 0.3, 0.8));
    vec3 lightColor2 = vec3(0.6, 0.8, 1.1); // Softer blue light
    
    // Enhanced ambient lighting (brighter underwater environment)
    vec3 ambient = vec3(0.5, 0.6, 0.7); // Much brighter ambient
    
    // Diffuse lighting from both lights
    float diff1 = max(dot(normal, lightDir1), 0.0);
    float diff2 = max(dot(normal, lightDir2), 0.0);
    vec3 diffuse = (diff1 * lightColor1 * 0.7) + (diff2 * lightColor2 * 0.4);
    
    // Enhanced specular lighting for wet sand
    vec3 viewDir = normalize(cameraPos - WorldPos);
    
    // Specular from main light
    vec3 reflectDir1 = reflect(-lightDir1, normal);
    float spec1 = pow(max(dot(viewDir, reflectDir1), 0.0), 16.0);
    
    // Specular from secondary light
    vec3 reflectDir2 = reflect(-lightDir2, normal);
    float spec2 = pow(max(dot(viewDir, reflectDir2), 0.0), 8.0);
    
    vec3 specular = (spec1 * lightColor1 * 0.3 + spec2 * lightColor2 * 0.2) * displacement;
    
    // Bright underwater caustics effect
    float caustic1 = sin(FragPos.x * 4.0 + time * 2.0) * sin(FragPos.z * 3.0 + time * 1.5);
    float caustic2 = cos(FragPos.x * 6.0 + time * 2.5) * cos(FragPos.z * 5.0 + time * 1.8);
    float caustics = (caustic1 + caustic2) * 0.5;
    caustics = max(caustics, 0.0) * 0.25; // Brighter caustics
    
    // Enhanced sparkle effect
    float sparkle = pow(displacement, 3.0) * sin(time * 8.0 + FragPos.x * 10.0 + FragPos.z * 8.0);
    sparkle = max(sparkle, 0.0) * 0.2; // Brighter sparkles
    
    // Enhanced color variation (more contrast)
    vec3 colorVariation = mix(vec3(0.8, 0.7, 0.6), vec3(1.2, 1.1, 1.0), displacement);
    
    // Less blue tinting for better sand visibility
    vec3 underwaterTint = vec3(0.95, 1.0, 1.05); // Much less blue tint
    
    // Finalne oœwietlenie - much brighter
    vec3 finalColor = (ambient + diffuse + specular) * diffuseColor.rgb * colorVariation * underwaterTint;
    finalColor += vec3(caustics * 1.2, caustics * 1.1, caustics); // Bright caustics
    finalColor += vec3(sparkle * 0.9, sparkle, sparkle * 1.1); // Bright sparkles
    
    // Reduced fog effect for better visibility
    vec3 fogColor = vec3(0.15, 0.25, 0.35); // Lighter fog color
    float fogFactor = clamp(DistanceFromCamera / 50.0, 0.0, 0.6); // Less fog, farther range
    finalColor = mix(finalColor, fogColor, fogFactor);
    
    // Brighter underwater shimmer
    float shimmer = sin(time * 3.0 + FragPos.x * 2.0) * cos(time * 2.5 + FragPos.z * 1.8) * 0.04;
    finalColor += vec3(shimmer * 0.5, shimmer * 0.7, shimmer * 1.0);
    
    FragColor = vec4(finalColor, 1.0);
}