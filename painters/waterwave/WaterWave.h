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

#ifndef TERRAIN_WATERWAVE_H
#define TERRAIN_WATERWAVE_H

#include <cmath>
#include <random>

#include "../../manager/ResourceManager.h"
#include "../../manager/shader/shader.h"
#include "../../manager/texture/texture.h"

struct Wave {
    float kx, ky;
    float amplitude, omega, time, period;
};

class WaterWave {

private:
    Shader shader;
    Texture2D picture, normal_map;
    GLuint VAO, VBO, EBO;
    const GLfloat width = 80.0, height = 1, texture_num = 30;
    const int smooth_level = 1;
    const int smooth[8] = {1, 4, 16, 64, 128, 256, 512, 1024};
    int water_size;
    const int wave_num = 5;
    Wave *waves[5];
    GLfloat texture_pos_x = 0.0, texture_pos_y = 0.0;

    void init();

    Wave *wave_generator();

    void wave_transformer(int idx);


public:
    explicit WaterWave();

    ~WaterWave();

    void step(GLfloat deltatime);

    void draw(glm::vec3 camera_position);

    static void load();

};


#endif //TERRAIN_WATERWAVE_H
