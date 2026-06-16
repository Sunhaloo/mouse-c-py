#include "include/raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

#include "win32_input.h"

// use `enums` for the "state machine"
typedef enum {
  STATE_IDLE,
  STATE_LOGGING,
  STATE_GENERATING,
  STATE_DONE,
  STATE_STOPPED,
} AppState;

// define window size
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 400

// our main function
int main(void) {
  // initialise Raylib window first
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "mouse-c-py");

  // set the FPS to a smooth 60 ( not as smooth if you have higher hertz )
  SetTargetFPS(60);

  // setup Win32 raw input
  if (win32_setup() != 0) {
    CloseWindow();
    return 1;
  }

  // setup font size
  float globalFontSize = 20.0f;
  float mainTitleFontSize = 28.0f;

  // setup styling ( global styling )
  GuiSetStyle(DEFAULT, TEXT_SIZE, globalFontSize);
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
  GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(GREEN));
  GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(RED));


  // GUI program state
  AppState currentState = STATE_IDLE;
  bool isDarkTheme = false;


  // main game loop
  while (!WindowShouldClose()) {

    // update states
    if (currentState == STATE_LOGGING) {
      // thread updates g_elapsed_time and g_is_logging automatically
      if (!g_is_logging) {
        currentState = STATE_GENERATING;
        // call the function to generate images with `graphs.exe`
        win32_launch_graphs();
        currentState = STATE_DONE;
      }
    }

    // theme button
    const char *buttonThemeTxt = isDarkTheme ? "Light Theme" : "Dark Theme";
    float themeBtnWidth = MeasureText(buttonThemeTxt, globalFontSize);
    float themeButtonPadWidth = themeBtnWidth + 20.0f;
    float themeButtonPadHeight = globalFontSize + 10.0f;
    float themeButtonPosX = SCREEN_WIDTH - themeButtonPadWidth - 10.0f;
    float themeButtonPosY = 10.0f;

    // main title label
    const char *mainLabelTitle = "Mouse Coordinates Logger and Heatmap Generator";
    float mainLblWidth = MeasureText(mainLabelTitle, mainTitleFontSize);
    float mainPadLblWidth = mainLblWidth + 10.0f;
    float mainPadLblHeight = mainTitleFontSize + 6.0f;
    float mainLblX = (SCREEN_WIDTH / 2.0f) - (mainLblWidth / 2.0f) + 20.0f;
    float mainLblY = (SCREEN_HEIGHT / 2.0f) - (mainTitleFontSize / 2.0f) - 120.0f;

    // slider
    const char *leftSliderTxt = "Logging Time";
    float leftSliderTxtWidth = MeasureText(leftSliderTxt, globalFontSize);
    float sliderBarDragWidth = 420.0f;
    float sliderBarDragHeight = 32.0f;
    float sliderTextPadding = 10.0f;
    float rightSliderTxtWidth = 80.0f;
    float totalSliderBarWidth = leftSliderTxtWidth + sliderTextPadding +
                                sliderBarDragWidth + sliderTextPadding +
                                rightSliderTxtWidth;
    float sliderBarRenderingStart = (SCREEN_WIDTH / 2.0f) - (totalSliderBarWidth / 2.0f);
    float sliderBarX = sliderBarRenderingStart + leftSliderTxtWidth + sliderTextPadding;
    float sliderBarY = (SCREEN_HEIGHT / 2.0f) - (globalFontSize / 2.0f) - 50.0f;

    // stop logging button
    const char *buttonStopTxt = "Stop Logging";
    float stopBtnWidth = MeasureText(buttonStopTxt, globalFontSize);
    float stopButtonPadWidth = stopBtnWidth + 20.0f;
    float stopButtonPadHeight = globalFontSize + 10.0f;
    float stopButtonPosX = 92.0f;
    float stopButtonPosY = SCREEN_HEIGHT - stopButtonPadHeight - 10.0f;

    // start logging button
    const char *buttonStartTxt = "Start Logging";
    float startBtnWidth = MeasureText(buttonStartTxt, globalFontSize);
    float startButtonPadWidth = startBtnWidth + 20.0f;
    float startButtonPadHeight = globalFontSize + 10.0f;
    float startButtonPosX = SCREEN_WIDTH - startButtonPadWidth - 116.0f;
    float startButtonPosY = stopButtonPosY;

    // reset slider button
    const char *buttonResetTxt = "Reset Slider";
    float sliderResetBtnWidth = MeasureText(buttonResetTxt, globalFontSize);
    float sliderResetBtnPadWidth = sliderResetBtnWidth + 20.0f;
    float sliderResetBtnPadHeight = globalFontSize + 10.0f;
    float sliderResetBtnPosX = (SCREEN_WIDTH / 2.0f) - (sliderResetBtnWidth / 2.0f);
    float sliderResetBtnPosY = (SCREEN_HEIGHT / 2.0f) - (globalFontSize / 2.0f);

    // "information" rectangle
    float infoRectWidth = 600.0f;
    float infoRectHeight = 80.0f;
    float infoRectPosX = (SCREEN_WIDTH / 2.0f) - (infoRectWidth / 2.0f);
    float infoRectPosY = (SCREEN_HEIGHT / 2.0f) - (infoRectHeight / 2.0f) + 80.0f;

    // text to write on screen / window based on current state
    const char *infoText = "";
    switch (currentState) {
      case STATE_IDLE:
        infoText = "Press Start Logging to get started";
        break;
      case STATE_LOGGING:
        infoText = TextFormat("Time Remaining: %.1f seconds",
                              (double)g_logging_duration - g_elapsed_time);
        break;
      case STATE_GENERATING:
        infoText = "Logging complete... Generating heatmap";
        break;
      case STATE_DONE:
        infoText = "Heatmap generated! Check your Downloads folder";
        break;
      case STATE_STOPPED:
        infoText = "Logging stopped";
        break;
    }


    // start drawing stuff to the window
    BeginDrawing();

    // set the background colour ==> initially set to 'WHITE'
    Color bgColor = GetColor(GuiGetStyle(DEFAULT, BACKGROUND_COLOR));
    ClearBackground(bgColor);

    // theme button
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

      // NOTE: we need to set these again due to theme switcher reloading everything to default using `GuiLoadStyleDefault`
      GuiSetStyle(DEFAULT, TEXT_SIZE, globalFontSize);
      GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, ColorToInt(GREEN));
      GuiSetStyle(DEFAULT, TEXT_COLOR_PRESSED, ColorToInt(RED));
    }

    // main title
    GuiSetStyle(DEFAULT, TEXT_SIZE, mainTitleFontSize);
    GuiLabel((Rectangle){mainLblX, mainLblY, mainPadLblWidth, mainPadLblHeight}, mainLabelTitle);
    GuiSetStyle(DEFAULT, TEXT_SIZE, globalFontSize);

    // slider and reset ( only in IDLE and STOPPED states )
    if (currentState == STATE_IDLE || currentState == STATE_STOPPED) {
      // get the `volatile value` into an actual `float` data type for slider
      float sliderValue = g_logging_duration;

      GuiSliderBar(
        (Rectangle){sliderBarX, sliderBarY, sliderBarDragWidth, sliderBarDragHeight},
        leftSliderTxt, TextFormat("%.0f s", sliderValue),
        &sliderValue, 60, 3600
      );

      // write slider value back to shared global
      g_logging_duration = sliderValue;

      if (GuiButton((Rectangle){sliderResetBtnPosX, sliderResetBtnPosY, sliderResetBtnPadWidth, sliderResetBtnPadHeight}, buttonResetTxt)) {
        g_logging_duration = 60.0f;
      }
    }

    // "information" rectangle
    GuiDrawRectangle(
        (Rectangle){infoRectPosX, infoRectPosY, infoRectWidth, infoRectHeight},
        2,
        LIME,
        BLANK
    );

    // information text centred inside rectangle
    float infoTextWidth = MeasureText(infoText, globalFontSize);
    float infoTextX = infoRectPosX + (infoRectWidth / 2.0f) - (infoTextWidth / 2.0f);
    float infoTextY = infoRectPosY + (infoRectHeight / 2.0f) - (globalFontSize / 2.0f);
    Color infoTextColor = GetColor(GuiGetStyle(DEFAULT, TEXT_COLOR_NORMAL));
    DrawText(infoText, (int)infoTextX, (int)infoTextY, (int)globalFontSize, infoTextColor);

    // stop button ( only during logging )
    if (currentState == STATE_LOGGING) {
      if (GuiButton((Rectangle){stopButtonPosX, stopButtonPosY, stopButtonPadWidth, stopButtonPadHeight}, buttonStopTxt)) {
        win32_stop_logging();
        currentState = STATE_STOPPED;
      }
    }

    // start button ( only in IDLE or STOPPED )
    if (currentState == STATE_IDLE || currentState == STATE_STOPPED) {
      if (GuiButton((Rectangle){startButtonPosX, startButtonPosY, startButtonPadWidth, startButtonPadHeight}, buttonStartTxt)) {
        win32_start_logging();
        currentState = STATE_LOGGING;
      }
    }

    EndDrawing();
  }

  // cleanup
  win32_cleanup();
  CloseWindow();

  return 0;
}
