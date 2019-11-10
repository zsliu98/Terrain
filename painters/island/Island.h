//
// Created by 柳子澍 on 2019/11/6.
//

#ifndef TERRAIN_ISLAND_H
#define TERRAIN_ISLAND_H

//#define GL_TEXTURE_ENV                    0x2300
//#define GL_COMBINE_RGB                    0x8571
//#define GL_ADD_SIGNED                     0x8574

#include "../../manager/ResourceManager.h"
#include "../../manager/shader/shader.h"
#include "../../manager/texture/texture.h"

class Island {
private:
    Shader shader;
    Texture2D picture, detail, grass;
    unsigned char* height_map;
    int map_size;
    GLuint VAO, VBO, EBO;
    const float width = 10.0, height = 1.5;

    void init();

public:
    explicit Island();

    ~Island();

    void step();

    void draw();

    static void load();
};


#endif //TERRAIN_ISLAND_H
