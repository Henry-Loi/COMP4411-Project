#include <cmath>
#include <vector>

#define PI 3.14159265

#define DEG2RAD(angle) ((angle)*PI / 180.0)
#define RAD2DEG(angle) ((angle)*180.0 / PI)

#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))

#define fmod(a, b) ((a) - (b)*floor((a) / (b)))

static std::vector<std::vector<float>> gaussKernel(int size, float sigma) {
  std::vector<std::vector<float>> kernel;
  float sum = 0.0;
  for (int x = -size / 2; x <= size / 2; x++) {
    std::vector<float> row;
    for (int y = -size / 2; y <= size / 2; y++) {
      float r = sqrt(x * x + y * y);
      float g = exp(-(r * r) / (2 * sigma * sigma)) / (2 * PI * sigma * sigma);
      sum += g;
      row.push_back(g);
    }
    kernel.push_back(row);
  }
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      kernel[i][j] /= sum;
    }
  }
  return kernel;
}

static float calDifference(unsigned char *a, unsigned char *b) {
  int deltaR = (int)a[0] - b[0], deltaG = (int)a[1] - b[1],
      deltaB = (int)a[2] - b[2];
  return std::sqrt(deltaR * deltaR + deltaG * deltaG + deltaB * deltaB);
}

static void RGBtoHSV(const unsigned char *rgb, float *hsv) {
  float r = rgb[0] / 255.0;
  float g = rgb[1] / 255.0;
  float b = rgb[2] / 255.0;
  float max_val = max(r, max(g, b));
  float min_val = min(r, min(g, b));
  float delta = max_val - min_val;
  hsv[2] = max_val;
  if (max_val != 0) {
    hsv[1] = delta / max_val;
  } else {
    hsv[1] = 0;
    hsv[0] = -1;
    return;
  }
  if (r == max_val) {
    hsv[0] = (g - b) / delta;
  } else if (g == max_val) {
    hsv[0] = 2 + (b - r) / delta;
  } else {
    hsv[0] = 4 + (r - g) / delta;
  }
  hsv[0] *= 60;
  if (hsv[0] < 0) {
    hsv[0] += 360;
  }
}

static void HSVtoRGB(const float *hsv, unsigned char *rgb) {
  float c = hsv[2] * hsv[1];
  float x = c * (1 - fabs(fmod(hsv[0] / 60, 2) - 1));
  float m = hsv[2] - c;
  float r, g, b;
  if (hsv[0] >= 0 && hsv[0] < 60) {
    r = c;
    g = x;
    b = 0;
  } else if (hsv[0] >= 60 && hsv[0] < 120) {
    r = x;
    g = c;
    b = 0;
  } else if (hsv[0] >= 120 && hsv[0] < 180) {
    r = 0;
    g = c;
    b = x;
  } else if (hsv[0] >= 180 && hsv[0] < 240) {
    r = 0;
    g = x;
    b = c;
  } else if (hsv[0] >= 240 && hsv[0] < 300) {
    r = x;
    g = 0;
    b = c;
  } else {
    r = c;
    g = 0;
    b = x;
  }
  rgb[0] = (r + m) * 255;
  rgb[1] = (g + m) * 255;
  rgb[2] = (b + m) * 255;
}

static float rgb2Grey(unsigned char *src) {
  return 0.30f * src[0] + 0.59f * src[1] + 0.11f * src[2];
}
