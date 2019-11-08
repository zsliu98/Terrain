#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 vPos;

out vec2 TexCoords;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform vec2 v_offset;

void main()
{
    TexCoords = vPos + v_offset;
    gl_Position = projection * view * vec4(aPos.x, aPos.z, aPos.y, 1.0);
}