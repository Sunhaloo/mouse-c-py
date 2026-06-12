#include "include/raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// NOTE: following 'Coding with Sphere' boilerplate
int main(void) {
  // initialise a simple window
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "mouse-c-py");

  // set the target FPS for the window
  SetTargetFPS(60);

  // use a custom font
  Font fnt_Rubik = LoadFont("Rubik-Regular.ttf");

  // check if the font is valid and is present
  if (IsFontValid(fnt_Rubik)) {
    TraceLog(LOG_ERROR, "Rubik font is invalid!");
  }

  char *aboveCenteredText = "Above Centered Text";
  char *centeredText = "Centered Text";
  // use the following to get the default Raylib font size
  // int fontSize = GetFontDefault().baseSize;
  int fontSize = 32;

  // measure the text width
  int textWidth = MeasureText(centeredText, fontSize);

  // font position
  Vector2 rubikFontTextPos = (Vector2) {0, 0};

  int fileSize;
  // WARNING: need to unload the font
  unsigned char *fontData = LoadFileData("C:\\Windows\\Fonts\\arial.ttf", &fileSize);
  Font fnt_Arial = LoadFontFromMemory(".ttf", fontData, fileSize, 64, 0, 250);
  Vector2 arialFontTextPos = (Vector2) {0, 50};

  float circleRadius = 50.0;

  Rectangle myBox = { 200, 150, 300, 300 };
  float roundness = 0.2f;
  int segments = 0; // Auto-calculate segments
  float lineThickness = 4.0f;

  // allow the user to see and close the window created
  while (!WindowShouldClose()) {
    BeginDrawing();

      ClearBackground(BLACK);

      // DrawText(TextFormat(centeredText), (SCREEN_WIDTH / 2) - (textWidth / 2), (SCREEN_HEIGHT / 2) - (fontSize / 2), fontSize, LIME);
      DrawText(TextFormat(aboveCenteredText), (SCREEN_WIDTH / 2) - (textWidth - 78), (SCREEN_HEIGHT / 2) - 100, fontSize, LIME);

      // completely centered text
      DrawText(TextFormat(centeredText), (SCREEN_WIDTH / 2) - (textWidth / 2), (SCREEN_HEIGHT / 2) - (fontSize - 2), fontSize, LIME);

      // WIDTH: increasing inner values makes it go more towards the left as it get's closer to '0'
      // HEIGHT: increasing inner values makes it go more towards the NORTH as it get's closer to '0'
      DrawText(TextFormat("Testing"), (SCREEN_WIDTH / 2) - (290), (SCREEN_HEIGHT / 2) - (0), fontSize, LIME);

      DrawTextEx(fnt_Rubik, "Rubik Text Font", rubikFontTextPos, fontSize, 1.0f, WHITE);

      DrawTextEx(fnt_Arial, "( Original 'C:\\Windows\\Fonts' ) Arial Text Font", arialFontTextPos, fontSize, 1.0f, WHITE);

      // DrawCircle(800 / 2, 600 / 2, circleRadius, GREEN);
      // DrawRectangleRoundedLinesEx(myBox, roundness, segments, lineThickness, WHITE);

    EndDrawing();
  }

  // unload the file data that we used for Arial font
  UnloadFileData(fontData);

  // close the created window
  CloseWindow();

  return 0;
}
