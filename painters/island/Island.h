//
// Created by 柳子澍 on 2019/11/6.
//

#ifndef TERRAIN_ISLAND_H
#define TERRAIN_ISLAND_H

#include "../../manager/ResourceManager.h"
#include "../../manager/shader/shader.h"
#include "../../manager/texture/texture.h"

class Island {
private:
    Shader shader;
    GLuint VAO, VBO, EBO;

    void init();

public:
    explicit Island(Shader shader);

    ~Island();

    void step();

    void draw();
};


#endif //TERRAIN_ISLAND_H
