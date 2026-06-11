// include the actual raylib "functions", etc
#include "include/raylib.h"

// include the actual raygui "functions", etc
#define RAYGUI_IMPLEMENTATION
// #include "include/raygui.h"

// define our window size
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400

// our main function
int main(void) {
  // title for our window
  char *windowTitle = "mouse-c-py";

  // initialise the main window rendered out by Windows
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, windowTitle);

  // set the target to 30
  SetTargetFPS(30);

  while (!WindowShouldClose()) {
    // start drawing stuff inside the window created
    BeginDrawing();

    // change the background colour 'white' colour
    ClearBackground(WHITE);



    // drawing finishes ==> end the drawing
    EndDrawing();
  }

  // close the window created
  CloseWindow();

  return 0;
}
