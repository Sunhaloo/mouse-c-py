#include "include/raylib.h"

// define our window size
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// our main function
int main(void) {
  // title for our window
  char *windowTitle = "Raylib ( Text )";

  // initialise the main window rendered out by Windows
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, windowTitle);

  // get the 'Arial' font found on most ( if not all ) Windows systems
  // use custom downloaded font
  Font fntRubik = LoadFont("Rubik-Regular.ttf");

  // check if we are able to load the font
  if (IsFontValid(fntRubik)) {
    TraceLog(LOG_ERROR, "Rubik font is invalid!");
  }

  // initialise a simple integer number to be displayed and later updated
  int number = 0;

  // run the window "indefinitely" until user closes it
  while (!WindowShouldClose()) {
    // if the user presses the left mouse button ==> increment the number
    // or if the user presses the '+' key ==> increment the number
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_EQUAL)) {
      number++;
    }
    // if the user presses the right mouse button ==> decrement the number
    // or if the user presses the '-' key ==> decrement the number
    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsKeyPressed(KEY_MINUS)) {
      number--;
    }
    // if the user presses the middle mouse button ==> reset the number to 0
    // or if the user presses the 'R' key ==> reset the number
    if (IsMouseButtonPressed(MOUSE_BUTTON_MIDDLE) || IsKeyPressed(KEY_R)) {
      number = 0;
    }


    // start drawing stuff inside the window created
    BeginDrawing();

    // change the background colour 'white' colour
    ClearBackground(WHITE);

    // variable to hold our text to display
    const char *textToDisplay = TextFormat("Current Number: %d", number);

    // change the font size
    float fontSize = 32.0f;
    // use the default value for font spacing
    float fontSpacing = 1.0f;

    // measure the width of the font ( to be able to center it )
    Vector2 textSize = MeasureTextEx(fntRubik, textToDisplay, fontSize, fontSpacing);

    // NOTE: use vector positions with `DrawTextEx`
    // in this case, we are going to have our text in the center of the window
    // Vector2 textPos = (Vector2) {(SCREEN_HEIGHT / 2.0f) - (121.5f), (SCREEN_HEIGHT / 2.0f) - (textSize.y / 2.0f)};
    Vector2 textPos = (Vector2) {(SCREEN_WIDTH / 2.0f) - (textSize.x / 2.0f), (SCREEN_HEIGHT / 2.0f) - (textSize.y / 2.0f)};

    // display text at the center of size 32 with colour dark gray
    DrawTextEx(fntRubik, textToDisplay, textPos, fontSize, fontSpacing, DARKGRAY);
    DrawTextEx(fntRubik, "(Left Mouse Button = +1)", (Vector2) {(SCREEN_WIDTH / 2.0f) - (162.0f), (SCREEN_HEIGHT / 2.0f) + (10.0f)}, fontSize, fontSpacing, LIGHTGRAY);
    DrawTextEx(fntRubik, "(Right Mouse Button = -1)", (Vector2) {(SCREEN_WIDTH / 2.0f) - (168.0f), (SCREEN_HEIGHT / 2.0f) + (36.0f)}, fontSize, fontSpacing, LIGHTGRAY);
    DrawTextEx(fntRubik, "(Keys: + / - / R)", (Vector2) {(SCREEN_WIDTH / 2.0f) - (80.0f), (SCREEN_HEIGHT / 2.0f) + (72.0f)}, fontSize, fontSpacing, LIGHTGRAY);

    // drawing finishes ==> end the drawing
    EndDrawing();
  }

  // close the window created
  CloseWindow();

  return 0;
}
