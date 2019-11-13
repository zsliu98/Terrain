#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 vPos;

out vec2 TexCoords;
out float visible;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform vec2 v_offset;

void main()
{
    TexCoords = vPos + v_offset;
    visible = max((1 - (aPos.x * aPos.x + aPos.y * aPos.y) / 1600) * 1, 0.1) - 0.1;
    gl_Position = projection * view * vec4(aPos.x, aPos.z, aPos.y, 1.0);
}