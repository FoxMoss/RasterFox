#pragma once
#include <optional>
#include <raylib.h>

typedef enum Axis { AXIS_X, AXIS_Y, AXIS_Z } Axis;

Vector3 RotateAxis(Vector3 point, Axis axis, float angle);

typedef struct Tri {
  Vector3 p1;
  Vector3 p2;
  Vector3 p3;
} Tri;

class FoxCamera {
public:
  FoxCamera(float CScale) { scale = CScale; }
  Vector3 cameraPosition;
  Vector3 cameraTheta;
  float scale;

  std::optional<Vector3> TransformPoint(Vector3 point);
  void DrawTri(Tri tri);
};
