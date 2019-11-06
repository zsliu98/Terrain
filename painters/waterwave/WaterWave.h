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

#include "../../manager/ResourceManager.h"
#include "../../manager/shader/shader.h"
#include "../../manager/texture/texture.h"

class WaterWave {

private:
    Shader shader;
    GLuint VAO, VBO, EBO;

    void init();

public:
    explicit WaterWave(Shader shader);

    ~WaterWave();

    void step();

    void draw();

};


#endif //TERRAIN_WATERWAVE_H
