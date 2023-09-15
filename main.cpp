#include "camera.hpp"
#include <optional>
#include <raylib.h>
#include <raymath.h>
#include <stdio.h>
#include <vector>

#define STEP 0.1
#define SENSITIVITY 0.01
#define SCALE 50

static void UpdateDrawFrame(void);

std::vector<Tri> points = {
    {{-5, -5, 2}, {-5, -10, 2}, {5, -5, 2}},
    {{5, -10, 2}, {-5, -5, 5}, {-5, -10, 5}},
};

FoxCamera camera(SCALE);
int main() {
  camera.cameraPosition = {0, 0, 2};
  InitWindow(450, 450, "Window");

  SetTargetFPS(60);

#if defined(PLATFORM_WEB)
  emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    UpdateDrawFrame();
  }
#endif
}

static void UpdateDrawFrame(void) {
  Vector3 nextPos = Vector3Zero();
  if (IsKeyDown(KEY_E)) {
    nextPos.y -= STEP;
  }
  if (IsKeyDown(KEY_Q)) {
    nextPos.y += STEP;
  }
  if (IsKeyDown(KEY_A)) {
    nextPos.x -= STEP;
  }
  if (IsKeyDown(KEY_D)) {
    nextPos.x += STEP;
  }
  if (IsKeyDown(KEY_S)) {
    nextPos.z -= STEP;
  }
  if (IsKeyDown(KEY_W)) {
    nextPos.z += STEP;
  }

  if (IsKeyDown(KEY_LEFT)) {
    camera.cameraTheta.y -= SENSITIVITY;
  }
  if (IsKeyDown(KEY_RIGHT)) {
    camera.cameraTheta.y += SENSITIVITY;
  }

  nextPos = RotateAxis(nextPos, AXIS_Y, -camera.cameraTheta.y);
  camera.cameraPosition = Vector3Add(camera.cameraPosition, nextPos);

  ClearBackground(BLACK);

  DrawFPS(10, 10);

  for (Tri currentPoint : points) {
    camera.DrawTri(currentPoint);
  }

  printf("%f, %f, %f\n", camera.cameraPosition.x, camera.cameraPosition.y,
         camera.cameraPosition.z);

  EndDrawing();
}
