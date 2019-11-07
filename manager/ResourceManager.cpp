/*******************************************************************
** This code is part of Breakout.
**
** Breakout is free software: you can redistribute it and/or modify
** it under the terms of the CC BY 4.0 license as published by
** Creative Commons, either version 4 of the License, or (at your
** option) any later version.
******************************************************************/
#include "ResourceManager.h"

#include <iostream>
#include <sstream>

// Instantiate static variables
std::map<std::string, Texture2D>    ResourceManager::Textures;
std::map<std::string, Shader>       ResourceManager::Shaders;


Shader ResourceManager::LoadShader(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile,
                                   const std::string& name) {
    Shaders[name] = loadShaderFromFile(vShaderFile, fShaderFile, gShaderFile);
    return Shaders[name];
}

Shader ResourceManager::GetShader(const std::string &name) {
    return Shaders[name];
}

Texture2D ResourceManager::LoadTexture(const GLchar *file, GLboolean alpha, GLuint wrap, const std::string& name) {
    Textures[name] = loadTextureFromFile(file, alpha, wrap);
    return Textures[name];
}

Texture2D ResourceManager::GetTexture(const std::string &name) {
    return Textures[name];
}

void ResourceManager::Clear() {
    // (Properly) delete all shaders
    for (const auto &iter : Shaders)
        glDeleteProgram(iter.second.ID);
    // (Properly) delete all textures
    for (const auto &iter : Textures)
        glDeleteTextures(1, &iter.second.ID);
}

Shader
ResourceManager::loadShaderFromFile(const GLchar *vShaderFile, const GLchar *fShaderFile, const GLchar *gShaderFile) {
    // 1. Retrieve the vertex/fragment source code from filePath
    Shader shader = Shader();
    shader.Compile(vShaderFile, fShaderFile, gShaderFile);
    return shader;
}

Texture2D ResourceManager::loadTextureFromFile(const GLchar *file, GLboolean alpha, GLuint wrap) {
    // Create Texture object
    Texture2D texture = Texture2D();
    if (alpha) {
        texture.Internal_Format = GL_RGBA;
        texture.Image_Format = GL_RGBA;
    }
    texture.Wrap_S = wrap;
    texture.Wrap_T = wrap;
    // Load image
    int width, height, nrChannels;
    unsigned char *image = nullptr;
    image = stbi_load(file, &width, &height, &nrChannels, alpha ? STBI_rgb_alpha : STBI_rgb);
    texture.Generate(width, height, image);
    // And finally free image data
    stbi_image_free(image);
    return texture;
}
