//
// Created by 柳子澍 on 2019/11/6.
//

#include "Island.h"

void Island::init() {}

Island::Island(Shader shader) {
    this->shader = shader;
}

Island::~Island() {
    glDeleteVertexArrays(1, &this->VAO);
    glDeleteBuffers(1, &this->VBO);
    glDeleteBuffers(1, &this->EBO);
}

void Island::step() {}

void Island::draw() {}