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
    this->normal_map = ResourceManager::GetTexture("normal");
    for (int i = 0; i < this->wave_num; ++i) {
        this->waves[i] = this->wave_generator();
    }
    this->init();
}

WaterWave::~WaterWave() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    for (int i = 0; i < this->wave_num; ++i) {
        delete this->waves[i];
    }
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
    this->waves[0]->time += deltatime;
    if (this->waves[0]->time > this->waves[0]->period) {
        this->waves[0]->time -= this->waves[0]->period;
    }
}

void WaterWave::draw(glm::vec3 camera_position) {
    this->shader.Use();

    this->shader.SetInteger("sprite", 0);

    for (int i = 0; i < this->wave_num; ++i) {
        this->wave_transformer(i);
    }

    this->shader.SetVector2f("v_offset", glm::vec2(this->texture_pos_x, this->texture_pos_y));
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glBindVertexArray(this->VAO);
    glActiveTexture(GL_TEXTURE0);
    this->picture.Bind();

    int camera_x = int(lround((camera_position.x / width + 0.5) * this->water_size));
    int camera_y = int(lround((camera_position.z / width + 0.5) * this->water_size));
    int t = 0;
    while (true) {
        int xu = camera_x - t, xd = camera_x + t, yl = camera_y - t, yr = camera_y + t;
        int valid_xu = std::max(xu, 0), valid_xd = std::min(xd, this->water_size - 1);
        int valid_yl = std::max(yl, 0), valid_yr = std::min(yr, this->water_size - 1);
        if (xu < 0 && xd >= this->water_size && yl < 0 && yr >= this->water_size) {
            break;
        }
        if (xu >= 0) {
            glDrawElements(GL_TRIANGLES, (valid_yr - valid_yl + 1) * 6, GL_UNSIGNED_INT,
                           (void *) (6 * sizeof(GLint) * (xu * this->water_size + valid_yl)));
        }
        if (xd < this->water_size) {
            glDrawElements(GL_TRIANGLES, (valid_yr - valid_yl + 1) * 6, GL_UNSIGNED_INT,
                           (void *) (6 * sizeof(GLint) * (xd * this->water_size + valid_yl)));
        }
        if (yl >= 0) {
            for (int i = valid_xu + 1; i < valid_xd; ++i) {
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                               (void *) (6 * sizeof(GLint) * (i * this->water_size + valid_yl)));
            }
        }
        if (yr < this->water_size) {
            for (int i = valid_xu + 1; i < valid_xd; ++i) {
                glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
                               (void *) (6 * sizeof(GLint) * (i * this->water_size + valid_yr)));
            }
        }
        t++;
    }
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE0);
}

void WaterWave::load() {
    ResourceManager::LoadShader("static_source/glsl_source/waterwave.vert.glsl",
                                "static_source/glsl_source/waterwave.frag.glsl", nullptr, "water_wave");
    ResourceManager::LoadTexture("static_source/images/SkyBox/SkyBox5.bmp", false, GL_REPEAT, "water_wave");
    ResourceManager::LoadTexture("static_source/images/normalMap.png", false, GL_REPEAT, "normal");
    // Bind camera uniform buffer to WaterWave shader
    unsigned int uniformBlockIndexWaterWave = glGetUniformBlockIndex(ResourceManager::GetShader("water_wave").ID,
                                                                     "Matrices");
    glUniformBlockBinding(ResourceManager::GetShader("water_wave").ID, uniformBlockIndexWaterWave, 0);
}

Wave *WaterWave::wave_generator() {
    auto *wave = new(Wave);
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> uniform(-1.0, 1.0);

    wave->kx = float(uniform(mt) * 4);
    wave->ky = float(uniform(mt) * 4);
    wave->amplitude = float((uniform(mt) + 1.0) / 30 + 0.02);
    wave->omega = float((uniform(mt) + 2) / 2);
    wave->period = 2 * float(M_PI) / wave->omega;
    wave->time = 0;
    return wave;
}

void WaterWave::wave_transformer(int idx) {
    std::ostringstream name1, name2, name3, name4;
    name1 << "waves[" << idx << "].k";
    name2 << "waves[" << idx << "].amplitude";
    name3 << "waves[" << idx << "].omega";
    name4 << "waves[" << idx << "].time";
    this->shader.SetVector2f(name1.str().c_str(), glm::vec2(this->waves[idx]->kx, this->waves[idx]->ky));
    this->shader.SetFloat(name2.str().c_str(), this->waves[idx]->amplitude);
    this->shader.SetFloat(name3.str().c_str(), this->waves[idx]->omega);
    this->shader.SetFloat(name4.str().c_str(), this->waves[idx]->time);
}