#version 330 core
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
out vec4 color;

uniform sampler2D texture_diffuse1;
uniform sampler2D texture_specular1;
uniform vec3 LightPos;
uniform vec3 ViewPos;
struct Material
{
    float shininess;
};
uniform Material material;

void main()
{
   // ambient
    vec3 ambient = 0.4f * texture(texture_diffuse1, TexCoords).rgb;
    
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);
    float diff = 0.6f * max( dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * texture(texture_diffuse1, TexCoords).rgb;
    
    // specular
    vec3 viewDir = normalize(ViewPos - FragPos);
    vec3 halfAngle = normalize(viewDir + lightDir);
    float spec = 0.9f * pow( max(dot(norm, halfAngle), 0.0f), material.shininess);
    vec3 specular = spec * texture(texture_specular1, TexCoords).rgb;
    
    color = vec4( ambient + diffuse + specular, 1.0f);
    //color = vec4(1.0f, 1.0f, 1.0f, 1.0f);    
}