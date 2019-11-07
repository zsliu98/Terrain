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

#ifndef TERRAIN_SKYBOX_H
#define TERRAIN_SKYBOX_H

#include "../../manager/ResourceManager.h"
#include "../../manager/shader/shader.h"
#include "../../manager/texture/texture.h"

class SkyBox {
private:

    GLuint VAO, VBO, EBO;
    Texture2D textures[5];
    const GLfloat width = 10.0, height = 13;
    void init();

public:
    Shader shader;
    explicit SkyBox();

    ~SkyBox();

    void step();

    void draw();

    static void load();

};


#endif //TERRAIN_SKYBOX_H
