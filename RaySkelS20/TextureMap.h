# pragma once
#include <FL/gl.h>
#include "../RaySkelS20/src/vecmath/vecmath.h" 
#include <cstring>
#include <iostream>
class TextureMap {
public:
    TextureMap() { }
    ~TextureMap() {}

    bool loadTexture(char*);
    bool TextureMap::loadNormal(char* filename);
    bool LoadMaterialTexture(char* orig_file, unsigned char*& target, int& width, int& height);
    vec3f getSquareColor(const vec3f P);
    vec3f getSphereColor(const vec3f P);
    vec3f getSquareNormal(const vec3f P);
    vec3f getSphereNormal(const vec3f P);

    int texImageWidth = 0;
    int texImageHeight = 0;

    int NorImageWidth = 0;
    int NorImageHeight = 0;


    double gradDirY[3][3] = { {1.0, 1.0, 1.0}, {0.0, 0.0, 0.0}, {-1.0, -1.0, -1.0} };
    double gradDirX[3][3] = { {-1.0, 0.0, 1.0}, {-1.0, 0.0, 1.0}, {-1.0, 0.0, 1.0} };
    std::pair<vec3f, vec3f> calGradient(int u, int v,
        unsigned char* src, int width, int height) {
        vec3f gradX(0.0, 0.0, 0.0);
        vec3f gradY(0.0, 0.0, 0.0);
        for (int i = -1; i < 2; i++)   // row
            for (int j = -1; j < 2; j++) // col
            {
              
                int  index = (v + i) * (width * 3) + ((u + j) * 3);
                for (int k = 0; k < 3; k++) {
            
                    //std::cout << "width_Height" << width << " " << height << std::endl;
                    //std::cout << "index"<<index<<"u,v"<<u<<" "<<v << std::endl;  

                    gradX[k] += src[index + k] * gradDirX[i+1][j+1];
                    gradY[k] += src[index + k] * gradDirY[i+1][j+1];
                }
            }
        return std::pair<vec3f, vec3f>(gradX, gradY);
    }

    vec3f getNormal(int u, int v,
        unsigned char* src, int width, int height) {
        vec3f normal(0.0, 0.0, 0.0);
                int  index = (v ) * (width * 3) + ((u ) * 3);
                for (int k = 0; k < 3; k++) {

                    //std::cout << "width_Height" << width << " " << height << std::endl;
                    //std::cout << "index"<<index<<"u,v"<<u<<" "<<v << std::endl;  

                    normal[k] = src[index + k]/255.0-0.5;
                }
            
        return normal;
    }
public:
    unsigned char* texImage = nullptr;
    unsigned char* norImage = nullptr;
    GLuint texImageId = NULL;
private:
    const double RAY_EPSILON = 0.00001;
    const double NORMAL_EPSILON = 0.00001;
};

