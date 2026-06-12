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
  const char *windowTitle = "mouse-c-py";

  // initialise the main window rendered out by Windows
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, windowTitle);

  // set the target to 30
  SetTargetFPS(30);

  // change the global font size to 20
  float globalFontSize = 20.0f;
  // font size for the main title label
  float mainTitleFontSize = 28.0f;

  // set the font size to desired size
  GuiSetStyle(DEFAULT, TEXT_SIZE, globalFontSize);

  // variable to hold status flag for theme switching
  bool isDarkTheme = false;

  // variable to hold the logging time set by the slider
  float loggingSliderTime = 60.0f;

  while (!WindowShouldClose()) {
    // create constant to hold button theme text depending on theme
    const char *buttonThemeTxt = isDarkTheme ? "Light Theme" : "Dark Theme";

    // measure the size of the theme button text dynamically
    float themeBtnWidth = MeasureText(buttonThemeTxt, globalFontSize);

    // setup button padding
    float buttonPadWidth = themeBtnWidth + 20.0f;
    float buttonPadHeight = globalFontSize + 10.0f;

    // setup button positioning
    float buttonPosX = SCREEN_WIDTH - buttonPadWidth - 10.0f;
    float buttonPosY = 10.0f;

    // create constant to hold main label title
   const char *mainLabelTitle = "Mouse Coordinates Logger and Heatmap Generator";

   // measure the size of the main label text dynamically
   float mainLblWidth = MeasureText(mainLabelTitle, mainTitleFontSize);

   // setup main label padding
   float mainPadLblWidth = mainLblWidth + 10.0f;
   float mainPadLblHeight = mainTitleFontSize + 6.0f;

   // setup main label title positioning
   float mainLblX = (SCREEN_WIDTH / 2.0f) - (mainLblWidth / 2.0f);
   float mainLblY = (SCREEN_HEIGHT / 2.0f) - (mainTitleFontSize / 2.0f) - 120.0f;

   // create constant to hold left and right text of slider bar
   const char *leftSliderTxt = "Logging Time";

   // measure the size of the left and right slider bar text
   float leftSliderTxtWidth = MeasureText(leftSliderTxt, globalFontSize);
   float rightSliderTxtWidth = 80.0f;

   // setup draggable size of slider bar
   float sliderBarDragWidth = 420.0f;
   float sliderBarDragHeight = 32.0f;

   // slider text padding
   float sliderTextPadding = 10.0f;

   // slider bar total width
   float totalSliderBarWidth = leftSliderTxtWidth + sliderTextPadding + sliderBarDragWidth + sliderTextPadding + rightSliderTxtWidth;

   // slider bar position ( of where it should start rendering from )
   float sliderBarRenderingStartPos = (SCREEN_WIDTH / 2.0f) - (totalSliderBarWidth / 2.0f);

   // setup slider bar positioning
   float sliderBarX = sliderBarRenderingStartPos + leftSliderTxtWidth + sliderTextPadding;
   float sliderBarY = (SCREEN_HEIGHT / 2.0f) - (globalFontSize / 2.0f) - 50.0;

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
      GuiSetStyle(DEFAULT, TEXT_SIZE, globalFontSize);
    }

    // change the font size ==> immediate mode GUI --> we need to do this
    GuiSetStyle(DEFAULT, TEXT_SIZE, mainTitleFontSize);

    // create main title label
    GuiLabel((Rectangle){mainLblX, mainLblY, mainPadLblWidth, mainPadLblHeight}, mainLabelTitle);

    // change the font back to what we had
    GuiSetStyle(DEFAULT, TEXT_SIZE, globalFontSize);

    // create slider bar for amount of time to log mouse coordinates
    GuiSliderBar((Rectangle){sliderBarX, sliderBarY, sliderBarDragWidth, sliderBarDragHeight}, leftSliderTxt, TextFormat("%0.2f", loggingSliderTime), &loggingSliderTime, 60, 3600);

    // drawing finishes ==> end the drawing
    EndDrawing();
  }

  // close the window created
  CloseWindow();

  return 0;
}
