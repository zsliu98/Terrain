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
uniform float direction;

void main()
{
    if (direction < 0 && aPos.z < 0){
        visible = -1;
    }
    else if (direction > 0 && aPos.z < -0.1){
        visible = 0.5;
    } else {
        visible = 1;
    }
    TexCoords = vPos;
    gl_Position = projection * view * vec4(aPos.x, direction * aPos.z, aPos.y, 1.0);
}