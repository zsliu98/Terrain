#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 vPos;

out vec2 TexCoords;
out vec3 s_normal;
out vec3 FragPos;

layout (std140) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

uniform vec2 v_offset;

struct Wave
{
    vec2 k;
    float amplitude, omega, time;
};

uniform Wave[5] waves;

void main()
{
    TexCoords = vPos + v_offset;
    vec3 pos = vec3(aPos.x, aPos.y, aPos.z);
    float hide_factor = min((aPos.x * aPos.x + aPos.y * aPos.y) / 125, 1);
    float phi = dot(waves[0].k, aPos.xy) - waves[0].omega * waves[0].time;
    pos.xy = pos.xy - normalize(waves[0].k) * waves[0].amplitude * sin(phi) * hide_factor;
    pos.z = pos.z + waves[0].amplitude * cos(phi) * hide_factor;
    phi = dot(waves[2].k, aPos.xy) - waves[2].omega * waves[2].time;
    pos.xy = pos.xy - normalize(waves[2].k) * waves[2].amplitude * sin(phi) * hide_factor;
    pos.z = pos.z + waves[2].amplitude * cos(phi) * hide_factor;
    phi = dot(waves[2].k, aPos.xy) - waves[2].omega * waves[2].time;
    pos.xy = pos.xy - normalize(waves[2].k) * waves[2].amplitude * sin(phi) * hide_factor;
    pos.z = pos.z + waves[2].amplitude * cos(phi) * hide_factor;
    phi = dot(waves[3].k, aPos.xy) - waves[3].omega * waves[3].time;
    pos.xy = pos.xy - normalize(waves[3].k) * waves[3].amplitude * sin(phi) * hide_factor;
    pos.z = pos.z + waves[3].amplitude * cos(phi) * hide_factor;
    phi = dot(waves[4].k, aPos.xy) - waves[4].omega * waves[4].time;
    pos.xy = pos.xy - normalize(waves[4].k) * waves[4].amplitude * sin(phi) * hide_factor;
    pos.z = pos.z + waves[4].amplitude * cos(phi) * hide_factor;
    s_normal = normalize(pos - aPos);
    FragPos = pos;
    gl_Position = projection * view * vec4(pos.x, pos.z, pos.y, 1.0);
}