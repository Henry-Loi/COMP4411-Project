#ifndef __READ_H__
#define __READ_H__

#include <iostream>
#include <string>

#include "../scene/scene.h"

Scene *readScene(const string &filename);
Scene *readScene(istream &is);

Scene *loadHeightField(char *iname);

#endif // __READ_H__
