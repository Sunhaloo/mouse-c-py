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
  // set global button normal colour
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
  // set global button hover colour
  GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(GREEN));
  // set global button pressed colour
  GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(RED));

  // variable to hold status flag for theme switching
  bool isDarkTheme = false;

  // variable to hold the logging time set by the slider
  float loggingSliderTime = 60.00f;

  while (!WindowShouldClose()) {
    // create constant to hold button theme text depending on theme
    const char *buttonThemeTxt = isDarkTheme ? "Light Theme" : "Dark Theme";

    // measure the size of the theme button text dynamically
    float themeBtnWidth = MeasureText(buttonThemeTxt, globalFontSize);

    // setup theme button padding
    float themeButtonPadWidth = themeBtnWidth + 20.0f;
    float themeButtonPadHeight = globalFontSize + 10.0f;

    // setup theme button positioning
    float themeButtonPosX = SCREEN_WIDTH - themeButtonPadWidth - 10.0f;
    float themeButtonPosY = 10.0f;

    // create constant to hold main label title
   const char *mainLabelTitle = "Mouse Coordinates Logger and Heatmap Generator";

   // measure the size of the main label text
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

   // create constant to hold button stop text
   const char *buttonStopTxt = "Stop Logging";

   // measure the size of the stop logging button text
   float stopBtnWidth = MeasureText(buttonStopTxt, globalFontSize);

   // setup stop logging button padding
   float stopButtonPadWidth = stopBtnWidth + 20.0f;
   float stopButtonPadHeight = globalFontSize + 10.0f;

   // setup stop logging button positioning
   float stopButtonPosX = 100.0f;
   float stopButtonPosY = SCREEN_HEIGHT - stopButtonPadHeight - 10.0f;

   // create constant to hold button start text
   const char *buttonStartTxt = "Start Logging";

   // measure the size of the start logging button text
   float startBtnWidth = MeasureText(buttonStartTxt, globalFontSize);

   // setup start logging button padding
   float startButtonPadWidth = startBtnWidth + 20.0f;
   float startButtonPadHeight = globalFontSize + 10.0f;

   // setup the start logging button positioning
   float startButtonPosX = SCREEN_WIDTH - startButtonPadWidth - 110.0f;
   float startButtonPosY = stopButtonPosY;

   // create constant to hold button reset slider text
   const char *buttonResetSliderTxt = "Reset Slider";

   // measure the size of the of the reset slider button text
   float sliderResetBtnWidth = MeasureText(buttonResetSliderTxt, globalFontSize);

   // setup reset slider button padding
   float sliderResetButtonPadWidth = sliderResetBtnWidth + 20.0f;
   float sliderResetButtonPadHeight = globalFontSize + 10.0f;

   // setup reset slider button positioning
   float sliderResetButtonPosX = (SCREEN_WIDTH / 2.0f) - (sliderResetBtnWidth / 2.0f);
   float sliderResetButtonPosY = (SCREEN_HEIGHT / 2.0f) - (globalFontSize / 2.0f);

   // start drawing stuff inside the window created
   BeginDrawing();

   // fetch the colour from the active theme
   Color currentTheme = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));

   // set the background colour to the current theme
   ClearBackground(currentTheme);

   // create our theme switcher button
   if (GuiButton((Rectangle){themeButtonPosX, themeButtonPosY, themeButtonPadWidth, themeButtonPadHeight}, buttonThemeTxt)) {
     // button pressed ==> swap the status check
     isDarkTheme = !isDarkTheme;

     // if we are currently on light theme
     if (isDarkTheme) {
       // change the theme to dark
       GuiLoadStyle("style_dark.rgs");

       // set global button normal colour
       GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(WHITE));

     // if we are currently on dark theme
     } else {
       // change theme to light ==> use raylib's default styling
       GuiLoadStyleDefault();

       // set global button normal colour
       GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
     }

     // NOTE: we need to do this again due to theme switcher reloading everything to default using `GuiLoadStyleDefaultj`
     // set the font size to desired size
     GuiSetStyle(DEFAULT, TEXT_SIZE, globalFontSize);
     // set global button hover colour
     GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(GREEN));
     // set global button pressed colour
     GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(RED));
   }

   // change the font size ==> immediate mode GUI --> we need to do this
   GuiSetStyle(DEFAULT, TEXT_SIZE, mainTitleFontSize);

   // create main title label
   GuiLabel((Rectangle){mainLblX, mainLblY, mainPadLblWidth, mainPadLblHeight}, mainLabelTitle);

   // change the font back to what we had
   GuiSetStyle(DEFAULT, TEXT_SIZE, globalFontSize);

   // create slider bar for amount of time to log mouse coordinates
   GuiSliderBar((Rectangle){sliderBarX, sliderBarY, sliderBarDragWidth, sliderBarDragHeight}, leftSliderTxt, TextFormat("%0.2f", loggingSliderTime), &loggingSliderTime, 60, 3600);

   // create the reset slider button
   if (GuiButton((Rectangle){sliderResetButtonPosX, sliderResetButtonPosY, sliderResetButtonPadWidth, sliderResetButtonPadHeight}, buttonResetSliderTxt)) {
     // simply reset the actual current value for the slider bar for next "rendering frame"
     loggingSliderTime = 60.00f;
   }

   // create the stop logging button
   if (GuiButton((Rectangle){stopButtonPosX, stopButtonPosY, stopButtonPadWidth, stopButtonPadHeight}, buttonStopTxt)) {}

   // create the start logging button
   if (GuiButton((Rectangle){startButtonPosX, startButtonPosY, startButtonPadWidth, startButtonPadHeight}, buttonStartTxt)) {}

   // drawing finishes ==> end the drawing
   EndDrawing();
  }

  // close the window created
  CloseWindow();

  return 0;
}
