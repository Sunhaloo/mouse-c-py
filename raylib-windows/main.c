// include the actual raylib "functions", etc
#include "include/raygui.h"
#include "include/raylib.h"

// include the actual raygui "functions", etc
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

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

  // change the font size to 18
  float fontSize = 18.0f;

  // set the font size to desired size
  GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);

  // variable to hold status flag for theme switching
  bool isDarkTheme = false;

  while (!WindowShouldClose()) {
    // create constant to hold button theme text depending on theme
    const char *buttonThemeTxt = isDarkTheme ? "Ligh Theme" : "Dark Theme";

    // measure the size of the theme button text dynamically
    float themeBtnWidth = MeasureText(buttonThemeTxt, fontSize);

    // setup button padding
    float buttonPadWidth = themeBtnWidth + 20.0f;
    float buttonPadHeight = fontSize + 10.0f;

    // setup button positioning
    float buttonPosX = SCREEN_WIDTH - buttonPadWidth - 10.0f;
    float buttonPosY = 10.0f;

    // create constant to hold main label title
   const char *mainLabelTitle = "Mouse Coordinates Logger and Heatmap Generator";

   // measure the size of the main label text dynamically
   float mainLblWidth = MeasureText(mainLabelTitle, fontSize);

   // setup main lable padding
   float mainPadLblWidth = mainLblWidth + 10.0f;
   float mainPadLblHeight = fontSize + 8.0f;

   // setup main label title positioning
   float mainLblX = (SCREEN_WIDTH / 2.0f) - (mainLblWidth / 2.0f);
   float mainLblY = (SCREEN_HEIGHT / 2.0f) - (fontSize / 2.0f);

    // start drawing stuff inside the window created
    BeginDrawing();

    // fetch the colour from the active theme
    Color currentTheme = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));

    // set the background colour to the current theme
    ClearBackground(currentTheme);

    // create our theme switcher button
    if (GuiButton((Rectangle){buttonPosX, buttonPosY, buttonPadWidth, buttonPadHeight}, buttonThemeTxt)) {
      // button pressed ==> swap the status check
      isDarkTheme = !isDarkTheme;

      // if we are currently on light theme
      if (isDarkTheme) {
        // change the theme to dark
        GuiLoadStyle("style_dark.rgs");

      // if we are currently on dark theme
      } else {
        // change theme to light ==> use raylib's default styling
        GuiLoadStyleDefault();
      }

      // set the font size to desired size
      GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);
    }

    // create main title label
    GuiLabel((Rectangle){mainLblX, mainLblY, mainPadLblWidth, mainPadLblHeight}, mainLabelTitle);

    // drawing finishes ==> end the drawing
    EndDrawing();
  }

  // close the window created
  CloseWindow();

  return 0;
}
