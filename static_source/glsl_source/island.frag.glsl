#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D sprite;
uniform sampler2D detail;

void main()
{
    if (TexCoords.x < 0) {
        discard;
    } else {
        vec4 tempFrag1 = texture(sprite, TexCoords);
        vec4 tempFrag2 = texture(detail, vec2(TexCoords.x * 10, TexCoords.y * 10));
        FragColor = tempFrag1 + tempFrag2 - 0.5;
    }
}