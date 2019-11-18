#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in vec3 s_normal;
in vec3 FragPos;

uniform sampler2D sprite;
uniform sampler2D normalMap;
uniform vec3 lightDir;
uniform vec3 viewPos;

void main()
{
    vec3 lightNormal = normalize(-lightDir);
    vec3 m_normal = normalize(2 * texture(normalMap, TexCoords).rgb - 1);
    vec3 normal = normalize(m_normal + s_normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightNormal, normal);
    float ambi = 0.2;
    float diff = max(dot(normal, lightDir), 0.0);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 4);
    float visible = max((1 - (FragPos.x * FragPos.x + FragPos.y * FragPos.y) / 1600) * 1, 0.1) - 0.1;
    vec3 _FragColor = vec3(40.0/256, 69.0/255, 150.0/255);
    FragColor = vec4(_FragColor * (ambi + diff + spec), visible);
}