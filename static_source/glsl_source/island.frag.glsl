#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
in float visible;

uniform sampler2D sprite;
uniform sampler2D detail;
uniform sampler2D grass;

void main()
{
    if (visible < 0) {
        discard;
    }
    vec4 tempFrag1 = texture(sprite, TexCoords);
    vec4 tempFrag2 = texture(detail, vec2(TexCoords.x * 15, TexCoords.y * 15));
    vec4 tempFrag3 = texture(grass, vec2(TexCoords.x * 20, TexCoords.y * 20));
    float factor = tempFrag1.y / (tempFrag1.x + tempFrag1.z) * 2 / 1.3;
    vec4 _FragColor;
    if (factor < 1.0) {
        _FragColor = tempFrag1 + tempFrag2 - 0.5;
    } else if (factor < 2.0){
        _FragColor = tempFrag1 + (2 - factor) * tempFrag2 + (factor - 1) * tempFrag3  - 0.5;
    } else {
        _FragColor = tempFrag1 + tempFrag3 - 0.5;
    }
    FragColor = vec4(_FragColor.xyz, visible);
}