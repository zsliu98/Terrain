#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in float visible;

uniform sampler2D sprite;

void main()
{
    vec4 _FragColor = texture(sprite, TexCoords);
    FragColor = vec4(_FragColor.xyz, visible);
}