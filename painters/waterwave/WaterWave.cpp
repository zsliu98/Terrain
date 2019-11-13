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
    this->water_size = this->smooth[this->smooth_level] * int(this->width);
    GLfloat vector[water_size + 1][water_size + 1][3];
    GLfloat texture[water_size + 1][water_size + 1][2];
    GLint index[water_size * water_size * 6];
    for (int i = 0; i <= water_size; ++i) {
        for (int j = 0; j <= water_size; ++j) {
            vector[i][j][0] = width * float(i) / float(water_size) - 0.5 * width;
            vector[i][j][1] = width * float(j) / float(water_size) - 0.5 * width;
            vector[i][j][2] = 0;
            texture[i][j][1] = float(i) / float(water_size) * this->texture_num;
            texture[i][j][0] = float(j) / float(water_size) * this->texture_num;
        }
    }
    int idx = 0;
    for (int i = 0; i < water_size; ++i) {
        for (int j = 0; j < water_size; ++j) {
            int current_vector = i * (water_size + 1) + j;
            index[idx] = current_vector;
            index[idx + 2] = current_vector + water_size + 1;
            index[idx + 3] = current_vector + water_size + 1;
            index[idx + 1] = current_vector + 1;
            index[idx + 4] = current_vector + 1;
            index[idx + 5] = current_vector + water_size + 2;
            idx += 6;
        }
    }
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);
    glBindVertexArray(this->VAO);

    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    GLfloat temp[(sizeof(vector) + sizeof(texture)) / sizeof(GLfloat)];
    glBufferData(GL_ARRAY_BUFFER, sizeof(temp), temp, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vector), vector);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vector), sizeof(texture), texture);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *) (sizeof(vector)));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

WaterWave::WaterWave() {
    this->shader = ResourceManager::GetShader("water_wave");
    this->picture = ResourceManager::GetTexture("water_wave");
    this->init();
}

WaterWave::~WaterWave() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
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
    glBlendFunc(GL_ALPHA, GL_SRC_COLOR);
    //glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    this->picture.Bind();
    glDrawElements(GL_TRIANGLES, this->water_size * this->water_size * 6, GL_UNSIGNED_INT, nullptr);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE0);
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