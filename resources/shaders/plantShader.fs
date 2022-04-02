#version 330 core
out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec2 TexCoords;
    vec3 TangentLightPos;
    vec3 TangentViewPos;
    vec3 TangentFragPos;
} fs_in;

struct Material {
     sampler2D texture_diffuse1;
     sampler2D texture_specular1;
     sampler2D normalMap;

    float shininess;
};

struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
    vec3 normal = texture(material.normalMap, fs_in.TexCoords).rgb;
    normal = (normal * 2 - 1.0);
    vec3 color = texture(material.texture_diffuse1, fs_in.TexCoords).rgb;

    // ambient
    vec3 ambient = light.ambient * color;

    // diffuse
    vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = light.diffuse * diff * color;

    // specular
    vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);
    vec3 specular = light.specular * spec * texture(material.texture_specular1, fs_in.TexCoords).rgb;

     // attenuation
     float distance    = length(fs_in.TangentLightPos - fs_in.TangentFragPos);
     float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

     ambient  *= attenuation;
     diffuse   *= attenuation;
     specular *= attenuation;

    vec3 result = ambient + diffuse + specular;

    FragColor = vec4(result, 1.0);
}

