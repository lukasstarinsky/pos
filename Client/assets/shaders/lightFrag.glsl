#version 460 core
in vec3 Normal;
in vec3 FragPos;

out vec4 outColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 diffuse = max(dot(norm, lightDir), 0.0f) * lightColor;

    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
    vec3 specular = specularStrength * spec * lightColor;

    outColor = vec4((ambient + diffuse) * objectColor, 1.0f);
}