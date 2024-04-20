#include "TextureMap.h"
#include "../RaySkelS20/src/fileio/bitmap.h"
#include <cmath>
#include "../RaySkelS20//src/vecmath/vecmath.h"

#include<iostream>
#include <FL/fl_ask.H>

//Do both texture and Map Bumpping
// Bumpr reference: chrome-extension://efaidnbmnnnibpcajpcglclefindmkaj/https://web.cse.ohio-state.edu/~shen.94/781/Site/Slides_files/bump.pdf

bool TextureMap::loadTexture(char* filename) {
    // Load the texture file
    cout << "Texture file: "<< filename << endl;

    // Seek to the beginning of the file and read the header (assuming BMP format)
    texImage = readBMP(filename, texImageWidth, texImageHeight);
    std::cout << "WIDTH_HEIGHT" << texImageWidth << " " << texImageHeight << std::endl;
    if (texImage== NULL) {
        // pop alert
        fl_alert("Error reading texture file");
        return false;
    }
    return true;
}

bool TextureMap::loadNormal(char* filename) {
    // Load the texture file
    if (texImage == NULL) {
        // pop alert
        fl_alert("Error load texture file first");
        return false;
    }
    cout <<"Normal file: "<< filename << endl;

    // Seek to the beginning of the file and read the header (assuming BMP format)
    norImage = readBMP(filename, NorImageWidth, NorImageHeight);
    std::cout << "WIDTH_HEIGHT" << NorImageWidth << " " << NorImageHeight << std::endl;
    if (norImage == NULL) {
        // pop alert
        fl_alert("Error reading texture file");
        return false;
    }
    return true;
}


bool TextureMap::LoadMaterialTexture(char* orig_file, unsigned char*& target, int&width,int&height) {
    (target) = readBMP(orig_file, width, height);

    if ((target) == NULL) {
        // pop alert
        fl_alert("Error reading texture file");
        return false;
    }
    return true;
}

vec3f TextureMap::getSquareColor(const vec3f P) {
   
    int X_pos = (P[0]+0.5)* (texImageWidth-1);
    int Y_pos = (P[1]+0.5)* (texImageHeight-1);

    if (texImage != NULL) {
        int index = Y_pos * (texImageWidth * 3) + (X_pos * 3);
        return vec3f(double(texImage[index])/255.0, double(texImage[index + 1])/255.0, double(texImage[index + 2])/255.0);
    }
    else
        return vec3f(1.0, 1.0, 1.0);
}

vec3f TextureMap::getSphereColor(const vec3f P) {
    vec3f Pos(P[2], P[0], P[1]);
    vec3f S_p(0.0, 0.0, 1.0);
    vec3f S_e(1.0, 0.0, 0.0);
    double phi = acos(-Pos.dot(S_p));
    double v = phi / 3.142;
    double sita = acos(S_e.dot(Pos) / sin(phi)) / (2 * 3.142);
    double u = sita;

    if ((S_p.cross(S_e)).dot(Pos) < 0)
        u = 1 - sita;
    if (abs(v - 1.0) < 0.001|| abs(v) < 0.001|| abs(v + 1.0) < 0.001)
        u = 0.0;
    if (std::isnan(u))
        u = 0.0;
    int X_pos = u* (texImageWidth - 1);
    int Y_pos = v* (texImageHeight - 1);

    if (texImage != NULL) {
        int index = Y_pos * (texImageWidth * 3) + (X_pos * 3);
        return vec3f(double(texImage[index]) / 255.0, double(texImage[index + 1]) / 255.0, double(texImage[index + 2]) / 255.0);

    }
    else
        return vec3f(1.0, 1.0, 1.0);
}

double CalGradientSquare(double input, int max) {
    return (((input + 1) / (double(max) - 1)) - ((input - 1) / (double(max) - 1) )) / 2;

}
vec3f TextureMap::getSquareNormal(const vec3f P) {

    int X_pos = (P[0]) * double(NorImageWidth - 1);
    int Y_pos = (P[1]) * double(NorImageHeight - 1);


    vec3f U_grad = vec3f(1.0/NorImageWidth,0.0,0.0);
    vec3f V_grad = vec3f(0.0,1.0/NorImageHeight, 0.0);

    vec3f N = U_grad.cross(V_grad);

    if (norImage != NULL) {
        return getNormal(X_pos, Y_pos, norImage, NorImageWidth, NorImageHeight);
    }
    else
        return N.normalize();
}


vec3f TextureMap::getSphereNormal(const vec3f P) {
    vec3f Pos(P[2], P[0], P[1]);
    //vec3f Pos(P[0], P[1], P[2]);
    vec3f S_p(0.0, 0.0, 1.0);
    vec3f S_e(1.0, 0.0, 0.0);
    double phi = acos(-Pos.dot(S_p));
    double v = phi / 3.142;
    double sita = acos(S_e.dot(Pos) / sin(phi)) / (2 * 3.142);
    double u = sita;
    if ((S_p.cross(S_e)).dot(Pos) < 0)
        u = 1 - sita;
    if (abs(v - 1.0) < 0.001 || abs(v) < 0.001)
        u = 0.0;
    int X_pos = u * (NorImageWidth - 1); //u
    int Y_pos = v * (NorImageHeight - 1); //v
    if (norImage != NULL) {
        vec3f result(0.0, 0.0, 0.0);
        result = getNormal(X_pos, Y_pos, norImage, NorImageWidth, NorImageHeight);
        return vec3f(result[1],result[2],result[0]);
        //return result;
    }
    else
        return vec3f(1.0, 1.0, 1.0);
}