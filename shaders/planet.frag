#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace; // for shadow calculation

uniform vec3 lightPos;     // Sun position
uniform vec3 viewPos;      // Camera position
uniform vec3 lightColor;   // Sun light color
uniform vec3 objectColor;  // Planet color
uniform sampler2D shadowMap;
uniform bool receiveShadows = true;  // to disable shadows on the sun

float ShadowCalculation(vec4 fragPosLightSpace) {
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    
    // check whether current frag pos is in shadow
    float bias = 0.005;  // to avoid shadow acne
    float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
    
    // if outside of light frustum, don't shadow
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main() {
    // ambient
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor;
    
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    // calculate shadow (0.0 = no shadow, 1.0 = full shadow)
    float shadow = receiveShadows ? ShadowCalculation(FragPosLightSpace) : 0.0;
    
    // shadow reduces diffuse and specular light
    vec3 result = (ambient + (1.0 - shadow) * (diffuse + specular)) * objectColor;
    FragColor = vec4(result, 1.0);
}