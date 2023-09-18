#pragma once
#include <cstdlib>
#include <math.h>
#include <optional>
#include <raylib.h>
#include <stdio.h>

typedef enum Axis { AXIS_X, AXIS_Y, AXIS_Z } Axis;

Vector3 RotateAxis(Vector3 point, Axis axis, float angle);

class Tri {
public:
  Vector3 p1;
  Vector3 p2;
  Vector3 p3;

  Tri(Vector3 cP1, Vector3 cP2, Vector3 cP3) {
    p1 = cP1;
    p2 = cP2;
    p3 = cP3;
  }
  std::optional<Vector3> GetPointWeights(Vector2 point, bool quickExit);
  bool PointIn(Vector2 point);
  Vector3 InterpolatePoint(Vector2 point, Vector3 weights);
};

class FoxCamera {
private:
  Image copyImage;
  Texture textureBuffer;

  float *depthBuffer;

public:
  FoxCamera(float CScale, int CWidth, int CHeight) {
    scale = CScale;
    width = CWidth;
    height = CHeight;
    copyImage = GenImageColor(width, height, WHITE);
    textureBuffer = LoadTextureFromImage(copyImage);

    depthBuffer = (float *)std::malloc(sizeof(float) * width * height);
  }
  void Draw() {
    for (int x = 0; x < width; x++) {
      for (int y = 0; y < height; y++) {
        if (depthBuffer[x + y * width] > 0) {
          ImageDrawPixel(
              &copyImage, x, y,
              ColorFromHSV(0, 1, fmin(depthBuffer[x + y * width] / 40, 0.8)));
          depthBuffer[x + y * width] = 0;
        }
      }
    }

    UpdateTexture(textureBuffer, copyImage.data);
    DrawTexture(textureBuffer, 0, 0, WHITE);
    ImageClearBackground(&copyImage, BLACK);
  }
  Vector3 cameraPosition;
  Vector3 cameraTheta;
  float scale;
  int width, height;

  std::optional<Vector3> TransformPoint(Vector3 point);
  void DrawTri(Tri tri);
};
