/**<The is a terrain model driven by OpenGL and C++.>
Copyright (C) <2019>  <Zishu Liu>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.
**/

#include "SkyBox.h"

void SkyBox::init() {
    GLfloat box_vector[] = {
            width, -width, height, -width, -width, height, -width, width, height, width, width, height,
            width, -width, height, width, -width, 0.0f, -width, -width, 0.0f, -width, -width, height,
            width, width, height, width, width, 0.0f, width, -width, 0.0f, width, -width, height,
            -width, width, height, -width, width, 0.0f, width, width, 0.0f, width, width, height,
            -width, -width, height, -width, -width, 0.0f, -width, width, 0.0f, -width, width, height
    };
    GLfloat box_texture[] = {
            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    GLfloat temp[(sizeof(box_vector) + sizeof(box_texture)) / sizeof(GLfloat)];
    glBufferData(GL_ARRAY_BUFFER, sizeof(temp), temp, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(box_vector), box_vector);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(box_vector), sizeof(box_texture), box_texture);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *) (sizeof(box_vector)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

SkyBox::SkyBox() {
    this->shader = ResourceManager::GetShader("sky_box");
    this->textures[0] = ResourceManager::GetTexture("sky_box_u");
    this->textures[1] = ResourceManager::GetTexture("sky_box_l");
    this->textures[2] = ResourceManager::GetTexture("sky_box_r");
    this->textures[3] = ResourceManager::GetTexture("sky_box_f");
    this->textures[4] = ResourceManager::GetTexture("sky_box_b");
    this->init();
}

SkyBox::~SkyBox() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void SkyBox::step() {}

void SkyBox::draw() {
    this->shader.Use();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(this->VAO);
    this->shader.SetFloat("direction", 1.0);
    for (int i = 0; i < 5; ++i) {
        this->textures[i].Bind();
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }
    glDepthMask(GL_TRUE);
    this->shader.SetFloat("direction", -1.0);
    for (int i = 0; i < 5; ++i) {
        this->textures[i].Bind();
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }
}

void SkyBox::load() {
    ResourceManager::LoadShader("static_source/glsl_source/skybox.vert.glsl",
                                "static_source/glsl_source/skybox.frag.glsl", nullptr, "sky_box");
    ResourceManager::LoadTexture("static_source/images/SkyBox/SkyBox4.bmp", false, GL_CLAMP_TO_EDGE, "sky_box_u");
    ResourceManager::LoadTexture("static_source/images/SkyBox/SkyBox3.bmp", false, GL_CLAMP_TO_EDGE, "sky_box_l");
    ResourceManager::LoadTexture("static_source/images/SkyBox/SkyBox2.bmp", false, GL_CLAMP_TO_EDGE, "sky_box_r");
    ResourceManager::LoadTexture("static_source/images/SkyBox/SkyBox1.bmp", false, GL_CLAMP_TO_EDGE, "sky_box_f");
    ResourceManager::LoadTexture("static_source/images/SkyBox/SkyBox0.bmp", false, GL_CLAMP_TO_EDGE, "sky_box_b");
    // Bind camera uniform buffer to SkyBox shader
    unsigned int uniformBlockIndexSkyBox = glGetUniformBlockIndex(ResourceManager::GetShader("sky_box").ID, "Matrices");
    glUniformBlockBinding(ResourceManager::GetShader("sky_box").ID, uniformBlockIndexSkyBox, 0);
}