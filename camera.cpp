#include "camera.hpp"
#include <cmath>
#include <cstdlib>
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

// https://en.wikipedia.org/wiki/3D_projection#Perspective_projection

float manageZ(float x) {
  if (x < 0) {
    return 0;
  }
  return x;
}
std::optional<Vector3> FoxCamera::TransformPoint(Vector3 point) {
  Vector3 result = Vector3Zero();
  Vector3 projectionPlane = {0, 0, 5};

  Vector3 computedPoint = Vector3Subtract(point, cameraPosition);

  computedPoint = RotateAxis(computedPoint, AXIS_X, cameraTheta.x);
  computedPoint = RotateAxis(computedPoint, AXIS_Y, cameraTheta.y);
  computedPoint = RotateAxis(computedPoint, AXIS_Z, cameraTheta.z);

  result.x = manageZ(projectionPlane.z / computedPoint.z) * computedPoint.x +
             projectionPlane.x;
  result.y = manageZ(projectionPlane.z / computedPoint.z) * computedPoint.y +
             projectionPlane.y;
  result.z = projectionPlane.z / computedPoint.z;

  return result;
}

void FoxCamera::DrawTri(Tri tri) {
  std::optional<Vector3> transformedPoint1Obj = TransformPoint(tri.p1);
  Vector3 transformedPoint1 = transformedPoint1Obj.value();
  Vector3 p1 = {width / 2 + transformedPoint1.x * scale,
                height / 2 + transformedPoint1.y * scale, transformedPoint1.z};

  std::optional<Vector3> transformedPoint2Obj = TransformPoint(tri.p2);
  Vector3 transformedPoint2 = transformedPoint2Obj.value();
  Vector3 p2 = {width / 2 + transformedPoint2.x * scale,
                height / 2 + transformedPoint2.y * scale, transformedPoint2.z};

  std::optional<Vector3> transformedPoint3Obj = TransformPoint(tri.p3);
  Vector3 transformedPoint3 = transformedPoint3Obj.value();
  Vector3 p3 = {width / 2 + transformedPoint3.x * scale,
                height / 2 + transformedPoint3.y * scale, transformedPoint3.z};

  if (transformedPoint1.z < 0 || transformedPoint2.z < 0 ||
      transformedPoint3.z < 0) {
    return;
  }

  Vector2 firstPoint = {(float)width, (float)height};
  Vector2 lastPoint = {0, 0};

  firstPoint.x = fmin(p1.x, fmin(p2.x, fmin(p3.x, width)));
  firstPoint.y = fmin(p1.y, fmin(p2.y, fmin(p3.y, width)));

  lastPoint.x = fmax(p1.x, fmax(p2.x, fmax(p3.x, 0)));
  lastPoint.y = fmax(p1.y, fmax(p2.y, fmax(p3.y, 0)));

  firstPoint.x = Clamp(ceil(firstPoint.x), 0, width);
  firstPoint.y = Clamp(ceil(firstPoint.y), 0, height);
  lastPoint.x = Clamp(ceil(lastPoint.x), 0, width);
  lastPoint.y = Clamp(ceil(lastPoint.y), 0, height);
  // printf("%f, %f : %f, %f\n", firstPoint.x, firstPoint.y, lastPoint.x,
  //  lastPoint.y);

  Tri computedTri(p1, p2, p3);
  for (int x = firstPoint.x; x < lastPoint.x; x++) {
    for (int y = firstPoint.y; y < lastPoint.y; y++) {

      auto values = computedTri.GetPointWeights(Vector2{(float)x, (float)y});
      if (!values.has_value() ||
          (values->x < 0 || values->y < 0 || values->z < 0)) {
        continue;
      }
      float depth =
          computedTri
              .InterpolatePoint(Vector2{(float)x, (float)y}, values.value())
              .z;
      if (depthBuffer[x + y * width] < depth) {
        depthBuffer[x + y * width] = depth;
      }
    }
  }
}

Vector3 Tri::InterpolatePoint(Vector2 point, Vector3 weights) {

  Vector3 ret;
  ret.x = weights.x * p1.x + weights.y * p2.x + weights.z * p3.x;
  ret.y = weights.x * p1.y + weights.y * p2.y + weights.z * p3.y;
  ret.z = weights.x * p1.z + weights.y * p2.z + weights.z * p3.z;
  return ret;
}

std::optional<Vector3> Tri::GetPointWeights(Vector2 point) {
  float w1 =
      ((p2.y - p3.y) * (point.x - p3.x) + (p3.x - p2.x) * (point.y - p3.y)) /
      ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));

  float w2 =
      ((p3.y - p1.y) * (point.x - p3.x) + (p1.x - p3.x) * (point.y - p3.y)) /
      ((p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y));
  float w3 = 1 - w1 - w2;

  return Vector3{w1, w2, w3};
}

Vector3 RotateAxis(Vector3 point, Axis axis, float angle) {

  float axisA = 0;
  float axisB = 0;

  switch (axis) {
  case AXIS_X:
    axisA = point.y;
    axisB = point.z;
    break;
  case AXIS_Y:
    axisA = point.x;
    axisB = point.z;
    break;
  case AXIS_Z:
    axisA = point.x;
    axisB = point.y;
    break;
  }

  float resA = axisA * cos(angle) - axisB * sin(angle);
  float resB = axisA * sin(angle) + axisB * cos(angle);

  Vector3 res = point;
  switch (axis) {
  case AXIS_X:
    res.y = resA;
    res.z = resB;
    break;
  case AXIS_Y:
    res.x = resA;
    res.z = resB;
    break;
  case AXIS_Z:
    res.x = resA;
    res.y = resB;
    break;
  }
  return res;
}
