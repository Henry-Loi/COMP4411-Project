// modelerdraw.h

// Contains object and routines related to rendering things

#ifndef MODELERDRAW_H
#define MODELERDRAW_H

#include <FL/gl.h>
#include <cstdio>

enum DrawModeSetting_t {
  NONE = 0,
  NORMAL,
  WIREFRAME,
  FLATSHADE,
};

enum QualitySetting_t {
  HIGH,
  MEDIUM,
  LOW,
  POOR,
};

// Ignore this; the ModelerDrawState just keeps
// information about the current color, etc, etc.
class ModelerDrawState {
public:
  static ModelerDrawState *Instance();

  FILE *m_rayFile;

  DrawModeSetting_t m_drawMode;
  QualitySetting_t m_quality;

  GLfloat m_ambientColor[4];
  GLfloat m_diffuseColor[4];
  GLfloat m_specularColor[4];
  GLfloat m_shininess;

private:
  ModelerDrawState();
  ModelerDrawState(const ModelerDrawState &) {}
  ModelerDrawState &operator=(const ModelerDrawState &) {}

  static ModelerDrawState *m_instance;
};

// ****************************************************************************
// DRAWING FUNCTIONS
//
// The following functions are for your use in Modeler.  You don't *need* to
// use these functions; however, if you desire to output your model for
// the raytracer project (in .ray file format), you must either call these
// functions or implement the appropriate functionality so that the raytracer
// can handle it.
//
// Note:  Depending on whether a ray file is open or closed, these functions
//        will either output to a ray file or make OpenGL calls.
// ****************************************************************************

// Set the current material properties
void setAmbientColor(float r, float g, float b);
void setDiffuseColor(float r, float g, float b);
void setSpecularColor(float r, float g, float b);
void setShininess(float s);

// Set the current draw mode (see DrawModeSetting_t for valid values
void setDrawMode(DrawModeSetting_t drawMode);

// Set the current quality mode (See QualityModeSetting_t for valid values
void setQuality(QualitySetting_t quality);

// Opens a .ray file for writing, returns false on error
bool openRayFile(const char rayFileName[]);
// Closes the current .ray file if one exists
void closeRayFile();

/////////////////////////////
// Raytraceable Primitives //
/////////////////////////////

// Draw a sphere of radius r
void drawSphere(double r);

// Draw an axis-aligned box from origin to (x,y,z)
void drawBox(double x, double y, double z);

// Draw an axis-aligned texture box from origin to (x,y,z)
void drawTextureBox(double x, double y, double z);

// Draw a cylinder from z=0 to z=h with radius r1 at origin and r2 at z=h
void drawCylinder(double h, double r1, double r2);

// Driangle with three given vertices.  Specify in counterclockwise direction
void drawTriangle(double x1, double y1, double z1, double x2, double y2,
                  double z2, double x3, double y3, double z3);

void drawTorus(double, double);

void drawMetaBall(void);

#include <string>
class LSystem {
public:
  LSystem() : angle(60), distance(1), radius(0.1f){};
  LSystem(float angle, float distance, float radius = 0.1f)
      : angle(angle), distance(distance), radius(radius){};
  ~LSystem() = default;

  void draw_system(int generations);

  std::string sys_string;

  float angle;
  float distance;
  float radius;
};

class Vector3 {
public:
  float x, y, z;

  // Default constructor
  Vector3() : x(0.0f), y(0.0f), z(0.0f) {}

  // Constructor with initial values
  Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

  // Copy constructor
  Vector3(const Vector3 &other) : x(other.x), y(other.y), z(other.z) {}

  // Destructor
  ~Vector3() {}

  // Addition operator
  Vector3 operator+(const Vector3 &other) const {
    return Vector3(x + other.x, y + other.y, z + other.z);
  }

  // Subtraction operator
  Vector3 operator-(const Vector3 &other) const {
    return Vector3(x - other.x, y - other.y, z - other.z);
  }

  // Scalar multiplication operator
  Vector3 operator*(float scalar) const {
    return Vector3(x * scalar, y * scalar, z * scalar);
  }

  // Dot product
  float dot(const Vector3 &other) const {
    return x * other.x + y * other.y + z * other.z;
  }

  // Cross product
  Vector3 cross(const Vector3 &other) const {
    return Vector3(y * other.z - z * other.y, z * other.x - x * other.z,
                   x * other.y - y * other.x);
  }

  // Normalize the vector
  void normalize() {
    float length = sqrt(x * x + y * y + z * z);
    if (length > 0.0f) {
      x /= length;
      y /= length;
      z /= length;
    }
  }
};

void drawLSystemTree(float angle, float distance, float radius);
void drawComplexShape();
void setDiffuseColorwithAlpha(float r, float g, float b, float opacity);
#endif