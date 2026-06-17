---
id: Raylib and RayGUI ( Windows )
aliases: An introduction to Raylib and RayGUIon Windows
tags:
  - C
  - windows
author: S.Sunhaloo
date: 2026-06-10
status: Completed
---

> [!INFO] Resource(s)
> - Raylib:
> 	- Official Website: https://www.raylib.com/
> 	- GitHub: https://github.com/raysan5/raylib
> 	- Wiki: https://github.com/raysan5/raylib/wiki
> - RayGUI:
> 	- GitHub: https://github.com/raysan5/raygui

# An Introduction To RayLib

> [!INFO] Resource(s)
> - Coding with Sphere: https://www.youtube.com/watch?v=AniAoJC6QAc

## Installation

> [!INFO] Resources(s)
> - Official Raylib Wiki:
> 	- Working on Windows: https://github.com/raysan5/raylib/wiki/Working-on-Windows
> 	- Raylib Quickstart: https://github.com/raylib-extras/raylib-quickstart


Following Coding with Sphere, we are going to be cloning the whole `raylib` folder and therefore `make` / *build* everything on our computer.

> [!NOTE]
> The whole `raylib` repository is just ( *around* ) 509 megabytes.

- Clone the `raylib` repository

```powershell
# clone the raylib repository
git clone https://github.com/raysan5/raylib.git
```

- Compile everything with `make`:

```powershell
# build everything on our machine itself
cd src; make PLATFORM=PLATFORM_DESKTOP
```

> It should generally **not** take a while to build everything!

> [!INFO]
> 
> > I did **not** that Raylib is supported on other platforms.
> 
> So apparently Raylib can work on Desktop Computer, on the Web, on Android and also on Embedded Machines.
> 
> That is the reason as do why we have this "*target*" `PLATFORM=PLATFORM_DESKTOP` type of thing!
> 
> > I recommended myself to just glance over the `Makefile` that they provided... Therefore, I suggest you to do the same thing even if we both know that we won't understand a single thing ( *after the platform "code"* )

> [!WARNING]
> You need to have `gcc` and `make` installed on your Windows machine to be able to compile it!
> 
> Please read the following AI generated note to be able to install 'MYSYS' on your system and thereby be able to install `gcc` and `make`.
> 
> > [!BUG] Brother WTF!
> > > "We do not recommend using MSYS2 to get GCC, even though several tutorials suggest it. It can be difficult to configure properly and does not always include all the things needed for a complete development system. The **W64-Devkit** is a much better way to get the needed toolchain for building games with raylib."
> 
> > It now that I am seeing this... But I digress; I hope it works now. NO, IT WILL WORK!

> [!SUCCESS]
> You should see the following "*message*" after compiling it.
> 
> ```console
> "raylib static library generated (libraylib.a) in ../src!"
> ```
> 
> Don't lose the `libraylib.a` **static file** as we are going to need it in our project.

## Setup Project

I am now going to setup another directory for our actual *project* whereby we are going to learn about Raylib.

- This is how my project directory looks like ( *after following 'Coding with Sphere'* ):

```console
C:\USERS\USERNAME\DESKTOP\RAYLIB-LEARNING\LEARNING
├───include
│       raylib.h
│
└───lib
        libraylib.a
```

The above output was done using the `tree /F .` command inside the `learning` folder.

### Create Main C File

- Create a simple `main.c` file at the root of `learning` folder / directory:

```C
#include <stdio.h>

int main(void) {
  printf("\nWe are going to learn about Raylib!\n\n");

  return 0;
}
```

### How To Compile

Let's say that we use `math.h` in a C project / program... Therefore, we are going to have to add the `-lm` flag to `gcc` when we are compiling.

As usual, the same thing applies here whereby we are going to have to find out what **flags** we need to add in order for our program to compile.

> [!INFO]
> The find the *compilation command* for your specific platform... First head over to the wiki and in my case, I am going to look for 'Working on Windows'.
> 
> > Again, if you are on MacOS or Linux; search for 'Working on MacOS / Linux' respectively.
> 
> Link: https://github.com/raysan5/raylib/wiki/Working-on-Windows#manual-setup-with-w64devkit ( *check Step 5* )
> 
> Looking around and searching for `gcc`, I found this:
> 
> ```powershell
> gcc -o raylibhelloworld.exe raylibhelloworld.c -I include -L lib -lraylib -lgdi32 -lwinmm
> ```
> 
> Then asking [ChatGPT](https://chat.openai.com) about it, it gave me the following:
> 
> ```powershell
> gcc main.c -o main.exe -Iinclude -Llib -lraylib -lgdi32 -lwinmm
> ```
> 
> - Full command to compile ( *also with my `-Wall -Wextra -pedantic`* ) our `main.c` file:
> 
> ```powershell
> gcc main.c -Wall -Wextra -pedantic -Iinclude -Llib -lraylib -lgdi32 -lwinmm -lopengl32 -lshell32 -o program.exe
> ```
> 
> > I am going to use the *last* command!

> [!SUCCESS]
> Therefore, I see the following when I run the above command at the root of `learning`:
> 
> ```console
> 
> We are going to learn about Raylib!
> 
> ```

## Create Makefile

> Again, to be able to use `make` command you have to install 'mingw-make' program.

- Create the following make file at the root of `learning`:

```make
program: compile run clean

compile:
	@gcc main.c -Wall -Wextra -pedantic -Iinclude -Llib -lraylib -lgdi32 -lwinmm -lopengl32 -lshell32 -o program.exe

run:
	@./program

clean:
	@del program.exe
```

- **Modify** our `main.c` file to actually include the `raylib.h` file:

```C
#include "include/raylib.h"
#include <stdio.h>

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
```

- This is the output after running ( *and "seeing"* ) the window create:

```console
INFO: Initializing raylib 6.1-dev
INFO: Platform backend: DESKTOP (GLFW)
INFO: Supported raylib modules:
INFO:     > rcore:..... loaded (mandatory)
INFO:     > rlgl:...... loaded (mandatory)
INFO:     > rshapes:... loaded (optional)
INFO:     > rtextures:. loaded (optional)
INFO:     > rtext:..... loaded (optional)
INFO:     > rmodels:... loaded (optional)
INFO:     > raudio:.... loaded (optional)
INFO: DISPLAY: Device initialized successfully
INFO:     > Display size: 1920 x 1080
INFO:     > Screen size:  800 x 600
INFO:     > Render size:  800 x 600
INFO:     > Viewport offsets: 0, 0
INFO: GLAD: OpenGL extensions loaded successfully
INFO: GL: Supported extensions count: 403
INFO: GL: OpenGL device information:
INFO:     > Vendor:   NVIDIA Corporation
INFO:     > Renderer: NVIDIA GeForce RTX 4060 Laptop GPU/PCIe/SSE2
INFO:     > Version:  3.3.0 NVIDIA 596.49
INFO:     > GLSL:     3.30 NVIDIA via Cg compiler
INFO: GL: VAO extension detected, VAO functions loaded successfully
INFO: GL: NPOT textures extension detected, full NPOT textures supported
INFO: GL: DXT compressed textures supported
INFO: GL: ETC2/EAC compressed textures supported
INFO: PLATFORM: DESKTOP (GLFW - Win32): Initialized successfully
INFO: TEXTURE: [ID 1] Texture loaded successfully (1x1 | R8G8B8A8 | 1 mipmaps)
INFO: TEXTURE: [ID 1] Default texture loaded successfully
INFO: SHADER: [ID 1] Vertex shader compiled successfully
INFO: SHADER: [ID 2] Fragment shader compiled successfully
INFO: SHADER: [ID 3] Program shader loaded successfully
INFO: SHADER: [ID 3] Default shader loaded successfully
INFO: RLGL: Render batch vertex buffers loaded successfully in RAM (CPU)
INFO: RLGL: Render batch vertex buffers loaded successfully in VRAM (GPU)
INFO: RLGL: Default OpenGL state initialized successfully
INFO: TEXTURE: [ID 2] Texture loaded successfully (128x128 | GRAY_ALPHA | 1 mipmaps)
INFO: FONT: Default font loaded successfully (224 glyphs)
INFO: SYSTEM: Working Directory: C:\Users\username\Desktop\raylib-learning\learning
```

---

# What Am I Trying To Achieve?

> Please do refer to the [[mouse-c-py Random Notes]] project!

So the main thing that I want my GUI to do is to basically to allow the user to change the time it takes for the program to run maybe a range for 60 seconds to 900 seconds ( 30 minutes ).

Then after the user clicks on the 'Run' button (  *or something like that* ); there will be a "*countdown*" that will show how long the program still needs to run and I mean we also could add an 'Abort' button that will basically kill all programs and if there are any residual `mouse_data.csv` file left.

## Writing Stuff On The Screen

> [!INFO] Resource(s)
> - Raylib Example: https://www.raylib.com/examples/text/loader.html?name=text_format_text
> - Andrew Hamel Codes: https://youtu.be/fXIbeM0PO5g?si=586QaniU0Z0HwiKl

### Simple Code To Display Hello World

> The "Hello World" of Raylib!

```C
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

  // always create text variable to use
  char *welcomingText = "Hello World";

  // setup "font"
  // INFO: get the default font size used by raylib ( default font size = 10 )
  float fontSize = GetFontDefault().baseSize;

  // run the window "indefinitely" until user closes it
  while (!WindowShouldClose()) {
    // start drawing stuff inside the window created
    BeginDrawing();

    // change the background colour 'white' colour
    ClearBackground(WHITE);

    // display hello world at the top-left corner of size 10 with colour black
    DrawText(TextFormat(welcomingText), 0, 0, fontSize, BLACK);

    // drawing finishes ==> end the drawing
    EndDrawing();
  }

  // close the window created
  CloseWindow();

  return 0;
}
```

> [!NOTE]
> Given that size of your screen and stuff like that, using the above screen width and height. We should see a white screen and at the top-left corner. The **small** 'Hello World' text should appear.
> 
> > You might need to squint your eyes for you to see it!

#### Use Custom Font Found On Windows

```C
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

  // variable to hold our text to display
  char *textToDisplay = "Arial Font";

  // get the 'Arial' font found on most ( if not all ) Windows systems
  int fileSize;
  unsigned char *fontData = LoadFileData("C:\\Windows\\Fonts\\arial.ttf", &fileSize);
  Font fntArial = LoadFontFromMemory(".ttf", fontData, fileSize, 64, 0, 250);

  // change the font size
  float fontSize = 24.0f;
  // use the default value for font spacing
  float fontSpacing = 1.0f;

  // measure the width of the font ( to be able to center it )
  Vector2 textSize = MeasureTextEx(fntArial, textToDisplay, fontSize, fontSpacing);

  // NOTE: use vector positions with `DrawTextEx`
  // in this case, we are going to have our text in the center of the window
  Vector2 textPos = (Vector2) {(SCREEN_WIDTH / 2.0f) - (textSize.x / 2.0f), (SCREEN_HEIGHT / 2.0f) - (textSize.y / 2.0f)};

  // run the window "indefinitely" until user closes it
  while (!WindowShouldClose()) {
    // start drawing stuff inside the window created
    BeginDrawing();

    // change the background colour 'white' colour
    ClearBackground(WHITE);

    // display text at the center of size 20 with colour dark gray
    DrawTextEx(fntArial, textToDisplay, textPos, fontSize, fontSpacing, DARKGRAY);

    // drawing finishes ==> end the drawing
    EndDrawing();
  }

  // WARNING: need to unload the file data that we used for Arial font
  UnloadFileData(fontData);
  UnloadFont(fntArial);

  // close the window created
  CloseWindow();

  return 0;
}
```

> [!WARNING]
> Take the above code with a grain of salt as I don't know if its actually the way that we need to do it as the code related to *using* the "*custom*" font found on Windows was provided by **Google AI Overview**.

#### Use Custom Downloaded Font

> [!NOTE]
> For this one, I downloaded the `Rubik-Regular.ttf` font and placed it at the root of our `learning` folder.

```C
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

  // variable to hold our text to display
  char *textToDisplay = "Rubik Font";

  // get the 'Arial' font found on most ( if not all ) Windows systems
  // use custom downloaded font
  Font fntRubik = LoadFont("Rubik-Regular.ttf");

  // check if we are able to load the font
  if (IsFontValid(fntRubik)) {
    TraceLog(LOG_ERROR, "Rubik font is invalid!");
  }

  // change the font size
  float fontSize = 32.0f;
  // use the default value for font spacing
  float fontSpacing = 1.0f;

  // measure the width of the font ( to be able to center it )
  Vector2 textSize = MeasureTextEx(fntRubik, textToDisplay, fontSize, fontSpacing);

  // NOTE: use vector positions with `DrawTextEx`
  // in this case, we are going to have our text in the center of the window
  Vector2 textPos = (Vector2) {(SCREEN_WIDTH / 2.0f) - (textSize.x / 2.0f), (SCREEN_HEIGHT / 2.0f) - (textSize.y / 2.0f)};

  // run the window "indefinitely" until user closes it
  while (!WindowShouldClose()) {
    // start drawing stuff inside the window created
    BeginDrawing();

    // change the background colour 'white' colour
    ClearBackground(WHITE);

    // display text at the center of size 32 with colour dark gray
    DrawTextEx(fntRubik, textToDisplay, textPos, fontSize, fontSpacing, DARKGRAY);

    // drawing finishes ==> end the drawing
    EndDrawing();
  }

  // close the window created
  CloseWindow();

  return 0;
}
```

### Display Value And Update Value ( Mouse and Keyboard Events )

```C
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
```

## GUI Controls

In terms of control that I want to give to the user; is that the user should be able to adjust a slider and be able to control the amount of time the program can run.

> In terms of the actual C file for my [[mouse-c-py Random Notes | mouse-c-py]] project but we are going to figure it out.

Then when the program is running for that amount of time; we should be able to display below the amount of time remaining before the program finishes it logging part. And then when we arrive at the `CreateProcessW`; we should then display something like 'Generating Heatmap...' and when the `graphs.exe` stops running, we should display a message like 'Completed! Please Check Downloads Folder' and keep the application open.

Therefore, I went searching around; and this is what I found:

- https://www.raylib.com/examples/shapes/loader.html?name=shapes_hilbert_curve
- https://www.raylib.com/examples/models/loader.html?name=models_animation_blending

Well, `GuiSlider` is not part of the default `raylib.h` header file and instead we need to use `raygui.h`.

> So, in this case, let's go ahead check if we can completely switch to Raygui without having to use Raylib.

---

# Raygui

So we should **not** completely switch off to using **only** `raygui.h`.

> This is what I also see when looking at the examples provided!

Therefore, go ahead and download the `raygui.h` file and we simply need to place it in our `learning/include` folder and we should be good to go.

## "Installing" Raygui

Instead of downloading the file directly from GitHub; I want to try something. Clone only the `src` folder of the `raygui` repository!

- Clone repository without checking out the files:

```powershell
git clone --no-checkout https://github.com/raysan5/raygui.git
```

- Head into that directory and create a sparse checkout:

```powershell
cd raygui; git sparse-checkout init --cone
```

- Get the `src` folder found at the root of the repository:

```powershell
git sparse-checkout set src
```

- Checkout the `master` branch ( *in this case* ):

```powershell
git checkout master
```

> [!SUCCESS]
> You should see that we now have the `raygui/src` folder ( *where you cloned it using the above steps* ) and then you should clearly see that we have the `raygui.h` header file inside it.

> [!INFO]
> Simply go ahead and move that `raygui.h` file to our `learning/include` folder and therefore add the following at the top.
> 
> > The code block below shows you the final thing that your file should look like at the top!
> 
> ```C
> // include the actual raylib "functions", etc
> #include "include/raylib.h"
> 
> // include the actual raygui "functions", etc
> #define RAYGUI_IMPLEMENTATION
> #include "include/raygui.h"
> ```

## Writing Stuff Using Raygui Labels

```C
// include the actual raylib "functions", etc
#include "include/raylib.h"

// include the actual raygui "functions", etc
#define RAYGUI_IMPLEMENTATION
#include "include/raygui.h"

// define our window size
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// our main function
int main(void) {
  // title for our window
  char *windowTitle = "Raylib + Raygui";

  // initialise the main window rendered out by Windows
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, windowTitle);

  // setup font size
  float fontSize = 32.0f;

  // use raygui style to set the text style
  // INFO: in this case we change the font size and font colour
  GuiSetStyle(DEFAULT, TEXT_SIZE, fontSize);
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, ColorToInt(BLACK));

  // variable to store our text message
  char *welcomingText = "Hello World";

  while (!WindowShouldClose()) {
    // start drawing stuff inside the window created
    BeginDrawing();

    // change the background colour 'white' colour
    ClearBackground(WHITE);

    // get the font that is currently activate
    Font defaultFont = GuiGetFont();

    // measure the width of the text
    int textWidth = MeasureText(welcomingText, fontSize);

    // display welcoming text in the center with font 32 and colour black
    GuiLabel((Rectangle){(SCREEN_WIDTH / 2.0f) - (textWidth / 2.0f), (SCREEN_HEIGHT / 2.0f) - (fontSize / 2.0f), textWidth, fontSize}, welcomingText);

    // drawing finishes ==> end the drawing
    EndDrawing();
  }

  // close the window created
  CloseWindow();

  return 0;
}
```

> [!INFO]
> In terms of coding it pretty similar to what we have been doing!

> [!WARNING]
> I am now going to close this note and continue everything over at '[[mouse-c-py Random Notes]]'.


---

# Socials

- **GitHub**: https://www.github.com/Sunhaloo
- **Instagram**: https://www.instagram.com/s.sunhaloo
- **YouTube**: https://www.youtube.com/@s.sunhaloo

---

S.Sunhaloo
Thank You!
