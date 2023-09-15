#include "camera.hpp"
#include <cmath>
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
  Vector3 projectionPlane = {0, 0, 2};

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
  Vector2 p1 = {225 + transformedPoint1.x * scale,
                225 + transformedPoint1.y * scale};

  std::optional<Vector3> transformedPoint2Obj = TransformPoint(tri.p2);
  Vector3 transformedPoint2 = transformedPoint2Obj.value();
  Vector2 p2 = {225 + transformedPoint2.x * scale,
                225 + transformedPoint2.y * scale};

  std::optional<Vector3> transformedPoint3Obj = TransformPoint(tri.p3);
  Vector3 transformedPoint3 = transformedPoint3Obj.value();
  Vector2 p3 = {225 + transformedPoint3.x * scale,
                225 + transformedPoint3.y * scale};

  if (transformedPoint1.z < 0 || transformedPoint2.z < 0 ||
      transformedPoint3.z < 0) {
    return;
  }
  DrawLineV(p3, p1, RED);
  DrawLineV(p1, p2, RED);
  DrawLineV(p3, p2, RED);
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
