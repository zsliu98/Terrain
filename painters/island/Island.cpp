//
// Created by 柳子澍 on 2019/11/6.
//

#include <OpenGL/OpenGL.h>
#include "Island.h"

void Island::init() {
    GLfloat vector[this->map_size + 1][this->map_size + 1][3];
    GLfloat texture[this->map_size + 1][this->map_size + 1][2];
    GLfloat nvector[this->map_size + 1][this->map_size + 1][3];
    GLint index[this->map_size * this->map_size * 6];
    for (int i = 0; i <= this->map_size; ++i) {
        for (int j = 0; j <= this->map_size; ++j) {
            vector[i][j][0] = width * float(i) / float(this->map_size) - 0.5 * width;
            vector[i][j][1] = width * float(j) / float(this->map_size) - 0.5 * width;
            if (i == this->map_size) {
                vector[i][j][2] = vector[i - 1][j][2] - 0.1;
            } else if (j == this->map_size) {
                vector[i][j][2] = vector[i][j - 1][2] - 0.1;
            } else {
                vector[i][j][2] = float(this->height_map[i * this->map_size + j]) / 255 * height - 0.39;
            }
            texture[i][j][1] = float(i) / float(this->map_size);
            texture[i][j][0] = float(j) / float(this->map_size);
        }
    }
    int idx = 0;
    for (int i = 0; i < this->map_size; ++i) {
        for (int j = 0; j < this->map_size; ++j) {
            int current_vector = i * (this->map_size + 1) + j;
            index[idx] = current_vector;
            index[idx + 2] = current_vector + this->map_size + 1;
            index[idx + 3] = current_vector + this->map_size + 1;
            index[idx + 1] = current_vector + 1;
            index[idx + 4] = current_vector + 1;
            index[idx + 5] = current_vector + this->map_size + 2;
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

Island::Island() {
    this->shader = ResourceManager::GetShader("island");
    this->picture = ResourceManager::GetTexture("island");
    this->detail[0] = ResourceManager::GetTexture("island_detail0");
    this->detail[1] = ResourceManager::GetTexture("island_detail1");
    this->detail[2] = ResourceManager::GetTexture("island_detail2");
    this->detail[3] = ResourceManager::GetTexture("island_detail3");
    this->detail[4] = ResourceManager::GetTexture("island_detail4");
    this->detail[5] = ResourceManager::GetTexture("island_detail5");
    this->grass[0] = ResourceManager::GetTexture("island_grass0");
    this->grass[1] = ResourceManager::GetTexture("island_grass1");
    this->grass[2] = ResourceManager::GetTexture("island_grass2");
    this->height_map = ResourceManager::GetHeightMap("island_height");
    this->map_size = ResourceManager::GetHeightMapWidth("island_height");
    this->init();
}

Island::~Island() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void Island::step() {}

void Island::draw() {
    this->shader.Use();

    this->shader.SetInteger("sprite", 0);
    this->shader.SetInteger("detail", 1);
    this->shader.SetInteger("grass", 2);

    glEnable(GL_CULL_FACE);
    //this->picture.Bind();
    glEnable(GL_DEPTH_TEST);
    glDepthMask(GL_TRUE);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ALPHA, GL_SRC_COLOR);
    glBindVertexArray(this->VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glActiveTexture(GL_TEXTURE0);
    this->picture.Bind();
    glActiveTexture(GL_TEXTURE1);
    this->detail[this->detail_mode].Bind();
    glActiveTexture(GL_TEXTURE2);
    glCullFace(GL_BACK);
    this->grass[this->grass_mode].Bind();
    this->shader.SetFloat("direction", 1.0f);
    glDrawElements(GL_TRIANGLES, this->map_size * this->map_size * 6, GL_UNSIGNED_INT, nullptr);
    this->picture.Bind();
    glCullFace(GL_FRONT);
    this->shader.SetFloat("direction", -1.0f);
    glDrawElements(GL_TRIANGLES, this->map_size * this->map_size * 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glDisable(GL_BLEND);
    glDisable(GL_CULL_FACE);
    glDisable(GL_TEXTURE0);
    glDisable(GL_TEXTURE1);
}

void Island::load() {
    ResourceManager::LoadShader("static_source/glsl_source/island.vert.glsl",
                                "static_source/glsl_source/island.frag.glsl", nullptr, "island");
    ResourceManager::LoadHeightMap("static_source/images/heightmap.bmp", "island_height");
    ResourceManager::LoadTexture("static_source/images/rock0.bmp", false, GL_REPEAT, "island_detail0");
    ResourceManager::LoadTexture("static_source/images/rock1.jpg", false, GL_REPEAT, "island_detail1");
    ResourceManager::LoadTexture("static_source/images/rock2.jpg", false, GL_REPEAT, "island_detail2");
    ResourceManager::LoadTexture("static_source/images/rock3.jpg", false, GL_REPEAT, "island_detail3");
    ResourceManager::LoadTexture("static_source/images/rock4.jpg", false, GL_REPEAT, "island_detail4");
    ResourceManager::LoadTexture("static_source/images/rock5.jpg", false, GL_REPEAT, "island_detail5");
    ResourceManager::LoadTexture("static_source/images/terrain-texture3.bmp", false, GL_CLAMP_TO_EDGE, "island");
    ResourceManager::LoadTexture("static_source/images/grass0.jpg", false, GL_REPEAT, "island_grass0");
    ResourceManager::LoadTexture("static_source/images/grass1.jpg", false, GL_REPEAT, "island_grass1");
    ResourceManager::LoadTexture("static_source/images/grass2.jpg", false, GL_REPEAT, "island_grass2");
    // Bind camera uniform buffer to Island shader
    unsigned int uniformBlockIndexIsland = glGetUniformBlockIndex(ResourceManager::GetShader("island").ID,
                                                                  "Matrices");
    glUniformBlockBinding(ResourceManager::GetShader("island").ID, uniformBlockIndexIsland, 0);
}

void Island::setMode(int detail, int grass) {
    this->detail_mode = detail;
    this->grass_mode = grass;
}