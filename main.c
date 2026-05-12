#include <errhandlingapi.h>
#include <minwindef.h>
#include <processthreadsapi.h>
#include <profileapi.h>
#include <stdio.h>
#include <hidusage.h>
#include <synchapi.h>
#include <windows.h>
#include <winnt.h>

// macro definition for how to long to run the program
#define TIME_TO_RUN 60.0

// global handle to our hidden window
HWND hwnd;

// global variables for our raw input "relative" coordinates
LONG last_x = 0;
LONG last_y = 0;

// forward declaration of WindowProc
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// structure that contains information for our hidden window
WNDCLASSEXW HiddenWindowClass;

// raw input device registration structure
RAWINPUTDEVICE rid;

// our point structure
POINT coordinates;

// FILE pointer for writing mouse data to CSV file
FILE *csv_file;

// create array for new process start-up and process information
STARTUPINFOW si = {0};
PROCESS_INFORMATION pi = {0};

// function to check if the `graphs.exe` file is present
// WARNING: implement later

// function to handle all non-raw-input message
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  // using `switch ... case` as it the standard and is natural for working with 'Win32'
  // additionally, given how 'Win32' was built --> make sense to use `switch ... case`
  // so that the readability improves compared to using `if ... else` statements
  switch(msg) {
    // if the incoming message is under the category of "raw-input"
    // NOTE: for more information see variable scope / "scoping"
    case WM_INPUT: {
      // get the raw input data from `lParam`
      HRAWINPUT hRawInput = (HRAWINPUT)lParam;
      // the actual mouse buttons data
      RAWINPUT raw;
      // size of the raw mouse buttons data
      UINT size = sizeof(RAWINPUT);

      // function to be able to extract the necessary data from the mouse
      UINT cbSize = GetRawInputData(
          hRawInput,
          RID_INPUT,
          &raw,
          &size,
          sizeof(RAWINPUTHEADER)
      );

      // check if `GetRawInputData` failed
      if (cbSize == (UINT)-1) {
          printf("Failed to get raw input data! Error: %lu\n", GetLastError());
          return 0;
      }

      // use the `GetCursorPos` function to get the coordinates
      if (GetCursorPos(&coordinates)) {
        // NOTE: only display coordinates if and only if its different
        if (coordinates.x != last_x || coordinates.y != last_y) {
          // NOTE: no need to display the coordinates anymore as we are writing to file
          // printf("x: %ld, y: %ld\n", coordinates.x, coordinates.y);

          // update the global variable for the last coodinates found
          last_x = coordinates.x;
          last_y = coordinates.y;
        }

      } else {
        printf("\n Failed To Retrive Cursor Coordinates! Error: %lu", GetLastError());
      }

      // main mouse buttons temporary variable
      int left = 0, right = 0, middle = 0;
      // side mouse buttons temporary variable
      int back = 0, forward = 0;
      // scroll wheel temporary variable
      int scroll_up = 0, scroll_down = 0;

      // get the actual mouse buttons data
      USHORT flags = raw.data.mouse.usButtonFlags;
      if (flags & RI_MOUSE_LEFT_BUTTON_DOWN) left = 1;
      if (flags & RI_MOUSE_RIGHT_BUTTON_DOWN)  right = 1;
      if (flags & RI_MOUSE_MIDDLE_BUTTON_DOWN) middle = 1;
      if (flags & RI_MOUSE_BUTTON_4_DOWN) back = 1;
      if (flags & RI_MOUSE_BUTTON_5_DOWN) forward = 1;

      // scroll wheel data logging
      // INFO: given that we have positive and negative values depending on which
      // direction the user is scrolling the scroll-wheel ( see official docs )
      // we are going to have to add an `if` statement to check for that "value"
      if (flags & RI_MOUSE_WHEEL) {
        // get the data from the scroll well ( through the inner structure )
        // INFO: again for more information ask LLM or see docs
        SHORT scroll = (SHORT)raw.data.mouse.usButtonData;

        // if scroll wheel if moving away from user ==> up
        if (scroll > 0) {
          scroll_up = 1;
          // if scroll wheel if moving towards from user ==> down
        } else if (scroll < 0) {
          scroll_down = 1;
        }
      }

      // write the mouse data to the CSV file we created
      fprintf(csv_file, "%ld,%ld,%d,%d,%d,%d,%d,%d,%d\n", coordinates.x, coordinates.y, left, right, middle, back, forward, scroll_up, scroll_down);

      // NOTE: `return 0` so that we confirm that we handle this message
      return 0;

    }

    // for all the other message; let `DefWindowProcW` handle it for us
    default:
      return DefWindowProcW(hwnd, msg, wParam, lParam);
  }
}

// our main function
int main(int argc, char *argv[]) {
  // variable to keep the elapsed time
  double elapsed_time = 0;

  // declare variables for the timer
  LARGE_INTEGER frequency;
  LARGE_INTEGER start, current;

  // initialize `HiddenWindowClass` at runtime
  // INFO: doing it in this way to avoid declaration errors
  HiddenWindowClass.cbSize        = sizeof(WNDCLASSEXW);
  HiddenWindowClass.lpfnWndProc   = WindowProc;
  HiddenWindowClass.hInstance     = GetModuleHandleW(NULL);
  HiddenWindowClass.lpszClassName = L"RawInputWindow";

  // actually register the window class for subsequent use
  if (!RegisterClassExW(&HiddenWindowClass)) {
    printf("Failed to register window class! Error: %lu\n", GetLastError());
    return 1;
  }

  // create a window ==> pass "correct" parameters to make it hidden
  hwnd = CreateWindowExW(
    0,
    L"RawInputWindow",
    L"",
    0,
    0,
    0,
    0,
    0,
    HWND_MESSAGE,
    NULL,
    GetModuleHandleW(NULL),
    NULL
  );

  // check if window creation is successfull or not
  if (hwnd == NULL) {
    printf("Failed to create window! Error: %lu\n", GetLastError());
    return 1;
  }

  // configure the raw input device structure to target mouse input
  rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
  rid.usUsage     = HID_USAGE_GENERIC_MOUSE;
  rid.dwFlags     = RIDEV_INPUTSINK;
  rid.hwndTarget  = hwnd;

  // actually register the raw input devices ( in our case mouse devices )
  // NOTE: pass the struct, the number of devies to register and size of struct
  if (!RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE))) {
    printf("Failed to register raw input device! Error: %lu\n", GetLastError());
    return 1;
  }

  // initialise the message structure's data to '0'
  MSG message = {0};

  // get the number of ticks that makes up one second from CPU
  QueryPerformanceFrequency(&frequency);

  // get the actual time stamp for the start time
  QueryPerformanceCounter(&start);

  // open the CSV file to write to --> start from scratch each program run
  csv_file = fopen("mouse_data.csv", "w");

  // check if the file has been able to open
  if (csv_file == NULL) {
    printf("Failed to open CSV file for writing!\n");

    // return with an error
    return 1;
  }

  // write the header for the CSV file
  fprintf(csv_file, "x,y,left,right,middle,back,forward,scroll_up,scroll_down\n");

  // iterate through the `while` loop indefinitely
  while (1) {
    // get the messages from the queue
    while (PeekMessageW(&message, hwnd, 0, 0, PM_REMOVE)) {
      // send that message to the hidden window for processing
      // whereby our mouse raw data will be handled
      DispatchMessageW(&message);
    }

    // get the actual time stamp for the current time
    QueryPerformanceCounter(&current);

    // calculate the elapsed time
    elapsed_time = (double)(current.QuadPart - start.QuadPart) / frequency.QuadPart;

    // calaculate the remaining time before the program ends
    double remaining = TIME_TO_RUN - elapsed_time;

    printf("\rRecording mouse data... Time remaining: %.1f seconds ", remaining);

    // NOTE: flushing directly to the screen as we are inside a `while` loop
    fflush(stdout);

    // check if the time to "leave" has been reached!
    if (elapsed_time >= TIME_TO_RUN) {
      // `break` to "exit" the `while` loop
      break;
    }

    // sleep the program for 50 miliseconds to avoid large CSV files
    Sleep(50);
  }

  // change the --> for aesthetic purposes
  printf("\n");

  // close the file to free up memory
  fclose(csv_file);

  // call the `graphs.exe` created by `pyinstaller` from the `main.py` file
  // INFO: this calls the `graphs.exe` file which is going to search
  // for our CSV file and then be able to generate the graphs in our downloads folder!

  // find the size of the structure / array `si` --> for the startup information
  si.cb = sizeof(si);

  // create a new process and run `graphs.exe` in that process
  if (!CreateProcessW(
    // name of process / executable to run
    L"graphs.exe",
    NULL,
    NULL,
    NULL,
    // does not inherit any handles
    FALSE,
    // no creation flags has been passed
    0,
    NULL,
    NULL,
    // start-up information
    &si,
    // process information
    &pi
  )) {
    // meaning that the program `graphs.exe` was not found or something
    printf("\nFailed To Launch `graphs.exe`! Error: %lu\n", GetLastError());

  } else {
    // meaning that our `graphs.exe` as found ==> wait for it to complete
    WaitForSingleObject(pi.hProcess, INFINITE);

    // close the handle for the new process
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
  }

  // destroy the hidden window that we created
  DestroyWindow(hwnd);
  // unregister the window class
  UnregisterClassW(L"RawInputWindow", GetModuleHandleW(NULL));

  printf("Program Exiting... Thank You!\n");

  return 0;
}
