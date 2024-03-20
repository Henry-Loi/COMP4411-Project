#include "metaballs.h"

#include <FL/gl.h>
#include <cmath>
#include <cstdlib>

/* reference: https://paulbourke.net/geometry/polygonise/ */

/*
   Given a grid cell and an isolevel, calculate the triangular
   facets required to represent the isosurface through the cell.
   Return the number of triangular facets, the array "triangles"
   will be loaded up with the vertices at most 5 triangular facets.
        0 will be returned if the grid cell is either totally above
   of totally below the isolevel.
*/
int Polygonise(GRIDCELL grid, double isolevel, TRIANGLE *triangles) {
  int i, ntriang;
  int cubeindex;
  Vec3f vertlist[12];

  /*
        Determine the index into the edge table which
        tells us which vertices are inside of the surface
     */
  cubeindex = 0;
  if (grid.val[0] < isolevel)
    cubeindex |= 1;
  if (grid.val[1] < isolevel)
    cubeindex |= 2;
  if (grid.val[2] < isolevel)
    cubeindex |= 4;
  if (grid.val[3] < isolevel)
    cubeindex |= 8;
  if (grid.val[4] < isolevel)
    cubeindex |= 16;
  if (grid.val[5] < isolevel)
    cubeindex |= 32;
  if (grid.val[6] < isolevel)
    cubeindex |= 64;
  if (grid.val[7] < isolevel)
    cubeindex |= 128;

  /* Cube is entirely in/out of the surface */
  if (edgeTable[cubeindex] == 0)
    return (0);

  /* Find the vertices where the surface intersects the cube */
  if (edgeTable[cubeindex] & 1)
    vertlist[0] =
        VertexInterp(isolevel, grid.p[0], grid.p[1], grid.val[0], grid.val[1]);
  if (edgeTable[cubeindex] & 2)
    vertlist[1] =
        VertexInterp(isolevel, grid.p[1], grid.p[2], grid.val[1], grid.val[2]);
  if (edgeTable[cubeindex] & 4)
    vertlist[2] =
        VertexInterp(isolevel, grid.p[2], grid.p[3], grid.val[2], grid.val[3]);
  if (edgeTable[cubeindex] & 8)
    vertlist[3] =
        VertexInterp(isolevel, grid.p[3], grid.p[0], grid.val[3], grid.val[0]);
  if (edgeTable[cubeindex] & 16)
    vertlist[4] =
        VertexInterp(isolevel, grid.p[4], grid.p[5], grid.val[4], grid.val[5]);
  if (edgeTable[cubeindex] & 32)
    vertlist[5] =
        VertexInterp(isolevel, grid.p[5], grid.p[6], grid.val[5], grid.val[6]);
  if (edgeTable[cubeindex] & 64)
    vertlist[6] =
        VertexInterp(isolevel, grid.p[6], grid.p[7], grid.val[6], grid.val[7]);
  if (edgeTable[cubeindex] & 128)
    vertlist[7] =
        VertexInterp(isolevel, grid.p[7], grid.p[4], grid.val[7], grid.val[4]);
  if (edgeTable[cubeindex] & 256)
    vertlist[8] =
        VertexInterp(isolevel, grid.p[0], grid.p[4], grid.val[0], grid.val[4]);
  if (edgeTable[cubeindex] & 512)
    vertlist[9] =
        VertexInterp(isolevel, grid.p[1], grid.p[5], grid.val[1], grid.val[5]);
  if (edgeTable[cubeindex] & 1024)
    vertlist[10] =
        VertexInterp(isolevel, grid.p[2], grid.p[6], grid.val[2], grid.val[6]);
  if (edgeTable[cubeindex] & 2048)
    vertlist[11] =
        VertexInterp(isolevel, grid.p[3], grid.p[7], grid.val[3], grid.val[7]);

  /* Create the triangle */
  ntriang = 0;
  for (i = 0; triTable[cubeindex][i] != -1; i += 3) {
    triangles[ntriang].p[0] = vertlist[triTable[cubeindex][i]];
    triangles[ntriang].p[1] = vertlist[triTable[cubeindex][i + 1]];
    triangles[ntriang].p[2] = vertlist[triTable[cubeindex][i + 2]];
    ntriang++;
  }

  return (ntriang);
}

/*
   Linearly interpolate the position where an isosurface cuts
   an edge between two vertices, each with their own scalar value
*/

Vec3f VertexInterp(double isolevel, Vec3f p1, Vec3f p2, double valp1,
                   double valp2) {
  double mu;

  Vec3f p;

  if (abs(isolevel - valp1) < 0.00001)
    return (p1);
  if (abs(isolevel - valp2) < 0.00001)
    return (p2);
  if (abs(valp1 - valp2) < 0.00001)
    return (p1);
  mu = (isolevel - valp1) / (valp2 - valp1);

  for (int i = 0; i < 3; i++) {
    p[i] = p1[i] + mu * (p2[i] - p1[i]);
  }

  return (p);
}

void Metaball::draw() {
  glBegin(GL_TRIANGLES);
  {
    for (int k = 0; k < numGrids; k++) {
      int ntriang = Polygonise(grid[k], threshold, triangles);

      for (int i = 0; i < ntriang; i++) {
        for (int j = 0; j < 3; j++) {
          glVertex3f(triangles[i].p[j][0], triangles[i].p[j][1],
                     triangles[i].p[j][2]);
        }
      }
    }
  }
  glEnd();
}

Metaball::Metaball(int gridSize) {
  int currentVertex = 0;
  // using grid size = 20 in 10 x 10 cube
  float dimension = 10.0f;
  float offset = 5.0f;

  triangles = new TRIANGLE[256];

  grid = new GRIDCELL[gridSize * gridSize * gridSize];

  // CUBES
  numGrids = (gridSize) * (gridSize) * (gridSize);

  int currentCube = 0;

  for (int i = 0; i < gridSize; i++) {
    for (int j = 0; j < gridSize; j++) {
      for (int k = 0; k < gridSize; k++) {
        grid[currentCube].p[0] = Vec3f((i * dimension) / (gridSize)-offset,
                                       (j * dimension) / (gridSize)-offset,
                                       (k * dimension) / (gridSize)-offset);
        grid[currentCube].val[0] = 0.0f;

        grid[currentCube].p[1] =
            Vec3f((i * dimension) / (gridSize)-offset,
                  (j * dimension) / (gridSize)-offset,
                  ((k + 1) * dimension) / (gridSize)-offset);
        grid[currentCube].val[1] = 0.0f;

        grid[currentCube].p[2] =
            Vec3f((i * dimension) / (gridSize)-offset,
                  ((j + 1) * dimension) / (gridSize)-offset,
                  ((k + 1) * dimension) / (gridSize)-offset);
        grid[currentCube].val[2] = 0.0f;

        grid[currentCube].p[3] =
            Vec3f((i * dimension) / (gridSize)-offset,
                  ((j + 1) * dimension) / (gridSize)-offset,
                  (k * dimension) / (gridSize)-offset);
        grid[currentCube].val[3] = 0.0f;

        grid[currentCube].p[4] =
            Vec3f(((i + 1) * dimension) / (gridSize)-offset,
                  (j * dimension) / (gridSize)-offset,
                  (k * dimension) / (gridSize)-offset);
        grid[currentCube].val[4] = 0.0f;

        grid[currentCube].p[5] =
            Vec3f(((i + 1) * dimension) / (gridSize)-offset,
                  (j * dimension) / (gridSize)-offset,
                  ((k + 1) * dimension) / (gridSize)-offset);
        grid[currentCube].val[5] = 0.0f;

        grid[currentCube].p[6] =
            Vec3f(((i + 1) * dimension) / (gridSize)-offset,
                  ((j + 1) * dimension) / (gridSize)-offset,
                  ((k + 1) * dimension) / (gridSize)-offset);
        grid[currentCube].val[6] = 0.0f;

        grid[currentCube].p[7] =
            Vec3f(((i + 1) * dimension) / (gridSize)-offset,
                  ((j + 1) * dimension) / (gridSize)-offset,
                  (k * dimension) / (gridSize)-offset);
        grid[currentCube].val[7] = 0.0f;

        currentCube++;
      }
    }
  }

  // TRIANGLES
  for (int i = 0; i < 256; i++) {
    for (int j = 0; j < 3; j++) {
      triangles[i].p[j] = Vec3f(0.0f, 0.0f, 0.0f);
    }
  }
}
