#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in float visible;

uniform sampler2D sprite;

void main()
{
    vec4 _FragColor = texture(sprite, TexCoords);
    _FragColor.z = _FragColor.z + 0.1;
    FragColor = vec4(_FragColor.xyz, visible);
}