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

#include "WaterWave.h"

void WaterWave::init() {
    GLfloat wave_vector[] = {
            -width, -width, 0, -width, width, 0, width, width, 0, width, -width, 0
    };
    GLfloat wave_texture[] = {
            0.0f, 0.0f, 0.0f, texture_num, texture_num, texture_num, texture_num, 0.0f,
    };

    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    GLfloat temp[(sizeof(wave_vector) + sizeof(wave_texture)) / sizeof(GLfloat)];
    glBufferData(GL_ARRAY_BUFFER, sizeof(temp), temp, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(wave_vector), wave_vector);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(wave_vector), sizeof(wave_texture), wave_texture);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *) (sizeof(wave_vector)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

WaterWave::WaterWave() {
    this->shader = ResourceManager::GetShader("water_wave");
    this->texture = ResourceManager::GetTexture("water_wave");
    this->init();
}

WaterWave::~WaterWave() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void WaterWave::step(GLfloat deltatime) {
    this->texture_pos_x += deltatime * 0.1;
    this->texture_pos_y += deltatime * 0.1;
    if (this->texture_pos_x >= 1.0) {
        this->texture_pos_x -= 1.0;
    }
    if (this->texture_pos_y >= 1.0) {
        this->texture_pos_y -= 1.0;
    }
}

void WaterWave::draw() {
    this->shader.Use();
    this->shader.SetVector2f("v_offset", glm::vec2(this->texture_pos_x, this->texture_pos_y));
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_SRC_COLOR);
    //glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(this->VAO);
    this->texture.Bind();
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
}

void WaterWave::load() {
    ResourceManager::LoadShader("static_source/glsl_source/waterwave.vert.glsl",
                                "static_source/glsl_source/waterwave.frag.glsl", nullptr, "water_wave");
    ResourceManager::LoadTexture("static_source/images/SkyBox/SkyBox6.bmp", false, GL_REPEAT, "water_wave");
    // Bind camera uniform buffer to WaterWave shader
    unsigned int uniformBlockIndexWaterWave = glGetUniformBlockIndex(ResourceManager::GetShader("water_wave").ID,
                                                                     "Matrices");
    glUniformBlockBinding(ResourceManager::GetShader("water_wave").ID, uniformBlockIndexWaterWave, 0);
}