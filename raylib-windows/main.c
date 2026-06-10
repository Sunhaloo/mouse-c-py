#include <stdio.h>
#include <raylib.h>

// NOTE: following 'Coding with Sphere' boilerplate
int main(void) {
  // initialise a simple window
  InitWindow(800, 600, "My Raylib Window");

  // allow the user to see and close the window created
  while (!WindowShouldClose()) {
    BeginDrawing();

    ClearBackground(BLACK);

    EndDrawing();
  }

  // close the created window
  CloseWindow();

  return 0;
}
