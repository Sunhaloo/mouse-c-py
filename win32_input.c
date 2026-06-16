#include "win32_input.h"

#include <pathcch.h>
#include <stdio.h>
#include <hidusage.h>
#include <windows.h>

// macro definitions
#define CSV_FILENAME "mouse_data.csv"
#define CSV_FILENAME_W L"mouse_data.csv"
#define PYTHON_GRAPHS_FILENAME_W L"graphs.exe"
#define CSV_POLLING_WRITE_INTERVAL 50
#define CSV_FLUSH_WRITE_COUNT 500

// declare and define share global variables for `gui.c` file
volatile int g_is_logging = 0;
volatile double g_elapsed_time = 0.0;
volatile float g_logging_duration = 60.0f;
volatile int g_win32_ready = 0;

// declare private global variables specifically for `win32_input.c` file
static HWND hwnd = NULL;
static LONG last_x = 0;
static LONG last_y = 0;
static WNDCLASSEXW HiddenWindowClass = {0};
static RAWINPUTDEVICE rid;
static POINT coordinates;
static FILE *csv_file = NULL;
static LARGE_INTEGER frequency;
static LARGE_INTEGER start, current;
static HANDLE thread_handle = NULL;

// function to handle all non-raw-input message
static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  // using `switch ... case` as it the standard and is natural for working with
  // 'Win32' additionally, given how 'Win32' was built --> make sense to use
  // `switch ... case` so that the readability improves compared to using `if
  // ... else` statements
  switch (msg) {
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
        fprintf(stderr, "\nFailed to get raw input data! Error: %lu\n", GetLastError());
        fflush(stderr);
        return 0;
      }

      // use the `GetCursorPos` function to get the coordinates
      if (GetCursorPos(&coordinates)) {
        // NOTE: only display coordinates if and only if its different
        if (coordinates.x != last_x || coordinates.y != last_y) {
          // NOTE: no need to display the coordinates anymore as we are writing to file
          // printf("x: %ld, y: %ld\n", coordinates.x, coordinates.y);

          // update the global variable for the last coordinates found
          last_x = coordinates.x;
          last_y = coordinates.y;
        }
      } else {
        fprintf(stderr, "\nFailed to retrieve cursor coordinates! Error: %lu\n", GetLastError());
        fflush(stderr);
      }

      // main mouse buttons temporary variable
      int left = 0, right = 0, middle = 0;
      // side mouse buttons temporary variable
      int back = 0, forward = 0;
      // scroll wheel temporary variable
      int scroll_up = 0, scroll_down = 0;

      // get the actual mouse buttons data
      USHORT flags = raw.data.mouse.usButtonFlags;
      if (flags & RI_MOUSE_LEFT_BUTTON_DOWN)
        left = 1;
      if (flags & RI_MOUSE_RIGHT_BUTTON_DOWN)
        right = 1;
      if (flags & RI_MOUSE_MIDDLE_BUTTON_DOWN)
        middle = 1;
      if (flags & RI_MOUSE_BUTTON_4_DOWN)
        back = 1;
      if (flags & RI_MOUSE_BUTTON_5_DOWN)
        forward = 1;

      // scroll wheel data logging
      // INFO: given that we have positive and negative values depending on which
      // direction the user is scrolling the scroll-wheel ( see official docs )
      // we are going to have to add an `if` statement to check for that "value"
      if (flags & RI_MOUSE_WHEEL) {
        // get the data from the scroll well ( through the inner structure )
        // INFO: again for more information ask LLM or see docs
        SHORT scroll = (SHORT)raw.data.mouse.usButtonData;

        // if scroll wheel if moving away from user ==> up
        if (scroll > 0)
          scroll_up = 1;
        // if scroll wheel if moving towards from user ==> down
        else if (scroll < 0)
          scroll_down = 1;
      }

      // if we have been able to successfully open the CSV file for writing
      if (csv_file != NULL) {
        // see below for "flushing" to disk immediately
        static int write_count = 0;

        // write the mouse data to the CSV file we created
        fprintf(
          csv_file,
          "%ld,%ld,%d,%d,%d,%d,%d,%d,%d\n",
          coordinates.x,
          coordinates.y,
          left,
          right,
          middle,
          back,
          forward,
          scroll_up,
          scroll_down
        );

        // force data to disk immediately ==> in case of program crash when "logging time" is increased
        // NOTE: writing to disk immediately ( instead of the buffer ) is costly ==> flush for every 500 writes
        if (++write_count % CSV_FLUSH_WRITE_COUNT == 0) {
          // write to the disk for these `CSV_FLUSH_WRITE_COUNT` lines
          fflush(csv_file);
        }
      }

      // NOTE: `return 0` so that we confirm that we handle this message
      return 0;
    }

    // for all the other message; let `DefWindowProcW` handle it for us
    default:
      return DefWindowProcW(hwnd, msg, wParam, lParam);
    }
}

// ---------------------------------------------------------------------------
// check file existence ( private helper )
// ---------------------------------------------------------------------------

// function to check if a file is present under the current working directory
static int check_file_existence(const wchar_t *filename) {
  // part 1: get the current working directory

  // create the buffer to hold current working directory
  wchar_t getcwd_buffer[MAX_PATH];

  // try to get the current working directory by calling function
  DWORD getcwd_result = GetCurrentDirectoryW(MAX_PATH, getcwd_buffer);

  // check if we have been able to get the current working directory
  if (getcwd_result == 0) {
    // meaning we failed to get the current working directory
    fprintf(stderr, "\nFailed to get current working directory!\n");
    fflush(stderr);

    // exit the function with an error
    return 1;
  }

  // check if our ( result ) buffer is too small
  if (getcwd_result >= MAX_PATH) {
    fprintf(stderr, "\nBuffer too small for current working directory!\n");
    fflush(stderr);

    // exit the function with an error
    return 1;
  }

  // part 2: combine the paths

  // create a buffer to hold the full path
  wchar_t fullpath[MAX_PATH] = {0};

  // try to combine paths by calling function
  HRESULT hr = PathCchCombine(fullpath, MAX_PATH, getcwd_buffer, filename);

  // check if we have been able to combine the paths
  if (FAILED(hr)) {
    fprintf(stderr, "\nFailed to combine paths!\n");
    fflush(stderr);

    // exit the function with an error
    return 1;
  }

  // part 3: check if the file actually exists in the directory

  // try to check if file exists by calling function
  DWORD attrs = GetFileAttributesW(fullpath);

  // check if the file actually exists
  if (attrs == INVALID_FILE_ATTRIBUTES) {
    fprintf(stderr, "\nERROR: File does not exist!\n");
    fflush(stderr);

    // exit the function with an error
    return 1;
  }

  // part 4: check if the file is an actual file or directory

  if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
    fprintf(stderr, "\nERROR: Path is a directory!\n");
    fflush(stderr);

    // exit the function with an error
    return 1;
  }

  // path is actually a file
  return 0;
}

// ---------------------------------------------------------------------------
// input thread ( private )
// runs the hidden window and message loop on its own thread
// so it never conflicts with Raylib's message loop on the main thread
// ---------------------------------------------------------------------------

static DWORD WINAPI win32_input_thread(LPVOID lpParam) {
  (void)lpParam;

  // setup hidden window class
  HiddenWindowClass.cbSize = sizeof(WNDCLASSEXW);
  HiddenWindowClass.lpfnWndProc = WindowProc;
  HiddenWindowClass.hInstance = GetModuleHandleW(NULL);
  HiddenWindowClass.lpszClassName = L"RawInputWindow";

  // actually register the window class for subsequent use
  if (!RegisterClassExW(&HiddenWindowClass)) {
    fprintf(
      stderr,
      "\nThread: Failed to register window class! Error: %lu\n",
      GetLastError()
    );
    fflush(stderr);

    // set the global logging state to 0 ( if failed )
    g_is_logging = 0;

    // exit the function with an error
    return 1;
  }

  // create hidden message-only window
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
    fprintf(
      stderr,
      "\nThread: Failed to create window! Error: %lu\n",
      GetLastError()
    );
    fflush(stderr);

    // set the global logging state to 0 ( if failed )
    g_is_logging = 0;

    // exit the function with an error
    return 1;
  }

  // configure the raw input device structure to target mouse input
  rid.usUsagePage = HID_USAGE_PAGE_GENERIC;
  rid.usUsage = HID_USAGE_GENERIC_MOUSE;
  rid.dwFlags = RIDEV_INPUTSINK;
  rid.hwndTarget = hwnd;

  // actually register the raw input devices ( in our case mouse devices )
  // NOTE: pass the struct, the number of devies to register and size of struct
  if (!RegisterRawInputDevices(&rid, 1, sizeof(RAWINPUTDEVICE))) {
    fprintf(
      stderr,
      "\nThread: Failed to register raw input device! Error: %lu\n",
      GetLastError()
    );
    fflush(stderr);

    // set the global logging state to 0 ( if failed )
    g_is_logging = 0;

    // exit the function with an error
    return 1;
  }

  // signal to gui.c that setup is complete
  g_win32_ready = 1;

  // message loop --> runs until logging stops
  MSG message = {0};

  // iterate through the `while` loop indefinitely
  while (g_is_logging) {
    // get the messages from the queue
    while (PeekMessageW(&message, hwnd, 0, 0, PM_REMOVE)) {
      // send that message to the hidden window for processing
      // whereby our mouse raw data will be handled
      DispatchMessageW(&message);
    }

    // get the actual time stamp for the current time
    QueryPerformanceCounter(&current);

    // update the global elapsed time
    g_elapsed_time = (double)(current.QuadPart - start.QuadPart) / frequency.QuadPart;

    // check if logging duration reached
    if (g_elapsed_time >= (double)g_logging_duration) {
      // reset the global logging state
      g_is_logging = 0;
    }

    // sleep the program for 50 miliseconds to avoid large CSV files
    Sleep(CSV_POLLING_WRITE_INTERVAL);
  }

  // cleanup hidden window
  DestroyWindow(hwnd);
  hwnd = NULL;
  UnregisterClassW(L"RawInputWindow", GetModuleHandleW(NULL));

  return 0;
}

// our public functions

int win32_setup(void) {
  // nothing to do here now
  // the thread handles hidden window setup when logging starts
  return 0;
}

// start the writing process to the CSV data file
void win32_start_logging(void) {
  // open CSV file
  csv_file = fopen(CSV_FILENAME, "w");

  // if we were not able to open it up
  if (csv_file == NULL) {
    fprintf(stderr, "\nFailed to open CSV file for writing!\n");
    fflush(stderr);
    return;
  }

  // write CSV header
  fprintf(
    csv_file,
    "x,y,left,right,middle,back,forward,scroll_up,scroll_down\n"
  );

  // reset timer
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&start);

  // reset state
  g_elapsed_time = 0.0;
  g_win32_ready = 0;
  g_is_logging = 1;

  // launch input thread
  thread_handle = CreateThread(NULL, 0, win32_input_thread, NULL, 0, NULL);

  // if the thread was not able to be created
  if (thread_handle == NULL) {
    fprintf(
      stderr,
      "\nFailed to create input thread! Error: %lu\n",
      GetLastError()
    );
    fflush(stderr);

    // set the global logging state to 0 ( if failed )
    g_is_logging = 0;
  }
}

// stop the writing process to the CSV data file
void win32_stop_logging(void) {
  // signal thread to stop
  g_is_logging = 0;

  // wait for thread to finish cleanly
  if (thread_handle != NULL) {
    WaitForSingleObject(thread_handle, 3000);
    CloseHandle(thread_handle);
    thread_handle = NULL;
  }

  // close and delete incomplete CSV
  if (csv_file != NULL) {
    fclose(csv_file);
    csv_file = NULL;
  }

  // delete the CSV file
  DeleteFileW(CSV_FILENAME_W);
}

// run the `graphs.exe` executable file for heatmap generation
void win32_launch_graphs(void) {
  // wait for input thread to finish first
  if (thread_handle != NULL) {
    WaitForSingleObject(thread_handle, 5000);
    CloseHandle(thread_handle);
    thread_handle = NULL;
  }

  // close CSV file
  if (csv_file != NULL) {
    fclose(csv_file);
    csv_file = NULL;
  }

  // check both files exist
  if (check_file_existence(CSV_FILENAME_W) != 0) {
    fprintf(stderr, "\nCSV file missing - skipping graph generation!\n");
    fflush(stderr);
    return;
  }

  if (check_file_existence(PYTHON_GRAPHS_FILENAME_W) != 0) {
    fprintf(
      stderr,
      "\ngraphs.exe missing - skipping graph generation!\n"
    );
    fflush(stderr);
    return;
  }

  // launcher the `graphs.exe` executable file

  // create array for new process start-up and process information
  // INFO: again, this is created / used for the `CreateProcessW` function
  STARTUPINFOW si = {0};
  PROCESS_INFORMATION pi = {0};

  // find the size of the structure / array `si` --> for the startup information
  si.cb = sizeof(si);

  if (!CreateProcessW(
    // name of process / executable to run
    PYTHON_GRAPHS_FILENAME_W,
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
    fprintf(stderr, "\nFailed To Launch `graphs.exe`! Error: %lu\n", GetLastError());
    fflush(stderr);

  } else {
    // meaning that our `graphs.exe` as found ==> wait for it to complete
    WaitForSingleObject(pi.hProcess, INFINITE);

    // close the handle for the new process
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
  }
}

// cleaup everything that for another run
void win32_cleanup(void) {
  // ensure thread is stopped if still running
  if (thread_handle != NULL) {
    g_is_logging = 0;
    WaitForSingleObject(thread_handle, 3000);
    CloseHandle(thread_handle);
    thread_handle = NULL;
  }

  // close CSV if still open
  if (csv_file != NULL) {
    fclose(csv_file);
    csv_file = NULL;
  }
}
