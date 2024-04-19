#include "TextureMap.h"
#include "../RaySkelS20/src/fileio/bitmap.h"
#include <cmath>
#include "../RaySkelS20//src/vecmath/vecmath.h"

#include<iostream>
#include <FL/fl_ask.H>


bool TextureMap::loadTexture(char* filename) {
    // Load the texture file
    cout << filename << endl;

    // Seek to the beginning of the file and read the header (assuming BMP format)
    texImage = readBMP(filename, texImageWidth, texImageHeight);
    std::cout << "WIDTH_HEIGHT" << texImageWidth << " " << texImageHeight << std::endl;
    if (texImage== NULL) {
        // pop alert
        fl_alert("Error reading texture file");
        return false;
    }
    std::cout << texImage << std::endl;
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
    std::cout << "POS" << P << "X:" << X_pos<<"Y:"<<Y_pos << std::endl;
    if (texImage != NULL) {
        int index = Y_pos * (texImageWidth * 3) + (X_pos * 3);
        return vec3f(double(texImage[index])/255.0, double(texImage[index + 1])/255.0, double(texImage[index + 2])/255.0);
    }
    else
        return vec3f(1.0, 1.0, 1.0);
}