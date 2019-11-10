#version 330 core
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D sprite;
uniform sampler2D detail;
uniform sampler2D grass;

void main()
{
    if (TexCoords.x < 0) {
        discard;
    } else {
        vec4 tempFrag1 = texture(sprite, TexCoords);
        vec4 tempFrag2 = texture(detail, vec2(TexCoords.x * 10, TexCoords.y * 10));
        vec4 tempFrag3 = texture(grass, vec2(TexCoords.x * 10, TexCoords.y * 10));
        float factor = tempFrag1.y / (tempFrag1.x + tempFrag1.z) * 2 / 1.5;
        if (factor < 1.0) {
            FragColor = tempFrag1 + tempFrag2 - 0.5;
        } else if (factor < 2.0){
            FragColor = tempFrag1 + (2 - factor) * tempFrag2 + (factor - 1) * tempFrag3  - 0.5;
        } else {
            FragColor = tempFrag1 + tempFrag3 - 0.5;
        }
    }
}