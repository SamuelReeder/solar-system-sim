#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform vec3 sunColor = vec3(1.0, 0.9, 0.7);

void main() {
    // calculate rim lighting effect (glow around the edges)
    vec3 normal = normalize(Normal);
    vec3 viewDir = normalize(-FragPos);
    float rim = 1.0 - max(dot(viewDir, normal), 0.0);
    rim = smoothstep(0.4, 1.0, rim);
    
    // combine base color with rim effect
    vec3 result = sunColor + rim * vec3(0.3, 0.2, 0.1);
    FragColor = vec4(result, 1.0);
}