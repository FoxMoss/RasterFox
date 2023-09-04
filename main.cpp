#include <raylib.h>
#include <raymath.h>

static void UpdateDrawFrame(void);

int main() {
  InitWindow(450, 450, "FoxMoss");

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
  ClearBackground(RED);

  DrawFPS(10, 10);

  EndDrawing();
}
