# pragma once
#include <FL/gl.h>
#include "../RaySkelS20/src/vecmath/vecmath.h" 
#include <cstring>
#include <iostream>
class TextureMap {
public:
    TextureMap() { std::cout << "created" << std::endl;}
    ~TextureMap() {}

    bool loadTexture(char*);
    bool LoadMaterialTexture(char* orig_file, unsigned char*& target, int& width, int& height);
    vec3f getSquareColor(const vec3f P);
    vec3f getSphereColor(const vec3f P);

    int texImageWidth = 0;
    int texImageHeight = 0;
public:
    unsigned char* texImage = nullptr;
    GLuint texImageId = NULL;
private:
    const double RAY_EPSILON = 0.00001;
    const double NORMAL_EPSILON = 0.00001;
};

