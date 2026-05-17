---
id: mouse-c-py Random Notes
aliases: Random notes that I gathered when talking to Claude
tags:
  - C
  - windows
author: S.Sunhaloo
date: 2026-03-12
status: In-Progress
---

# Flushing!

```C
#include <stdio.h>
#include <string.h>
#include <windows.h>

// our main function
int main(int argc, char *argv[]) {
  char *tick_tock = "Tock";

  // iterate through the loop 5 times
  for (int i = 0; i < 5; i++) {
    // sleep the program for 1 second
    Sleep(1000);

    // display either 'Tick' or 'Tock'
    if (strcmp(tick_tock, "Tick") == 0) {
      tick_tock = "Tock";
    } else {
      tick_tock = "Tick";
    }

  printf("%s\n", tick_tock);

  }

  return 0;
```

When writing this code, Claude asked me if the output was **instant** or was it *sleeping* like its supposed to do...

The above code did run perfectly like the code indented.

But it told me something that I did **not** know about!

Using the `\n` character inside the `printf` function means that it automatically applies the `flush` function to it.

> Using `\n` means that `stdout` is **line buffered**!

> [!NOTE]
> Nevertheless, the code still *sleeps* perfectly fine even if I **remove** `\n` from the `printf` function.

> [!TIP]
> The `printf` function is similar to the `print` function found in Python ( *basically all `print` function comes from C's implementation I guess* ) whereby we obviously write to an ( *array* ) buffer first and then display on the screen!
> 
> > This is the main reason as to why Claude asked us this question!

# The `-lgdi32` Flag

This is a `gcc` flag that is we used to be able to compile things like *graphics* and *drawing* things on the screen like a **window**, **text** and other things!

Given that we are not going to be working with "*graphics*"; this means that, you **don't** need to add it to the `gcc` compilation command.

> Nevertheless, I would recommended you to add it because just for "*safety*"...

---

# From The 'Windows API For Hackers Video'

> Link to YouTube Video: https://www.youtube.com/watch?v=zqi2KE6RA38

## Common Data Types

> Official Windows Documentation: https://learn.microsoft.com/en-us/windows/win32/winprog/windows-data-types

- `HANDLE`:
	- To deal with *files*, *windows* and **devices**

> So from what I can understand we are going to be using this a lot!

- `HWND`:
	- Handle to a window $\rightarrow$ Basically the same thing as `HANDLE` just that we are instead handling actual *GUI* windows
- `HINSTANCE`:
	- Handle to an instance $\rightarrow$ Module handler
	- Identify the instance of an application / Dynamic Link Library ( DLL )
- `HMODULE`:
	- The `LoadLibrary` *function* is going to return data of **type** `HMODULE`
	- *Module* can be an **executable** or `.dll`
	- Same thing as `HINSTANCE` but specifically for *modules*
- `size_t`:
	- Basically the return value of `sizeof` function
	- Supposedly its and **unsigned** integer value
- `ULONG_PTR`:
	- **Unsigned** long pointer that is used for *precise pointers*
	- Stored the full width of a pointer
- `PVOID`:
	- A pointer to `void`
	- When we want to write generic function / APIs so that we can accept pointers to any kind of data
- `LPCVOID`:
	- Same thing as `PVOID` but in this case, its a **constant**
- `DWORD`:
	- 32-bit **unsigned** integers
	- Used a lot in the Windows API for flags, bit-fields and other general values
- `WORD`:
	- 16-bit **unsigned** integers
	- Used for smaller numerical values
- `BYTE`:
	- 8-big **unsigned** integers
	- Used for binary values, data and Boolean values
- `LPSTR`:
	- Pointer to a *null* terminated string of 8-bit of *Windows ANSI characters*
	- This means that we can change in at any point in time during program *run* **only** if actual **buffer** / **array** is on the stack / heap else not!
- `LPCSTR`:
	- Basically the same thing as `LPSTR` but instead we a `str`ing **constant**
- `LPWSTR`:
	- Pointer to a *null* terminated string of 16-bit **UNICODE** characters
	- Given that `ANSI` characters are eventually converted into `UNICODE`; better to use them as the start, I guess!
- `LPCTSTR`:
	- Pointer to **constant** *null* null terminated string of 16-bit **UNICODE** characters
- `LPCWSTR`:
	- Pointer to a *null* terminated string of 16-bit **UNICODE** characters
	- Same thing as `LPCSTR` but this time in **UNICODE**
- `LPARAM`:
	- Message parameter used in message handling functions
	- Accepts a pointer
- `WPARAM`:
	- Message parameter used in message handling functions
	- But in this case **unsigned**
- `UINT`:
	- **Unsigned** integer value
- `LONG`:
	- **Signed** 32-bit integer value
	- *Ouuhh*, used to specifying **coordinates**; which might be of value to us!

> For example and showing off purposes!

- Here is how we can use something like `ULONG_PTR`:

```C

  // declare variables and also find the address of variable
  int x = 100;
  int *x_ptr = &x;

  // get the precise address
  ULONG_PTR unlong_ptr = (ULONG_PTR)x_ptr;

  // display the "normal" and "precise" address
  printf("\nPointer For 'x': %p\n", x_ptr);
  printf("\nLong Precise Pointer: %p\n", (void *)unlong_ptr);

```

- Here is how we can use something like `LPSTR`:

```C

  // declare mutable string
  LPSTR str = "Hello World";

  printf("%s\n", str);

  str = "Very Nice";

  printf("%s\n", str);

```

## Create A File

> [!INFO] Resource(s)
> Windows File Header API: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/
> `CreateFileW` Function: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-createfilew
> `WriteFile` Function: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-writefile

```C
#include <handleapi.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <errhandlingapi.h>

// our main function
int main(int argc, char *argv[]) {
  // variable to hold the file name
  LPWSTR file_name = L"file_testing.txt";

  // create a file uisng the `CreateFileW` system call
  HANDLE file_handler = CreateFileW(
    file_name,
    (GENERIC_READ | GENERIC_WRITE),
    0,
    NULL,
    OPEN_ALWAYS,
    FILE_ATTRIBUTE_NORMAL,
    NULL
  );

  // actually use the system call
  if (file_handler == INVALID_HANDLE_VALUE) {
    printf("\nFailed To Open File! Error: %lu\n", GetLastError());

    return 1;
  }

  // write to the file

  // create the message to write to the file
  const char file_data[] = "Hello World In Windows Through Windows ( Win32) API";

  // number of bytes that we are going to write into our file
  DWORD num_of_bytes_written;

  // INFO: this is similar to the `write` system call in Linux / UNIX systems
  BOOL fileWriter = WriteFile(
    file_handler,
    file_data,
    strlen(file_data),
    &num_of_bytes_written,
    NULL
  );

  // check if we have successfully written data to the file
  if (!fileWriter) {
    printf("\nFailed To Write To File! Error: %lu\n", GetLastError());

    return 1;

  } else {
    wprintf(L"Written to file '%ls' successfully\n", file_name);
  }

  // close the file handler
  CloseHandle(file_handler);

  return 0;
}
```

> [!SUCCESS]
> We have completed watching the video and its time to move onto other things...
> 
> > I think it going to be time to actually start writing the main thing!

## Our Makefile

> Yes, I did install `make` on Windows through 'MYSYS'!

```make
program: compile run clean

compile:
	@gcc main.c -Wall -Wextra -lgdi32 -o program

run:
	@./program

clean:
	@del program.exe
```

# Windows Coordinate System

> [!INFO] Resource(s)
> - `POINT` Structure: https://learn.microsoft.com/en-us/windows/win32/api/windef/ns-windef-point
> - `GetCursorPos` Function: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getcursorpos
> - `QueryPerformanceFrequency`: https://learn.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancefrequency
> - `QueryPerformanceCounter`: https://learn.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter

## Using Pointers To `POINT` Structure and `GetCursorPos` Function

### Side-Quest: Timing Things!

There are many ways to "*get time*" in C and because there are many ways... It get difficult to pick and choose what we should use!

Therefore, I spoke a little bit with [Gemini](https://gemini.google.com) about it and after chatting we it.

I decided to use `clock_gettime` function so that we get a *fast* and **precise** way to measure time.

> [!BUG]
> `clock_gettime` function is a function that is only present on **Linux** and **Posix** compliant machines!
> 
> There are no such things such as a `clock_gettime` function on Windows!

Therefore, we are going to have to use `QueryPerformanceFrequency` and `QueryPerformanceCounter`. Whereby with the *former*, we are going to ask the CPU at what **frequency** it works so as to get the *number of ticks* that makes up 1 second. While the *latter* is going to be the one that is going to be actually "*tracking*" the time!

### Actually Coding It!

#### Testing The `GetCursorPos` Function Only

```C
#include <stdio.h>
#include <windows.h>

// our `POINT` structure
POINT coordinates;


// our main function
int main(int argc, char *argv[]) {
  // call the `GetCursorPos` function to get the current coordindates of the mouse
  // TIP: simply pass the 'point' `coordinates` and then simply access via `coordinates.x` and `coordinates.y`
  // additionally, add to `if ... else ...` statement for error handling
  if (GetCursorPos(&coordinates)) {
    printf("\nx: %d, y: %d\n", coordinates.x, coordinates.y);
  } else {
    printf("\n Failed To Retrive Cursor Coordinates! Error: %lu", GetLastError());
  }

  return 0;
```

#### Adding Continuous Mouse Logging and Timer

```C
#include <profileapi.h>
#include <stdio.h>
#include <windows.h>
#include <winnt.h>

// our `POINT` structure
POINT coordinates;


// our main function
int main(int argc, char *argv[]) {
  // variable to keep the elapsed / remaining time
  double elapsed_time = 0;
  double total_time = 0;

  // declare variables for the timer
  LARGE_INTEGER frequency;
  LARGE_INTEGER start, current;

  // get the number of ticks that makes up one second from CPU
  QueryPerformanceFrequency(&frequency);

  // get the actual time stamp for the start time
  QueryPerformanceCounter(&start);

  while (1) {
    // get the actual time stamp for the current time
    QueryPerformanceCounter(&current);

    // calculate the elapsed time
    elapsed_time = (double)(current.QuadPart - start.QuadPart) / frequency.QuadPart;

    // get the cursor's position
    if (GetCursorPos(&coordinates)) {
      printf("x: %ld, y: %ld, elapsed time: %0.2lf\n", coordinates.x, coordinates.y, elapsed_time);
    } else {
      printf("\n Failed To Retrive Cursor Coordinates! Error: %lu", GetLastError());
    }

    // check if the time to "leave" has been reached!
    if (elapsed_time >= 10.0) {
      // `exit` the program ( in this case ) instead of a `break`
      exit(0);
    }

    // sleep the program every 500 mili-seconds so that use can see output on console clearly
    Sleep(500);
  }

  return 0;
}
```

---

# Windows Raw Input

> [!INFO] Resource(s)
> - About Raw Input ( Windows Documentation ): https://learn.microsoft.com/en-us/windows/win32/inputdev/about-raw-input

> Ohh, 'HIDs' is basically the acronym for 'Human Interface Devices'!

> [!INFO] **Advantages** of 'Raw Input'
> 
> > From the official documentation...
> 
> - An application does not have to detect or open the input device.
> - An application gets the data directly from the device, and processes the data for its needs.
> - An application can distinguish the source of the input even if it is from the same type of device. For example, two mouse devices.
> - An application manages the data traffic by specifying data from a collection of devices or only specific device types.
> - HID devices can be used as they become available in the marketplace, without waiting for new message types or an updated OS to have new commands in `WM_APPCOMMAND`.

> [!WARNING] Something To Note About
> [`WM_APPCOMMAND`](https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-appcommand) does provide for some HID devices.
> 
> However, `WM_APPCOMMAND` is a **higher** level device-independent input event, while [`WM_INPUT`](https://learn.microsoft.com/en-us/windows/win32/inputdev/wm-input) sends *raw*, **low** level data that is specific to a device.

## How To Raw Input

1. No applications receives raw data from devices... Need to **first** *register* the device
2. Create array of [`RAWINPUTDEVICE`](https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawinputdevice) `struct`ure
3. Then after registration, application calls [`RegisterRawInputDevices`](https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerrawinputdevices) function
4. Read the data coming from 'Raw Input'

> [!TIP]
> An application **can** *register* a device that is **not** currently plugged into the computer system.
> 
> Then when the device gets **attached** / plugged-in; Windows ( Driver ) Manager is going to do its thing!

Some / Most new games has a toggle that allows the user to switch between **using** 'Raw Input' or turning it **off**.

This is basically a *switch* that the developers / programmer can implement in their program from passing *flags* to `RAWINPUTDEVICE`.

## Hidden Windows!

> [!INFO] Resource(s)
> - `CreateWindowExA`: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexa

Talking with [Claude](https://claude.ai), I see that to be able to use `WM_INPUT`. We are going to have to create a **window** because `WM_INPUT` passes *messages* through these windows.

Hence, given that we just need to get the raw data from the mouse and don't really need an actual 'GUI' window... We need to create a hidden window do that the *data* can **communicate**.

The *window* is **required** as `WL_INPUT` is part of the 'Win32' **message loop system** and look something like this:

```mermaid
flowchart TD
    subgraph System
        B[OS]
        C[Raw Input]
        D[Queue]
    end

    A[Hardware] --> B --> C --> D --> E[HWND] --> F[WM_INPUT]
```

> [!INFO]
> The thing that we are actually requiring is a "**message queue tie to a thread**".
> 
> This is why we need that **hidden window** to be able to communicate through that thread.

---

Now Claude gave me this and it's saying that this is basically the entire program ( skeleton )!

```console
1. Create a hidden window
        ↓
2. Register mouse as Raw Input device
        ↓
3. Start message loop (waiting for WM_INPUT)
        ↓
4. Each time mouse moves/clicked → WM_INPUT fires
        ↓
5. We extract x, y, buttons from the raw data
        ↓
6. Write to CSV
        ↓
7. Timer runs out → close window → program exits
```

---

### How To Keep Waiting For Messages and Keep Timer Running?

> [!INFO] Resource(s)
> - `PeekMessage`: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagea

- **Cannot** use `fork` as its Linux only
- `p_threads` are available in 'MingW' but are **not** native as they are emulated
	- I mean Windows does have Windows specific threads!

The answer is more simple than *forking* or making our program multi-threaded. Instead we simply just need to do something like this and use a function calls `PeekMessage`.

> Claude did gave me this answer!

```console
while (timer hasn't expired) {
    PeekMessage → if there's a WM_INPUT, handle it
    check timer
    update CSV
}
```

> [!NOTE]
> It has something to do with *blocking* and from what I can understand `PeekMessage` function is **non-blocking**!

### Peek Message Function

> [!INFO] Resource(s)
> 
> - `PeekMessagew`: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-peekmessagew

From the documentation I see that it simply checks for any thread messages that are being received.

> It will return a `BOOL` value!

Additionally, here is a little snippet of code, that I don't understand at all, from Nick Walton's code:

```C
while (!quit) {
    static MSG message = { 0 };
    
    // Process Windows messages
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
        DispatchMessage(&message);
    }

    static int keyboard_x = 0, keyboard_y = 0;

    // Movement Input
    if (keyboard[VK_RIGHT] || keyboard['D']) ++keyboard_x;
    if (keyboard[VK_LEFT]  || keyboard['A']) --keyboard_x;
    if (keyboard[VK_UP]    || keyboard['W']) ++keyboard_y;
    if (keyboard[VK_DOWN]  || keyboard['S']) --keyboard_y;

    // Boundary Checks (Clamping)
    if (keyboard_x < 0)            keyboard_x = 0;
    if (keyboard_x > frame.w - 1)  keyboard_x = frame.w - 1;
    if (keyboard_y < 0)            keyboard_y = 0;
    if (keyboard_y > frame.h - 1)  keyboard_y = frame.h - 1;

    // Visual Effects
    // 1. Draw 1000 random black pixels (noise)
    for (int i = 0; i < 1000; ++i) {
        frame.pixels[rand32() % (frame.w * frame.h)] = 0;
    }

    // 2. Draw white pixel at keyboard position
    frame.pixels[keyboard_x + keyboard_y * frame.w] = 0x00ffffff;

    // 3. Draw white pixel at mouse position if left-clicked
    if (mouse.buttons & MOUSE_LEFT) {
        frame.pixels[mouse.x + mouse.y * frame.w] = 0x00ffffff;
    }

    // Trigger window redraw
    InvalidateRect(window_handle, NULL, FALSE);
    UpdateWindow(window_handle);
}
```

Where I see that the `PeekMessagew` function itself is found **inside** a `while` loop that keeps on checking for the received messages.

The above code snippet was inside a `WinMain` function. But we are not interested in making an actual window now!

Additionally, we know that as `PeekMessage` is non-blocking making it the ideal thing for us!

---

# Defining And Creating A Hidden Window

> [!INFO] Resource(s)
> 
> - `tagWNDCLASSEXA`: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexa
> - `DefWindowProcW`: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-defwindowprocw
> - `WNDCLASSEXW`: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-wndclassexw
> - `RegisterClassExW`: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerclassexw
> - `CreateWindowExW`: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-createwindowexw
> - `RAWINPUTDEVICE`: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawinputdevice
> - `RegisterRawInputDevices`: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-registerrawinputdevices

## Create Callback Function To Handle Messages

> Created the following code with the help of ChatGPT

> [!BUG] I know nothing!
> 
> Yes, I know nothing about this shit and I just want to make something...
> 
> Therefore, you could say that I am following a tutorial from these large language models!

```C
// function to handle all non-raw-input message
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  // using `switch ... case` as it the standard and is natural for working with 'Win32'
  // additionally, given how 'Win32' was built --> make sense to use `switch ... case`
  // so that the readability improves compared to using `if ... else` statements
  switch(msg) {
    // if the incoming message is under the category of "raw-input"
    case WM_INPUT:
      printf("Raw Input Message Detected!\n");

      // NOTE: `return 0` so that we confirm that we handle this message
      return 0;

    // for all the other message; let `DefWindowProcW` handle it for us
    default:
      return DefWindowProcW(hwnd, msg, wParam, lParam);
  }
}
```

> [!TIP] Where does these `DefWindowProcW` parameters comes from?
> 
> > [!NOTE]
> > This answer is from Claude!
> 
> ```C
> // code snippet from Nick Walton!
> while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
>     DispatchMessage(&message);
> }
> ```
> 
> `DispatchMessage` is the answer. When you call `DispatchMessage`, Windows looks at the message, figures out which window it belongs to, and then **calls your `WindowProc` function directly** passing in those four parameters automatically.
> 
> So the chain is:
> 
> ```
> PeekMessage → finds a message
>     ↓
> DispatchMessage → calls WindowProc for you
>     ↓
> WindowProc → you handle it
> ```

## Registration Of ( GUI ) Windows

### Create Structure Holding Hidden Window Info

```C
// structure that contains information for our hidden window
// NOTE: this global structure is defined at the top
// who would have fucking thought!
WNDCLASSEXW HiddenWindowClass;

// our main function
int main(int argc, char *argv[]) {
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

  return 0;
}
```

## Create The Hidden Window

To create the hidden window, we just to update `main` so that we use the `CreateWindowExW` function to create the window and by passing *correct* parameters... We should be able to make it hidden

> For more information about the function; please refer to the link in the resources section found above ( for this heading )!

```C
// global handle to our hidden window
// again, this is found at the top of our file
HWND hwnd;

// here is our updated `main` function

// our main function
int main(int argc, char *argv[]) {
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

  return 0;
}
```

## Registration Of Raw Input Devices

We are now going to configure the `RAWINPUTDEVICE` `struct`ure so that we are able to register actual devices to the window that we created above!

> From the documentation, here it how the structure looks like:

```C
typedef struct tagRAWINPUTDEVICE {
  USHORT usUsagePage;
  USHORT usUsage;
  DWORD  dwFlags;
  HWND   hwndTarget;
} RAWINPUTDEVICE, *PRAWINPUTDEVICE, *LPRAWINPUTDEVICE;
```

We are interested in `usUsagePage` ( *for generic desktop device* ) and also `usUsage` ( *specifically need to be set for the mouse* ).

From the documentation, I see that `usUsagePage` is going to accept a top-level collection which is:

> "a grouping of functionality that targets a particular software consumer (or type of consumer) of the functionality. For example, a top-level collection can be described as keyboard, mouse, consumer control, sensor, display, and so on" [top-level-collections](https://learn.microsoft.com/en-us/windows-hardware/drivers/hid/top-level-collections)

For the `dwFlags`, as we need our program to have this flow:

```console
User clicks on a game -> our program loses focus -> we STILL receive mouse data
```

Therefore, if you go and take a look at the documentation, you are going to see `RIDEV_INPUTSINK` and it says that:

> "If set, this enables the caller to receive the input even when the caller is not in the foreground. Note that `hwndTarget` must be specified."

And for the `hwndTarget` its the above `hwnd` - `CreateWindowExW` window that we should supply it with!

```C
// raw input device registration structure
// again, this is found at the top of our file
RAWINPUTDEVICE rid;

// I am now only going to show you part of the inside
// of the main function

// our main function
int main(int argc, char *argv[]) {

  // other codes found here

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

  return 0;
}
```

### Actually Register The Devices

Now, we are going to use the `RegisterRawInputDevices` to actually register the devices that *supplies* "raw" data.

```C
// our main function
int main(int argc, char *argv[]) {
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

  return 0;
}
```

### Message Loop - For Information Passing And Processing

We are now going to add this little snippet of code:

```C
MSG message = {0};

while (1) {
    while (PeekMessageW(&message, hwnd, 0, 0, PM_REMOVE)) {
        DispatchMessageW(&message);
    }
```

The above code is also found in Nick Walton's code ( *which was the main inspiration for this project* )!

> Link To Blog: https://croakingkero.com/tutorials/input_win32/

Therefore, we are going to add the above line so that we are able to *get* the "raw-input" *enabled* devices **data** and therefore use that data from the **hidden** window for processing.

## Great Success

> You could say that this is a milestone!

Here is the full code that allow us to see / get the data from a "raw-input" enabled device and our hidden window then processes the data!

```C
#include <stdio.h>
#include <windows.h>
#include <hidusage.h>


// global handle to our hidden window
HWND hwnd;

// forward declaration of WindowProc
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// structure that contains information for our hidden window
WNDCLASSEXW HiddenWindowClass;

// raw input device registration structure
RAWINPUTDEVICE rid;

// function to handle all non-raw-input message
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  // using `switch ... case` as it the standard and is natural for working with 'Win32'
  // additionally, given how 'Win32' was built --> make sense to use `switch ... case`
  // so that the readability improves compared to using `if ... else` statements
  switch(msg) {
    // if the incoming message is under the category of "raw-input"
    case WM_INPUT:
      printf("Raw Input Message Detected!\n");

      // NOTE: `return 0` so that we confirm that we handle this message
      return 0;

    // for all the other message; let `DefWindowProcW` handle it for us
    default:
      return DefWindowProcW(hwnd, msg, wParam, lParam);
  }
}

// our main function
int main(int argc, char *argv[]) {
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

  // iterate through the `while` loop indefinitely
  while (1) {
    // get the messages from the queue
    while (PeekMessageW(&message, hwnd, 0, 0, PM_REMOVE)) {
      // send that message to the hidden window for processing
      // whereby our mouse raw data will be handled
      DispatchMessageW(&message);
    }
  }

  return 0;
}
```

# Coordinates and Mouse Buttons!

> [!INFO] Resource(s)
> 
> - `RAWINPUT`:https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawinput
> - `RAWMOUSE`: https://learn.microsoft.com/en-us/windows/win32/api/winuser/ns-winuser-rawmouse

We are now ready to actually implement the part whereby we are able to get the coordinates of the mouse!

From the documentation I see that we have this structure:

```C
typedef struct tagRAWINPUT {
  RAWINPUTHEADER header;
  union {
    RAWMOUSE    mouse;
    RAWKEYBOARD keyboard;
    RAWHID      hid;
  } data;
} RAWINPUT, *PRAWINPUT, *LPRAWINPUT;
```

From what I can understand we have a main header ( *IDK what is actually is and does* ) and additionally, we have pass a **union** of *structures*... Given that its a union, we can simply only add `RAWMOUSE` structure!

Additionally, from what I can see from the `RAWMOUSE` structure:

```C
typedef struct tagRAWMOUSE {
  USHORT usFlags;
  union {
    ULONG ulButtons;
    struct {
      USHORT usButtonFlags;
      USHORT usButtonData;
    } DUMMYSTRUCTNAME;
  } DUMMYUNIONNAME;
  ULONG  ulRawButtons;
  LONG   lLastX;
  LONG   lLastY;
  ULONG  ulExtraInformation;
} RAWMOUSE, *PRAWMOUSE, *LPRAWMOUSE;
```

Given that we *want* the **coordinates** and mouse **buttons**... We are going to need all the buttons related data and specifically `lLastX` and `lLastY`.

> [!WARNING] Relative V/S Absolute Coordinates
> 
> Apparently, the raw input data / coordinates does not give us the actual position like `GetCursorPos` does!
> 
> Therefore, we are going to have to convert our program so that we can combine both of these *technologies* so that we get and accurate but **absolute positioning** of the cursor to get real `x` and `y` coordinates!

## Coordinate - Get Cursor Position Function

This is only a snippet of the code where we modify it to add the `GetCursorPos` function.

> Will add the full code below!

```C
// at the top of our file
// our point structure
POINT coordinates;

  switch(msg) {
    // if the incoming message is under the category of "raw-input"
    case WM_INPUT:

      // use the `GetCursorPos` function to get the coordinates
      if (GetCursorPos(&coordinates)) {
        printf("x: %ld, y: %ld\n", coordinates.x, coordinates.y);
      } else {
        printf("\n Failed To Retrive Cursor Coordinates! Error: %lu", GetLastError());
      }

      // NOTE: `return 0` so that we confirm that we handle this message
      return 0;

    // for all the other message; let `DefWindowProcW` handle it for us
    default:
      return DefWindowProcW(hwnd, msg, wParam, lParam);
  }
```

> Its basically the same as what we did in '[[#Actually Coding It!]]'

If you go ahead and run the program / our `main.c` file. You are going to see... Well nothing!

> Until you **move** your mouse ( *apparently it also works on touchpad. Its a pointing device so...* )!

Now, even the smallest movement that you are going to make its going to output all the tiny little change in the `x` and `y` coordinates.

> See where I am going with it!

### Huge CSV Files!?!

Well given that we are going to be writing to a `.csv` file so our Python program is able to extract the data using the **built-in** `csv` and "*installed*" matplotlib module.

Whenever we move, even a tiny movement; its going to output / get the coordinates. Therefore, the `.csv` file is going to be massive and I don't want that!

Given that we are able to process huge amount of data using the CSV module... I want to keep things small and also fast

> Well, if you process small amounts of data; you are going to be faster... Duh!

Therefore, I am thinking of adding a little `Sleep` to the program.

Hence, modify the dispatcher part of the program and add the little `Sleep` function there!

```C
  // iterate through the `while` loop indefinitely
  while (1) {
    // get the messages from the queue
    while (PeekMessageW(&message, hwnd, 0, 0, PM_REMOVE)) {
      // send that message to the hidden window for processing
      // whereby our mouse raw data will be handled
      DispatchMessageW(&message);
    }

    // sleep the program for 50 miliseconds to avoid large CSV files
    Sleep(50);
  }
```

Now, you are going to see that the now the output is like so:

> I do feel that 50 miliseconds of delay!

```console
x: 848, y: 574
x: 848, y: 574
x: 848, y: 574
x: 848, y: 573
x: 848, y: 573
x: 848, y: 572
x: 848, y: 572
x: 848, y: 572
x: 848, y: 570
x: 848, y: 570
x: 848, y: 570
```

### More Problem...

Take a look again at the above output, you are going to see see that for 1 `x` and `y`. Its appearing 3 times!

```console
x: 848, y: 574
x: 848, y: 574
x: 848, y: 574
```

Therefore, Claude told me that we need to show only *record* a coordinate that is **different** from *itself*...

```C
#include <stdio.h>
#include <windows.h>
#include <hidusage.h>


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

// function to handle all non-raw-input message
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  // using `switch ... case` as it the standard and is natural for working with 'Win32'
  // additionally, given how 'Win32' was built --> make sense to use `switch ... case`
  // so that the readability improves compared to using `if ... else` statements
  switch(msg) {
    // if the incoming message is under the category of "raw-input"
    case WM_INPUT:

      // use the `GetCursorPos` function to get the coordinates
      if (GetCursorPos(&coordinates)) {
        // NOTE: only display coordinates if and only if its different
        if (coordinates.x != last_x || coordinates.y != last_y) {
          printf("x: %ld, y: %ld\n", coordinates.x, coordinates.y);

          // update the global variable for the last coodinates found
          last_x = coordinates.x;
          last_y = coordinates.y;
        }

      } else {
        printf("\n Failed To Retrive Cursor Coordinates! Error: %lu", GetLastError());
      }

      // NOTE: `return 0` so that we confirm that we handle this message
      return 0;

    // for all the other message; let `DefWindowProcW` handle it for us
    default:
      return DefWindowProcW(hwnd, msg, wParam, lParam);
  }
}

// our main function
int main(int argc, char *argv[]) {
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

  // iterate through the `while` loop indefinitely
  while (1) {
    // get the messages from the queue
    while (PeekMessageW(&message, hwnd, 0, 0, PM_REMOVE)) {
      // send that message to the hidden window for processing
      // whereby our mouse raw data will be handled
      DispatchMessageW(&message);
    }

    // sleep the program for 50 miliseconds to avoid large CSV files
    Sleep(50);
  }

  return 0;
}
```

The above code is going to now check for the coordinates and if the last coordinates is the same then its going to reject it but if we have a different coordinates then its going to display.

---

> [!NOTE]
> Before, we go to the file writing, there are still many things to be done!
> 
> Here are some of the things that needs to be done:
> 
> - Add timer ( so that program does not run indefinitely )
> - Actually destroy the hidden window
> - Log the mouse button's number of times they have been clicked
> 
> > Therefore, let's tackle these first and then we move on to the file writing part!

---

## Adding Timer!

Given that we already know what we can use to implement a time, we just need to place these functions ( *i.e `QueryPerformanceFrequence` and `QuertyPerformanceCounter`* ) so that we can actually close the program after a certain time limit!

Additionally, we are going to add the *destroy* function to actually free up the hidden window from memory.

> I have written the full program code again!

```C
#include <profileapi.h>
#include <stdio.h>
#include <hidusage.h>
#include <windows.h>
#include <winnt.h>

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

// function to handle all non-raw-input message
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
  // using `switch ... case` as it the standard and is natural for working with 'Win32'
  // additionally, given how 'Win32' was built --> make sense to use `switch ... case`
  // so that the readability improves compared to using `if ... else` statements
  switch(msg) {
    // if the incoming message is under the category of "raw-input"
    case WM_INPUT:

      // use the `GetCursorPos` function to get the coordinates
      if (GetCursorPos(&coordinates)) {
        // NOTE: only display coordinates if and only if its different
        if (coordinates.x != last_x || coordinates.y != last_y) {
          printf("x: %ld, y: %ld\n", coordinates.x, coordinates.y);

          // update the global variable for the last coodinates found
          last_x = coordinates.x;
          last_y = coordinates.y;
        }

      } else {
        printf("\n Failed To Retrive Cursor Coordinates! Error: %lu", GetLastError());
      }

      // NOTE: `return 0` so that we confirm that we handle this message
      return 0;

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

    // check if the time to "leave" has been reached!
    if (elapsed_time >= 10.0) {
      // `break` to "exit" the `while` loop
      break;
    }

    // sleep the program for 50 miliseconds to avoid large CSV files
    Sleep(50);
  }

  // destroy the hidden window that we created
  DestroyWindow(hwnd);
  // unregister the window class
  UnregisterClassW(L"RawInputWindow", GetModuleHandleW(NULL));

  return 0;
}
```

> [!SUCCESS]
> The code now logs the mouse input when its ( *the mouse* ) moving and also actually destroys and un-registers the ( *hidden* ) window!

## Mouse Buttons Logging

Remember this `struct`ure that we used for our raw-input mouse definition?

```C
typedef struct tagRAWMOUSE {
  USHORT usFlags;
  union {
    ULONG ulButtons;
    struct {
      USHORT usButtonFlags;
      USHORT usButtonData;
    } DUMMYSTRUCTNAME;
  } DUMMYUNIONNAME;
  ULONG  ulRawButtons;
  LONG   lLastX;
  LONG   lLastY;
  ULONG  ulExtraInformation;
} RAWMOUSE, *PRAWMOUSE, *LPRAWMOUSE;
```

> This is original found under the '[[#Coordinates and Mouse Buttons!]]' heading!

Well, here we are interested in the smaller `struct` that contains the `usButtonFlags` ( *which is used to handle the actual mouse buttons* ) and also `usButtonData` ( *which is going to be used to handle scrolling data* ).

Claude is telling me to look further into it and there are some constants that we need to first understand.

## Get Raw Input Data

> [!INFO] Resource(s)
> 
> - `GetRawInputData`: https://learn.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getrawinputdata


Therefore, we are going to have to update our `case WM_INPUT` to be like this:

```C
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
          printf("x: %ld, y: %ld\n", coordinates.x, coordinates.y);

          // update the global variable for the last coodinates found
          last_x = coordinates.x;
          last_y = coordinates.y;
        }

      } else {
        printf("\n Failed To Retrive Cursor Coordinates! Error: %lu", GetLastError());
      }

      // get the actual mouse buttons data
      USHORT flags = raw.data.mouse.usButtonFlags;
      if (flags & RI_MOUSE_LEFT_BUTTON_DOWN)   printf("LEFT CLICK\n");
      if (flags & RI_MOUSE_RIGHT_BUTTON_DOWN)  printf("RIGHT CLICK\n");
      if (flags & RI_MOUSE_MIDDLE_BUTTON_DOWN) printf("MIDDLE CLICK\n");
      if (flags & RI_MOUSE_BUTTON_4_DOWN) printf("BACK BUTTON\n");
      if (flags & RI_MOUSE_BUTTON_5_DOWN) printf("FORWARD BUTTON\n");

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
          printf("SCROLL UP\n");
          // if scroll wheel if moving towards from user ==> down
        } else if (scroll < 0) {
          printf("SCROLL DOWN\n");
        }
      }

      // NOTE: `return 0` so that we confirm that we handle this message
      return 0;

    }
```

> [!NOTE]
> Notice that *special* syntax that we used after the `case WM_INPUT:` whereby we wrapped everything inside of `{}`?
> 
> This is **not** a syntax only for Windows... Its just a standard C syntax and its used when we need to declare *stuff* inside a `case` statement!

# File Writing

Given that its now that I am going to learn about the CSV and Matplotlib module in Python... I did ask Claude some questions as to how many files we should write ( *like one for coordinates and another one for mouse buttons* ) and how we should proceed.

For the number of file that we need to create; its telling me that I should create a **single** file only and it inside, it will look something like this:

```csv
x,y,left,right,middle,back,forward,scroll_up,scroll_down
956,432,0,0,0,0,0,0,0
956,432,1,0,0,0,0,0,0
```

Additionally, we have to think about how we are actually going to implement the file writing!

Given that in my C University Labsheets whereby we are using UNIX / LINUX system calls like `open`, `write` and others... I don't want to use that here!

> Because from my point of view, its overkill...

I want to use functions like `fopen` and other things as they are C-standard functions for file operation on any Operating System!

> I am going to be showing to the full `main.c` file again because I think it better for the context and you also see how it changes over time.

## Opening and Writing The Header Only

In the iteration of the code, we are only going to open the `mouse_data.csv` file, check if it has been opened successfully, write the header of our needs and simply close the file!

```C
#include <errhandlingapi.h>
#include <minwindef.h>
#include <profileapi.h>
#include <stdio.h>
#include <hidusage.h>
#include <windows.h>
#include <winnt.h>

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
          printf("x: %ld, y: %ld\n", coordinates.x, coordinates.y);

          // update the global variable for the last coodinates found
          last_x = coordinates.x;
          last_y = coordinates.y;
        }

      } else {
        printf("\n Failed To Retrive Cursor Coordinates! Error: %lu", GetLastError());
      }

      // get the actual mouse buttons data
      USHORT flags = raw.data.mouse.usButtonFlags;
      if (flags & RI_MOUSE_LEFT_BUTTON_DOWN)   printf("LEFT CLICK\n");
      if (flags & RI_MOUSE_RIGHT_BUTTON_DOWN)  printf("RIGHT CLICK\n");
      if (flags & RI_MOUSE_MIDDLE_BUTTON_DOWN) printf("MIDDLE CLICK\n");
      if (flags & RI_MOUSE_BUTTON_4_DOWN) printf("BACK BUTTON\n");
      if (flags & RI_MOUSE_BUTTON_5_DOWN) printf("FORWARD BUTTON\n");

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
          printf("SCROLL UP\n");
          // if scroll wheel if moving towards from user ==> down
        } else if (scroll < 0) {
          printf("SCROLL DOWN\n");
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

    // check if the time to "leave" has been reached!
    if (elapsed_time >= 10.0) {
      // `break` to "exit" the `while` loop
      break;
    }

    // sleep the program for 50 miliseconds to avoid large CSV files
    Sleep(50);
  }

  // close the file to free up memory
  fclose(csv_file);

  // destroy the hidden window that we created
  DestroyWindow(hwnd);
  // unregister the window class
  UnregisterClassW(L"RawInputWindow", GetModuleHandleW(NULL));

  return 0;
}
```

## Writing Data To The File

Here, I am just going to show you where we have made changes to instead of writing the whole file as we already have the "*boilerplate*" from above.

```C
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
          printf("x: %ld, y: %ld\n", coordinates.x, coordinates.y);

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
```

## Showing Remaining Time On Console

Now, the thing that I need to say its that; on the final... I do mean "*final*" product when the user is going to have a GUI and other things... I think the user should simply see a progress bar of some sort.

But for now, given that we are already logging the **both** the *coordinates* and also the *mouse button clicks* in our `mouse_data.csv` file. I just want to user to see the remaining time left before the program is going to close!

> Therefore, update this part of the `main.c` file like so:

```C
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
    double remaining = 10.0 - elapsed_time;

    printf("\rRecording mouse data... Time remaining: %.1f seconds  ", remaining);

    // NOTE: flushing directly to the screen as we are inside a `while` loop
    fflush(stdout);

    // check if the time to "leave" has been reached!
    if (elapsed_time >= 10.0) {
      // `break` to "exit" the `while` loop
      break;
    }

    // sleep the program for 50 miliseconds to avoid large CSV files
    Sleep(50);
  }
```

# 3/04/2026 - Current Iteration

Here is the full code that I have inside my `main.c` file.

Writing this here because, we are going to move onto the Python script to generate the graphs!

```C
#include <errhandlingapi.h>
#include <minwindef.h>
#include <profileapi.h>
#include <stdio.h>
#include <hidusage.h>
#include <windows.h>
#include <winnt.h>

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
    double remaining = 10.0 - elapsed_time;

    printf("\rRecording mouse data... Time remaining: %.1f seconds ", remaining);

    // NOTE: flushing directly to the screen as we are inside a `while` loop
    fflush(stdout);

    // check if the time to "leave" has been reached!
    if (elapsed_time >= 10.0) {
      // `break` to "exit" the `while` loop
      break;
    }

    // sleep the program for 50 miliseconds to avoid large CSV files
    Sleep(50);
  }

  // close the file to free up memory
  fclose(csv_file);

  // destroy the hidden window that we created
  DestroyWindow(hwnd);
  // unregister the window class
  UnregisterClassW(L"RawInputWindow", GetModuleHandleW(NULL));

  printf("\n\nProgram Exiting... Thank You!\n");

  return 0;
}
```

---

# Python Implementation

> [The more you fuck around the more you are going to find out](https://www.youtube.com/watch?v=_qEcm43Lx3c)

Therefore, instead of taking elaborate notes for this one... I am going to take a leap of faith and learn what I only need to learn to complete this instead of first going to learn the entire `csv` module for example.

This is also means that I am going to fail and I need to fail a lot to be able to learn more!

## Initial Python Code

> [!INFO] Resource(s)
> 
> - Bar Chart: https://matplotlib.org/stable/gallery/lines_bars_and_markers/bar_colors.html
> - Hist2D Chart / Graph: https://matplotlib.org/stable/plot_types/stats/hist2d.html

```python
# import `path` from the `os` module
from os import path

# import the entire pandas library and use `pd` as alias
import pandas as pd

# import the matplotlib 2D plots to make the bar chart and heatmap graph
import matplotlib.pyplot as plt

# global constants
FILE_NAME: str = "mouse_data.csv"
BAR_CHART_PNG_NAME: str = "mouse_clicks.png"
HIST2D_CHART_PNG_NAME: str = "mouse_coordinates.png"
SAVE_FILE_DIR: str = path.expanduser("~/Downloads/")


# WARNING: Google Collab Users
# if you are Google Collab to specifically run this Python code
# whereby if you don't have Python installed on your system or whatever the case may be
# uncomment the following lines below so that in Google Collab; you are able to import CSV file

# from google.colab import files

# print("Please upload your CSV file:")
# uploaded = files.upload()


# function to be able to create the mouse coordinates heatmap graph / image
def coordinates_heatmap(coords_x: pd.Series, coords_y: pd.Series):
    # create the figure and the axes
    fig, axes = plt.subplots()

    # invert y axis for the heatmap
    axes.invert_yaxis()

    # add more description to the graph / image
    axes.set_title("Mouse Movement Heatmap")
    axes.set_xlabel("X coordinate")
    axes.set_ylabel("Y coordinate")

    # create the actual 'hist2d' graph using `axes`
    hist2d_graph = axes.hist2d(coords_x, coords_y)

    # create a colour bar to show activity level "key"
    fig.colorbar(hist2d_graph[3], ax=axes)

    # make sure that nothing is cut off
    fig.tight_layout()

    # save the image to the downloads directory / folder
    fig.savefig(
        SAVE_FILE_DIR + HIST2D_CHART_PNG_NAME,
        dpi=600,
        bbox_inches="tight",
    )


# function to be able to create the mouse coordinates barchart graph / image
def buttons_count(
    left: pd.Series,
    right: pd.Series,
    middle: pd.Series,
    back: pd.Series,
    forward: pd.Series,
    scroll_up: pd.Series,
    scroll_down: pd.Series,
):
    # create the figure and the axes
    fig, axes = plt.subplots()

    # create the x-axis ==> writing the label for each bar
    mouse_buttons = [
        "Left Click",
        "Right Click",
        "Middle Click",
        "Back ( Mouse 4 )",
        "Forward ( Mouse 5 )",
        "Scroll-Up",
        "Scroll-Down",
    ]

    # create the y-axis ==> get the actual data from the CSV file
    # NOTE: we need to use `.sum` --> take a look at our data in the CSV file!
    mouse_click_count = [
        left.sum(),
        right.sum(),
        middle.sum(),
        back.sum(),
        forward.sum(),
        scroll_up.sum(),
        scroll_down.sum(),
    ]

    # add more description to the graph / image
    axes.set_ylabel("Click Counts")
    axes.set_title("Number of times mouse buttons clicked")

    # create the actual barchart graph using `axes`
    axes.bar(mouse_buttons, mouse_click_count)

    # rotate the labels
    plt.xticks(rotation=30, ha="right")

    # make sure that nothing is cut off
    fig.tight_layout()

    # save the image to the downloads directory / folder
    fig.savefig(SAVE_FILE_DIR + BAR_CHART_PNG_NAME, dpi=600, bbox_inches="tight")


# our main function
def main():
    # create a pandas object from the csv file
    csv_file = pd.read_csv(FILE_NAME)

    # call the function to create the number 'hist2d' graph / heatmap-like graph
    coordinates_heatmap(csv_file["x"], csv_file["y"])

    # call the function to create the barchart
    buttons_count(
        csv_file["left"],
        csv_file["right"],
        csv_file["middle"],
        csv_file["back"],
        csv_file["forward"],
        csv_file["scroll_up"],
        csv_file["scroll_down"],
    )


if __name__ == "__main__":
    main()
```

> Next up on our list is to add **error handling** for our `mouse_data.csv` file!

# Pyinstaller

> [!INFO] Resource(s)
> 
> - Official GitHub Repository: https://github.com/pyinstaller/pyinstaller
> - Official Documentation: https://pyinstaller.org/en

> [!WARNING] Switching Back To Working On Windows!
> 
> > Even for Python!
> 
> Given that we are going to be using `pyinstaller` to convert our little `main.py` file into a `.exe` / executable file...
> 
> Claude is telling me that we need to do that process on Windows itself as we are planning to ship this application for Windows users!
> 
> Therefore, here I am back on Windows and now its time to setup the Python Virtual Environment Here!

Here is what the current version of the file looks like:

```python
# import `path` from the `os` module
from os import getcwd, path

# import the matplotlib 2D plots to make the bar chart and heatmap graph
import matplotlib.pyplot as plt

# import the entire pandas library and use `pd` as alias
import pandas as pd

# global constants
FILE_NAME: str = "mouse_data.csv"
BAR_CHART_PNG_NAME: str = "mouse_clicks.png"
HIST2D_CHART_PNG_NAME: str = "mouse_coordinates.png"
SAVE_FILE_DIR: str = path.expanduser("~/Downloads/")


# WARNING: Google Collab Users
# if you are Google Collab to specifically run this Python code
# whereby if you don't have Python installed on your system or whatever the case may be
# uncomment the following lines below so that in Google Collab; you are able to import CSV file

# from google.colab import files

# print("Please upload your CSV file:")
# uploaded = files.upload()


# function to check if the CSV file actually exists
def csv_file_existance(file_name: str):
    # find the full path of the file
    file_full_path = path.join(getcwd(), file_name)

    # check if the file exists or not
    if not path.isfile(file_full_path):
        # output appropriate message to the user and exit the program
        print(f"CSV File '{file_name}' is MISSING at '{getcwd()}'!")

        exit(1)

    # if the file has been found
    print(f"CSV File '{file_name}' has been FOUND at '{getcwd()}'!")


# function to be able to create the mouse coordinates heatmap graph / image
def coordinates_heatmap(coords_x: pd.Series, coords_y: pd.Series):
    # create the figure and the axes
    fig, axes = plt.subplots()

    # invert y axis for the heatmap
    axes.invert_yaxis()

    # add more description to the graph / image
    axes.set_title("Mouse Movement Heatmap")
    axes.set_xlabel("X coordinate")
    axes.set_ylabel("Y coordinate")

    # create the actual 'hist2d' graph using `axes`
    hist2d_graph = axes.hist2d(coords_x, coords_y)

    # create a colour bar to show activity level "key"
    fig.colorbar(hist2d_graph[3], ax=axes)

    # make sure that nothing is cut off
    fig.tight_layout()

    # save the image to the downloads directory / folder
    fig.savefig(
        SAVE_FILE_DIR + HIST2D_CHART_PNG_NAME,
        dpi=600,
        bbox_inches="tight",
    )


# function to be able to create the mouse coordinates barchart graph / image
def buttons_count(
    left: pd.Series,
    right: pd.Series,
    middle: pd.Series,
    back: pd.Series,
    forward: pd.Series,
    scroll_up: pd.Series,
    scroll_down: pd.Series,
):
    # create the figure and the axes
    fig, axes = plt.subplots()

    # create the x-axis ==> writing the label for each bar
    mouse_buttons = [
        "Left Click",
        "Right Click",
        "Middle Click",
        "Back ( Mouse 4 )",
        "Forward ( Mouse 5 )",
        "Scroll-Up",
        "Scroll-Down",
    ]

    # create the y-axis ==> get the actual data from the CSV file
    # NOTE: we need to use `.sum` --> take a look at our data in the CSV file!
    mouse_click_count = [
        left.sum(),
        right.sum(),
        middle.sum(),
        back.sum(),
        forward.sum(),
        scroll_up.sum(),
        scroll_down.sum(),
    ]

    # add more description to the graph / image
    axes.set_ylabel("Click Counts")
    axes.set_title("Number of times mouse buttons clicked")

    # create the actual barchart graph using `axes`
    axes.bar(mouse_buttons, mouse_click_count)

    # rotate the labels
    plt.xticks(rotation=30, ha="right")

    # make sure that nothing is cut off
    fig.tight_layout()

    # save the image to the downloads directory / folder
    fig.savefig(SAVE_FILE_DIR + BAR_CHART_PNG_NAME, dpi=600, bbox_inches="tight")


# our main function
def main():
    # call the function to check if the CSV data file exists
    # WARNING: I have not check this; but I think you are going to have to comment
    # this file checker function below if you are using Google Collab!
    csv_file_existance(FILE_NAME)

    # create a pandas object from the csv file
    csv_file = pd.read_csv(FILE_NAME)

    # call the function to create the number 'hist2d' graph / heatmap-like graph
    coordinates_heatmap(csv_file["x"], csv_file["y"])

    # call the function to create the barchart
    buttons_count(
        csv_file["left"],
        csv_file["right"],
        csv_file["middle"],
        csv_file["back"],
        csv_file["forward"],
        csv_file["scroll_up"],
        csv_file["scroll_down"],
    )

    # display a little message
    print(f"Images has been created at '{SAVE_FILE_DIR}'")


if __name__ == "__main__":
    main()
```

## Setting Up Python Virtual Environment

- Create the Python virtual environment inside the `mouse-c-py` folder / directory:

```powershell
python -m venv venv
```

- Activate the Python Virtual Environment ( *on Windows* ):

```powershell
.\Scripts\activate
```

> [!BUG] I use a "Custom" and Debloated Windows
> 
> - Running the above program; I get the following error:
> 
> ```console
> .\Scripts\activate : File
> C:\Users\username\Desktop\mouse-c-py\venv\Scripts\Activate.ps1
> cannot be loaded because running scripts is disabled on this
> system. For more information, see about_Execution_Policies at
> https:/go.microsoft.com/fwlink/?LinkID=135170.
> At line:1 char:1
> + .\Scripts\activate
> + ~~~~~~~~~~~~~~~~~~
>     + CategoryInfo          : SecurityError: (:) [], PSSecurityE
>    xception
>     + FullyQualifiedErrorId : UnauthorizedAccess
> ```
> 
> > [!TIP] The Fix!
> > 
> > Run the following commands to enable these types of scripts to be able to run on *my* machine:
> > 
> > ```powershell
> > Set-ExecutionPolicy -ExecutionPolicy RemoteSigned -Scope CurrentUser
> > ```
>  
> > [!SUCCESS]
> > Therefore, go ahead and run the command to activate the virtual environment and you should be good to go!
> > 
> > ```console
> > # the `(venv)` prefix is here!
> > (venv) PS C:\Users\username\Desktop\mouse-c-py\venv>
> > ```

- Install the required dependencies from the `requirements.txt` file:

```powershell
pip install -r requirements.txt
```

- Install Pyinstaller:

```powershell
pip install -U pyinstaller
```

> [!INFO] What does `-U` means for the `pip` command?
> 
> The `-U` flags means that if the module / dependency is **not** installed. It will go ahead and install the package as usual!
> 
> > The fuck is the difference then?
> 
> The difference comes in when the package is already installed on your system!
> 
> If the package is already installed on your systems and you try to run it again; `pip` will simply tell you that it's already here...
> 
> But **using** the `-U` flag; its going to try to fetch the latest version / any updates that has been done to the dependency.

## Creating The Executable File

- From the documentation:

```powershell
# this is what the documentation is telling to run
pyinstaller main.py
```

> [!NOTE]
> So this might be the reason that Claude is telling us to continue the entire project on Windows itself.
> 
> - At the start of the compilation phase:
> 
> ```console
> 207 INFO: Platform: Windows-11-10.0.26200-SP0
> ```

> [!SUCCESS] Compiled Executable!
> 
> I see see that a `dist/main` directory has been created and inside that `main` folder, we have the following:
> 
> ```console
>     Directory: C:\Users\username\Desktop\mouse-c-py\venv\dist\main
> 
> 
> Mode                 LastWriteTime         Length Name
> ----                 -------------         ------ ----
> d-----         4/11/2026   2:04 PM                _internal
> -a----         4/11/2026   2:04 PM       11878088 main.exe
> ```

> [!SUCCESS] Running The Executable!
> 
> > [!TIP] Important!
> > 
> > So I when into the `dist/main` directory and copied the `main.exe` **executable file** to the *root* of the virtual environment folder.
> 
> Running it for the first time; it opened a command prompt / terminal window with a blinking cursor and it simply shut-off!
> 
> Now trying to run it from the command line with `.\main.exe`; I see the following error:
> 
> ```console
> [PYI-7648:ERROR] Failed to load Python DLL 'C:\Users\username\Desktop\mouse-c-py\venv\_internal\python314.dll'.
> ```
> 
> Therefore, what I did instead was take our `mouse_data.csv` file and moved it to the `dist/main` directory where the original `main.exe` is currently located.
> 
> Now, double-clicking on the `main.exe` executable file again, I see that we do see the `mouse_data.csv` file ( *from the `print` function that we have in the code* ) and the program did run and create the 2 images inside our `~/Downloads` folder!
> 
> > Yipee!

### Claude's Command

Well, this is where Claude's command come into play ( *I guess... I did not run it yet*! ).

With the `--onefile`, I think the missing `.dll` file error will disappear and we will be able to run the program from anywhere in our computer!

> [!NOTE]
> I am now going to **delete** everything that has been created and run the command that Claude is telling me to run!

#### Add One More Function

Before we go ahead and add the following function to our `main.py` file to create a graph that actually mimics the mouse position of the screen

> Origin at the top-left corner similar to how all screen are!

> [!WARNING]
> > I knew that we cannot rely on AI!
> 
> Given that I am working on Windows, I don't have access to Neovim and therefore I cannot do `gd` to '*go to definition*' ( *really well - Zed* )
> 
> Therefore, I started thinking; The developers / programmers that made `hist2d` might not have added that "*functionality*" to move the origin!
> 
> Therefore talking with Claude, it now tells me that this is a "_**known issue**_" or something along the lines.
> 
> Hence, I decided to ask it if we could use `imshow` ( *because I saw some YouTube videos using `imshow` for making a heatmap* ).
> 
> > [!SUCCESS]
> > We are able to move the **origin** to the top-left corner of our graph so that the mouse movement is represented 1:1.
> 
> Therefore, I am going to now try to learn a bit about `imshow` as Claude gave me the *fix*. But I don't really understand it!

> [!NOTE] Matplotlib ( *and C* ) is vast!
> 
> Yeap... After this little introduction and after making `v0.0`... I think I am going to go and actually re-write all the code and actually go learn matplotlib for real.
> 
> > Because its a powerful tool!

```python
# WARNING: testing `imshow` ( just to check it we can move the coordinates )
# this code was given by code... Given that it works and does move the origin
# I am now going to go and learn more about the `imshow` and `spines`
# after this learning endeavour and before I add a GUI to this;
# I will take some time to learn about the `matplotlib` module as a whole
def coordinates_heatmap_imshow(coords_x: pd.Series, coords_y: pd.Series):
    # create the figure and the axes
    fig, axes = plt.subplots()

    # move the x-axis from the bottom to the top
    axes.spines["bottom"].set_position(("axes", 1.0))

    # make the x-axis "ticks" and labels appear at the top
    axes.xaxis.set_ticks_position("top")
    axes.xaxis.set_label_position("top")

    # NOTE: the lines of code below actually removes the border for the graph
    # we don't want that - we want it to have that "box" around the graph
    # axes.spines["top"].set_visible(False)
    # axes.spines["bottom"].set_visible(False)
    # axes.spines["right"].set_visible(False)
    # axes.spines["left"].set_visible(False)

    # add more description to the graph / image
    axes.set_title("Mouse Movement Heatmap ( IMSHOW Screen Representation )")

    # label the axes
    axes.set_xlabel("X coordinate")
    axes.set_ylabel("Y coordinate")

    # get the data to compute the histogram
    # NOTE: `xedges` and `yedges` are arrays and the `heatmap` is a 2D-array
    heatmap, xedges, yedges = np.histogram2d(coords_x, coords_y)

    # move the origin to the top-left corner of the screen to match screen
    image = axes.imshow(
      # transpose the array
      heatmap.T,
      # set the origin to the top-left corner
      origin="upper",
      # stretch to fill the axes
      aspect="auto",
      # get the actual data coordinates
      extent=[xedges[0], xedges[-1], yedges[-1], yedges[0]],
    )

    # create a colour bar to show activity level "key"
    fig.colorbar(image, ax=axes)

    # make sure that nothing is cut off
    fig.tight_layout()

    # save the image to the downloads directory / folder
    fig.savefig(
      SAVE_FILE_DIR + IMSHOW_PNG_NAME,
      dpi=600,
      bbox_inches="tight",
)
```

#### Create The Actual Executable

- This is the command that Claude is telling me to run:

```powershell
pyinstaller --onefile --name graphs main.py
```

---

# Back To C!

Given that we know have our `graphs.exe` file... We need to call it from the `main.c` file.

Therefore, this can be easily done using the `system` **function** from the `stdlib` library.

Therefore, this is what we have to add in our `main.c` file:

```C
  // close the file to free up memory
  fclose(csv_file);

  // call the `graphs.exe` created by `pyinstaller` from the `main.py` file
  // INFO: this calls the `graphs.exe` file which is going to search
  // for our CSV file and then be able to generate the graphs in our downloads folder!
  // WARNING: I think this is shit and might be a security issue
  // but for what it is right now, I don't really care!
  system("graphs.exe");

  // destroy the hidden window that we created
  DestroyWindow(hwnd);
  // unregister the window class
  UnregisterClassW(L"RawInputWindow", GetModuleHandleW(NULL));
```

Therefore, running our `main.c` file using our `Makefile`, we should see something like this:

```console
Recording mouse data... Time remaining: -0.0 seconds

==================================================

        Python Program Starting!

==================================================


--------------------------------------------------

CSV File 'mouse_data.csv' has been FOUND at 'C:\Users\username\Desktop\mouse-c-py'!

--------------------------------------------------

Images has been created at 'C:\Users\username/Downloads/'

--------------------------------------------------


==================================================

        Python Program Ending!

==================================================

Program Exiting... Thank You!
```

> Yes, I did modify the *output* of the `main.py` file!

> [!SUCCESS]
> 
> > [Great Success](https://www.youtube.com/watch?v=r13riaRKGo0)
> 
> Given that the initial versions are working... The thing that we have to do right now is go simply go ahead and make the GitHub repository for version `0.0.0`.

---

# GitHub Actions

> [!INFO] Resource(s)
> 
> - GitHub Actions Official Documentations: https://docs.github.com/en/actions

First of all create a `.gitignore` file so that things we don't want to push does not accidentally gets pushed to our repository.

> Claude generated the `.gitignore` file but I modified it a bit!

```txt
# compiled executables
*.exe
*.out
*.app

# intermediate objects
*.o
*.obj

# pyinstaller build folders
build/
dist/
*.spec

# generated data
*.csv

# some random stuff ( might remove later )
nul
```

## Our Makefile

Because we want to compile using GitHub's *computers*... We don't actually want to run the program there!

Given that right now we have `program: compile run clean` which **compiles**, **runs** and **cleans** the executable file.

> Well if we want to compile only, we are going to have to modify our `Makefile`.

Now, GitHub Actions uses the `build` keyword for the **target** ( *i.e `build, program, compile, etc`* ).

Therefore, modify our **current** `Makefile` to something like this:

```make
# remote - GitHub Actions ( compiling the `main.c` file )
build:
	@gcc main.c -Wall -Wextra -lgdi32 -o program.exe

# local development ( running and testing )
program: compile run clean

# compile the program
compile:
	@gcc main.c -Wall -Wextra -lgdi32 -o program

# run the `program.exe` executable file
run:
	@./program

# remove / clean the `program.exe` executable file
clean:
	@del program.exe
```

So before, we write the `.yaml` file for GitHub Actions, I need to tell you something.

I wanted to have automated code pushes to compilation.

- But this is going to be bad as each and every single push is going to **redo** the `.exe` file even if we updated the `README.md` file
- Therefore, I am going to *stick* with using `git tag`s

> [!NOTE] Git Tag
> 
> > I will have to make a complete note about this after!
> 
> In short, we all know about `commit`s and `branch`es. But what is a Git `tag`?
> 
> A Git Tag **points** to a *specific commit* and gives it a "*name*".
> 
> What basically is marking *that* specific **commit** and kind-of "*bookmarks*" it.
> 
> Thereby giving it a special purpose or meaning of some sort.
> 
> This is basically what we are going to be using to mark as "_**build**_" for the `.exe` files compilation.
> 
> ```bash
> # here is a little simple usage
> git tag <insert_name>
> ```
> 
> > I will be adding this to my overall GitHub Notes later on ( Public Obsidian Link: https://sunhaloo.github.io/obsidian-quartz/Learning/Hobbies/Git/ )!


## Releases YAML GitHub Actions File

> [!WARNING]
> This `.yaml` file was completely generated by Claude after I discussed what is possible to do and what I want.
> 
> Reading it I do understand it but I have a simple question for Claude ( *will continue below* ).

> [!WARNING] Where and how to save it?
> 
> You are going to have to save this file under `<repository_root>/.github/workflows/release.yml`.

```yaml
name: Release

on:
  push:
    tags:
      - 'v*'

env:
  FORCE_JAVASCRIPT_ACTIONS_TO_NODE24: true

jobs:
  build-and-release:
    runs-on: windows-latest
    
    permissions:
      contents: write

    steps:
      - name: Checkout code
        uses: actions/checkout@v4

      - name: Setup Python
        uses: actions/setup-python@v5
        with:
          python-version: '3.12'

      - name: Install Python dependencies
        run: pip install matplotlib numpy pandas pyinstaller

      - name: Install MSYS2
        run: choco install msys2 -y

      - name: Build program.exe
        run: make build

      - name: Build graphs.exe
        run: pyinstaller --onefile --name graphs main.py

      - name: Create GitHub Release
        uses: softprops/action-gh-release@v2
        with:
          generate_release_notes: true
          files: |
            program.exe
            dist/graphs.exe
```

- The `.exe` files are going to be **compiled** when I create a `tag` that starts with the `v` character
- Switch to `nodejs` '24' to avoid any warnings as `nodejs` '20' is deprecated
- Give '**write**' permission else '403' error ( *ask me how I know that* )
- [Chocolatey](https://community.chocolatey.org/) / `choco` is already pre-installed on the machine
	- Making it easy to download Windows specific packages
- As Python is popular, GitHub Actions already provides an "*action*" for Python
	- No `venv` folder created as each time its a fresh instance ( *so basically no pollution of the environment / setup* )
- `mysys2` setup for having / getting `gcc` + `make`

### Steps

- Add the above YAML `release.yml` file in the `<repo_root>/.github/workflows/release.yml`
- Then `add`, `commit` and `push` as normal
- Tag that commit with `git tag v0.1.0` ( *in my case* )
- Then push that `tag` with `git push origin refs/tags/v0.1.0` ( *in my case* )!

> [!TIP] Why `refs/tags/v0.1.0`?
> 
> Given that in *my* case my *branch name* is the **same** as my `tag`... Therefore, I need to specify that its an actual `tag`.
> 
> > [!WARNING]
> > After you push that `tag` you now are going to **have** to *use* `git push origin refs/heads/v0.1.0` **instead** of a simple `git push -u origin v0.1.0`

> [!TIP]
> To not repeat the same mistake that I made above where now I have to provide *full path* for the `push`...
> 
> Simply bump up the version of the tag ( *again in my case its too late* )... Meaning given that your branch name is `v.0.1.0`; you instead do `git tag v.0.1.1` instead!
> 
> > Or you could simply pick a **different** `tag` *name*!
> 
> ```bash
> # delete the current local tag
> git tag -d v0.1.0
> # push the new changes to remote repo
> git push origin --delete refs/tags/v0.1.0
> 
> # therefore simply re-tag with a different name!
> ```
> 
> > I think I am going to do it!

# Integrating Python Code Into C Code ( C-Python API )

Okay, we are going to go off on a little tangent to learn about C-Python with Claude.

> [!INFO] Resource(s)
> 
> - Mr AI: https://www.youtube.com/watch?v=ndLqg4RYcXE
> - NeuralNine ( The GOAT Writes C Code ): https://www.youtube.com/watch?v=92Z6lqd8U8w

## Learning C-Python Setup

Here is what I currently have ( *and what I told Claude what I have* ):

> I created a `test` folder over at `~/Desktop/test`

```console
Folder PATH listing
Volume serial number is 5411-C9BE
C:.
    main.c
    main.py
    Makefile
    
No subfolders exist 
```

### Python Installation Location

The most important thing right now is to know where our Python **executable file** is located on our system.

> We can use the `Get-Command` command to do that ( *basically the `which` command found on Linux/UNIX systems* )!

> [!NOTE]
> Given that I know how to install things from the command line on Linux based systems.
> 
> Therefore, to download Python '3.14.2'; I simply used the official `winget` package manager.
> 
> What I am trying to say its that my `python.exe` path might **not** be similar to yours!

- Find out where Python is located on our system:

```powershell
Get-Command python
```

- This is the output after running the above command:

```console
C:\Users\username\AppData\Local\Programs\Python\Python314\python.exe
```

> Remember this well as we are going to need this!

### Modify `Makefile`

We are going to have to modify our `Makefile` so that when we compile we link the Python library ( *codes* ) and the header file.

Therefore, in my case, I am going to need these 2 files:

1. `C:\Users\username\AppData\Local\Programs\Python\Python314\include`
2. `C:\Users\user\AppData\Local\Programs\Python\Python314\libs`

Hence, I am going to modify my `Makefile` so that it now looks like this:

```bash
# C-Python relate stuff
PYTHON_INCLUDE = C:\Users\$(USERNAME)\AppData\Local\Programs\Python\Python314\include
PYTHON_LIBS = C:\Users\$(USERNAME)\AppData\Local\Programs\Python\Python314\libs

program: compile run clean

compile:
	@gcc main.c -I$(PYTHON_INCLUDE) -L$(PYTHON_LIBS) -lpython314 -Wall -Wextra -o program

run:
	@./program

clean:
	@del program.exe
```

> [A few words!](https://www.youtube.com/watch?v=E92glWB6sPE)

So first of all, given that we have a "*user*" path in our `Makefile` now. I found out that you could simply do `$(USERNAME)` ( *like user the `USERNAME` variable* ) to replace *my* username with your system's username; in the case that you want to work with the code!

> [!INFO]
> 
> > Fun Fact!
> 
> If you run `echo $USERNAME` or even `echo $USER` ( *the Linux way* ) inside Powershell. Well you get **nothing** at all!
> 
> If you want to get / see your *username* from Powershell; therefore, you are going to have to run this:
> 
> ```powershell
> echo $env:USERNAME
> ```

Additionally, we are now linking the `Python.h` header and `python314.lib` file. But there is something that we need to do. We **don't** point to the actual `Python.h` and `python314.lib` file directly.

Instead we simply *point* to the folder that they resides in!

- Running `make` from my terminal, I see this as output:

```console
Hello World
```

> [!SUCCESS]
> Very Fucking Nice!

## Running Python From Inside of `main.c` File!

> [!INFO] Resource(s)
> 
> - Official Python Documentation: https://docs.python.org/3/c-api/intro.html#include-files

Go ahead and run the following code below.

> Again, I know nothing about this and I am learning Claude!

```C
// include the Python header file ==> linked through `Makefile`
#include <Python.h>
#include <stdio.h>

// idk Claude tell me that "its using the newer `ssize_t` compared to old `int`"
// python official documentation: https://docs.python.org/3/c-api/intro.html#include-files
#define PY_SSIZE_T_CLEAN

// our main function
int main(int argc, char *argv[]) {
  // initiliase python interpreter
  Py_Initialize();

  // run 'hello world' from within C itself
  printf("Hello World From C!\n");

  // run 'hello world' from C but through Python ( intepreter )
  PyRun_SimpleString("print('Hello World From C Through Python Interpreter')");

  // shut down the python interpreter
  Py_Finalize();

  return 0;
}
```

If you run the above code using our `Makefile`; we can see that our output is like so:

```console
Hello World From C Through Python Interpreter
Hello World From C!
```

### Code Explanation

- `#define PY_SSIZE_T_CLEAN`:
	- Something that the official Python documentation says that you have to add
	- Apparently uses some *type* of **better** *datatypes*
- `Py_Initialize();`:
	- Simple, enough, initialise the Python interpreter from inside of our C code
- `PyRun_SimpleString("...");`:
	- This is basically using the `print` function that is found in Python
- `Py_Finalize();`:
	- Simply close / exit out of the Python interpreter

## Another More Concrete Example

Let me do another learning session whereby this one is going to be much more inline to what we are going to be working with in the project.

- Create a simple little `main.py` file:

```python
# function to add 2 integer numbers
def int_add(x: int, y: int) -> int:
    return x + y


# function to greet the user
def greet(name: str):
    print(f"\n\t-- Hello {name}! --\n")


# our main function that calls everything
def main():
    print("\n" + "-" * 50, "\n")
    print("\t-- Python File's Main Function --")
    print("\n" + "-" * 50, "\n")

    # ask the user to enter this name
    user_name = input("Please Enter Your Name: ")

    # call the function to greet the user
    greet(user_name)

    # ask the user to enter 2 integer numbers to add
    while True:
        try:
            num_1 = int(input("Please Enter First Number: "))
            num_2 = int(input("Please Enter Second Number: "))

            break

        except ValueError:
            print("\n\t == Enter Integer Numbers Only!!! ==\n")

    addition_result: int = int_add(num_1, num_2)

    print(f"\nResult Of Addition: {addition_result}")


if __name__ == "__main__":
    main()
```

- *Initialise* our `main.c` file:

```C
// as per the official python documentation
#define PY_SSIZE_T_CLEAN

// include the Python header file ==> linked through `Makefile`
#include <Python.h>

// our main function
int main(int argc, char *argv[]) {
  // initiliase python interpreter
  Py_Initialize();

  // shut down the python interpreter
  Py_Finalize();

  return 0;
}
```

### Call A Single Function Only And Pass Arguments

```C
// include the Python header file ==> linked through `Makefile`
// as per the official python documentation
#define PY_SSIZE_T_CLEAN

// include the Python header file ==> linked through `Makefile`
#include <Python.h>


// our main function
int main(int argc, char *argv[]) {
  // initiliase python interpreter
  Py_Initialize();

  // import the `main.py` file as a module ( basically )
  // INFO: we don't add the `.py` extension
  PyObject *module = PyImport_ImportModule("main");

  // get the `greet` function from Python file
  // this is done by ( creating a ) referencing the `greet` function inside `main.py`
  PyObject *greet_function = PyObject_GetAttrString(module, "greet");

  // our arguments
  // NOTE: everything that needs to be passed as arguments should be inside a tuple
  // INFO: the `"(s)"` represents that we are passing a string argument
  PyObject *greet_arguments = Py_BuildValue("(s)", "Sebastien Vettel");

  // call the function from inside C code!
  PyObject *greet_func_result = PyObject_CallObject(greet_function, greet_arguments);

  // decrement reference counts to tell Python we are done with these objects
  // INFO: when reference count hits 0, Python's memory manager frees the object from the heap
  Py_DECREF(greet_func_result);
  Py_DECREF(greet_arguments);
  Py_DECREF(greet_function);
  Py_DECREF(module);

  // shut down the python interpreter
  Py_Finalize();

  return 0;
}
```

- This is the output after running the above `main.c` file:

```console

	-- Hello Sebastien Vettel! --

```

> [!INFO]
> I just noticed something.
> 
> You see how we packaged the argument for the `greet` function inside a **tuple** right?
> 
> Well, talking with [Gemini](https://gemini.google.com). It did tell me that it does use some *sort of* `tuple` behind the scenes.
> 
> > "*sort of*" because you and me know that its much complicated behind the scenes!

### Call Function That Will Return Something

```C
// as per the official python documentation
#define PY_SSIZE_T_CLEAN

// include the Python header file ==> linked through `Makefile`
#include <Python.h>

void display_rule(int char_count) {
  printf("\n\t");

  for (int i = 0; i < char_count; i++) {
    printf("-");
  }
  printf("\n\n");
}

// as per the official python documentation
#define PY_SSIZE_T_CLEAN

// our main function
int main(int argc, char *argv[]) {
  // initiliase python interpreter
  Py_Initialize();

  // import the `main.py` file as a module ( basically )
  // INFO: we don't add the `.py` extension
  PyObject *module = PyImport_ImportModule("main");

  // get the `int_add` function from Python file
  PyObject *int_add_function = PyObject_GetAttrString(module, "int_add");

  // pass our arguments
  // NOTE: as you can see here; we are passing 2 integer numbers
  // therefore, we simply do `"(ii)"`
  PyObject *int_add_arguments = Py_BuildValue("(ii)", 1, 1);

  // simply run the `int_add` function of the `main.py` file
  PyObject *int_add_result = PyObject_CallObject(int_add_function, int_add_arguments);

  // get the data from the result of the function
  // WARNING: needs to be converted back into a simple C data type / "object"
  int addition_result = (int)PyLong_AsLong(int_add_result);

  // display the result to from C
  display_rule(50);
  printf("\t   -- Result Of Python Addition Function: %d! --\n", addition_result);
  display_rule(50);

  // clean memory to avoid memory leaks by "imported" function
  // INFO: meaning that not leftover Python's functions left behind in C's memory
  Py_DECREF(int_add_result);
  Py_DECREF(int_add_arguments);
  Py_DECREF(int_add_function);
  Py_DECREF(module);

  // shut down the python interpreter
  Py_Finalize();

  return 0;
}
```

- This is the output after running the above `main.c` file:

```console

	--------------------------------------------------

	   -- Result Of Python Addition Function: 2! --

	--------------------------------------------------

```

### Call The Python Main Function From C

```C
// as per the official python documentation
#define PY_SSIZE_T_CLEAN

// include the Python header file ==> linked through `Makefile`
#include <Python.h>

// our main function
int main(int argc, char *argv[]) {
  // initiliase python interpreter
  Py_Initialize();

  // import the `main.py` file as a module ( basically )
  // INFO: we don't add the `.py` extension
  PyObject *module = PyImport_ImportModule("main");

  // get the `main` function from Python file
  PyObject *main_function = PyObject_GetAttrString(module, "main");

  // NOTE: no arguments
  // even if there are no arguments that we "physically" have to pass
  // Python functions still expect a tuple argument for the functions

  // simply run the `main` function of the `main.py` file
  PyObject *main_result = PyObject_CallObject(main_function, PyTuple_New(0));

  // decrement reference counts to tell Python we are done with these objects
  // INFO: when reference count hits 0, Python's memory manager frees the object from the heap
  Py_DECREF(main_result);
  Py_DECREF(main_function);
  Py_DECREF(module);

  // shut down the python interpreter
  Py_Finalize();

  return 0;
}
```

- This is the output after running the above `main.c` file:

```console

--------------------------------------------------

	-- Python File's Main Function --

--------------------------------------------------

Please Enter Your Name: Lewis Hamilton

	-- Hello Lewis Hamilton! --

Please Enter First Number: 1
Please Enter Second Number: 2

Result Of Addition: 3
```

# Actual Python Code Integration

We are going to have to do some little things before we actually start using C-Python in our project.

1. Create a new branch `v0.2.0`
2. Update the `Makefile` to include Python header and library file(s)
3. Then go ahead and use C-Python

## Updating Makefile

Well, we know that we have to **update** our `Makefile` so that we can link up the `Python.h` header file and also the Python libraries found on our system.

Now, there is also something that we need to consider an cater for.

> GitHub Releases!

We are going to want to **also** update it so that we have a separate build "*path*" for GitHub's Workflow compute.

Hence, the simple reason as to why we have to do that!

```bash
# ohh this is similar to a C header file
# NOTE: `pythonLocation` is an environment variable created by `actions/setup-python`
ifdef pythonLocation
	# for remote release
	PYTHON_INCLUDE = $(pythonLocation)\include
	PYTHON_LIBS = $(pythonLocation)\libs
else
	# for local development
	# NOTE: I install Python using the "official" `winget` package manager
	PYTHON_INCLUDE = C:\Users\$(USERNAME)\AppData\Local\Programs\Python\Python314\include
	PYTHON_LIBS = C:\Users\$(USERNAME)\AppData\Local\Programs\Python\Python314\libs
endif


# remote - GitHub Actions ( compiling the `main.c` file )
build:
	@gcc main.c -I$(PYTHON_INCLUDE) -L$(PYTHON_LIBS) -lpython314 -Wall -Wextra -o program.exe

# local development ( running and testing )
program: compile run clean

# compile the program
compile:
	@gcc main.c -I$(PYTHON_INCLUDE) -L$(PYTHON_LIBS) -lpython314 -Wall -Wextra -o program.exe

# run the `program.exe` executable file
run:
	@./program.exe

# remove / clean the `program.exe` executable file
clean:
	@del program.exe
```

## Update Release YAML File

Similarly, as we are going to be using C-Python; we won't need to create the `graphs.exe` executable file anymore.

Therefore, we are going to remove the `pyinstaller` part from the `release.yml` file.

```yml
name: Release

on:
  push:
    tags:
      - "v*"

env:
  FORCE_JAVASCRIPT_ACTIONS_TO_NODE24: true

jobs:
  build-and-release:
    runs-on: windows-latest

    permissions:
      contents: write

    steps:
      - name: Checkout code
        uses: actions/checkout@v4

      - name: Setup Python
        uses: actions/setup-python@v5
        with:
          python-version: "3.12"

      - name: Install Python dependencies
        run: pip install matplotlib numpy pandas

      - name: Install MSYS2
        run: choco install msys2 -y

      - name: Build program.exe
        run: make build

      - name: Create GitHub Release
        uses: softprops/action-gh-release@v2
        with:
          generate_release_notes: true
          files: |
            program.exe
            dist/graphs.exe
```

## Update Requirements File Also!

> Yeap, I nearly forgot about this!

We also need to update our `requirements.txt` file so that we don't install *bloat* on our system.

```console
contourpy==1.3.3
cycler==0.12.1
fonttools==4.62.1
kiwisolver==1.5.0
matplotlib==3.10.8
numpy==2.4.4
packaging==26.0
pandas==3.0.2
pillow==12.2.0
pyparsing==3.3.2
python-dateutil==2.9.0.post0
six==1.17.0
tzdata==2026.1
```

> [!INFO]
> I am now going to create our `v0.2.0` branch and then update the required files!

## Updating Our C File!

> [!NOTE]
> I am only showing you the `main` function of the `main.c` file!

```C
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
    printf("\nFailed to register window class! Error: %lu\n", GetLastError());
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
    printf("\nFailed to create window! Error: %lu\n", GetLastError());
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
    printf("\nFailed to register raw input device! Error: %lu\n", GetLastError());
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
    printf("\nFailed to open CSV file for writing!\n");

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

  // C-Python integration / coding

  // intialise the python interpreter
  Py_Initialize();

  // import the 'main.py' file inside our C codebase
  PyObject *py_module = PyImport_ImportModule("main");

  // check if we successfully import the 'main.py' file
  if (py_module == NULL) {
    printf("\nFailed to import the 'main.py' file!\n");

    // display the error from python's interpreter
    PyErr_Print();

    // shutdown the python's interpreter
    Py_Finalize();

    // return an error to the user
    return 1;
  }

  // reference the `main` function from the 'main.py' file
  PyObject *py_func = PyObject_GetAttrString(py_module, "main");

  // check if we are able to "get" the `main` function of 'main.py'
  if (py_func == NULL) {
    printf("\nFailed to get `main` function of 'main.py' file!\n");

    // display the error from python's interpreter
    PyErr_Print();

    // decrement reference count --> when '0' ==> removed from memory's heap
    Py_DECREF(py_module);

    // shutdown the python's interpreter
    Py_Finalize();

    // return an error to the user
    return 1;
  }

  // INFO: no need to pass arguments but it expects a tuple
  // for more information refer to 'learning.md' file or online resources

  // simply run the python's `main` function from inside of here!
  PyObject *py_output = PyObject_CallObject(py_func, PyTuple_New(0));

  // check if we are able to call the `main` function of 'main.py'
  if (py_output == NULL) {
    printf("\nFailed to run `main` function of 'main.py' file!\n");

        // display the error from python's interpreter
        PyErr_Print();

        // decrement reference count --> when '0' ==> removed from memory's heap
        Py_DECREF(py_func);
        Py_DECREF(py_module);

        // shutdown the python's interpreter
        Py_Finalize();

        // return an error to the user
        return 1;
  }

  // decrement reference count --> when '0' ==> removed from memory's heap
  Py_DECREF(py_output);
  Py_DECREF(py_func);
  Py_DECREF(py_module);

  // shutdown the python interpreter
  Py_Finalize();

  // destroy the hidden window that we created
  DestroyWindow(hwnd);
  // unregister the window class
  UnregisterClassW(L"RawInputWindow", GetModuleHandleW(NULL));

  printf("Program Exiting... Thank You!\n");

  return 0;
}
```

# The Problem

> [Houston we have problem](https://www.youtube.com/watch?v=eco_xvkEQlg)!

## What Was I Trying To Do?

So currently in our `v0.1.0` branch we have 2 executable file that a *non-technical* user has to download in order to be able to use it.

What I am trying to do is eliminate the need for the user to download the `graphs.exe` executable file and to simply run use C-Python API and be able to run the functions found inside the `main.py` file without having to create the `graphs.exe` file!

> [!TIP]
> I was like: "*OMG I am so smart*"!
> 
> Then the user will simply need to download the **extremely small** `program.exe` file and thus be happy that I don't bloat his / her system!
> 
> > "*Professional*" I said to myself!
> 
> Well fuck!

## The Problem In Detail

The above codes that I for when I was learning C-Python **does** work and you are able to run it!

The problem comes with the Python **virtual environment**!

Compiling the program **without** the Python's virtual environment and all of the `requirements.txt` is going to result in this:

```console
Failed to import the 'main.py' file!
Traceback (most recent call last):
  File "C:\Users\username\Desktop\mouse-c-py\main.py", line 5, in <module>
    import matplotlib.pyplot as plt
ModuleNotFoundError: No module named 'matplotlib'
make: *** [Makefile:28: run] Error 1
```

Therefore, I simply created a new `venv` folder, moved things like `main.c`, `main.py` and `Makefile` there and thus compiled again.

Running the new, freshly compiled `program.exe` file does result in a working "*program*" and the `.png` files does get created in our `~/Downloads` folder!

> But we do get this little pesky warning!

```console
<frozen site>:101: RuntimeWarning: Unexpected value in sys.prefix, expected C:\Users\username\Desktop\mouse-c-py, got C:\Users\username\Desktop\mouse-c-py\venv
<frozen site>:101: RuntimeWarning: Unexpected value in sys.exec_prefix, expected C:\Users\username\Desktop\mouse-c-py, got C:\Users\username\Desktop\mouse-c-py\venv
```

> [!NOTE]
> I knew that it was related to python because of the 'sys' word. Nevertheless, had not clue of why we do get this warning message.
> 
> After speaking with Claude; it told me that this is due to the fact that our `main.c` file has been compiled inside the `venv` folder and it should have instead been compiled *outside* ( *i.e `~/Desktop/mouse-c-py`* ).

> But this is line an infinite loop whereby I need our `main.c` file to be **inside** `venv` to be able to compile.

## In The End

Well, for the *non-technical* user to use my program, he / she have had to:

1. Clone the repository
2. Make sure that Python, MYSYS, GCC and MAKE are installed on their system
3. Create a virtual environment and compiled the `main.c` file

> You get the fucking point!

Well in that case, the "*non-technical*" user will think that he is a hacker at this point ( *like in the movies* )!

> [!WARNING]
> Therefore, I think for now, I am just going to leave it whereby the user is going to have to download **both** the `program.exe` and `graphs.exe` file to be able to use my program.
> 
> This will remove any complicated things from my side and also any complicated dependencies thingy!

> [!INFO]
> Hence, I am simply going to try to optimise the current code to the best of me and Claude's abilities for `v.0.2.0` and in `v0.3.0` we are going to implement the Raylib ( GUI ) GUI!

> "Mission failed successfully!"

# Version 0.2.0

> [!WARNING]
> Given that I "*lost*" and I have reverted all my codes back to the current codes found in the **branch** of `v0.1.0`.
> 
> > Well, I am going to continue where I left off from there!

## Chatting With Gemini!

I am currently chatting with Gemini and I was just speaking about decreasing the size of the `graphs.exe` file and there is a lot of things that we can do.

Apparently when we compile, using `pyinstaller`, a simple, let's say 'hello world' Python program. Its also going to compile things like 'TKinter' and such and such.

Therefore, `pyinstaller` give you the `--exclude` flag so that we are able to remove them.

> [!INFO] But how do you see what you can remove?
> 
> But how can you check which files are bundled with your created executable file.
> 
> This is where the `--onedir` flags comes into play!
> 
> Therefore, I am going to run the following command with our **current** `main.py` file and all of the code inside still intact.
> 
> ```powershell
> pyinstaller --onedir --name graphs main.py
> ```
> 
> The executable file is going to still created under the `venv\dist\graphs` but we are **not** interested in that right now; what we are actually interested is the `_internal` folder found *under* that **same directory**.
> 
> Go ahead and use the properties window or the command line to check for its side; I see that the whole folder is around 88.9 MB ( *actual 'Size' from the Properties window* ).
> 
> > The actual `graphs.exe` file is still around 46.3 MB *if we use the `--onefile` flag instead of the current flag*

### Excluding some things

Let's just go ahead and **exclude** some modules and we should see a **small decrease** in file size.

- Compile the `main.py` file using the following command:

```powershell
pyinstaller --onedir \
--exclude-module tkinter \
--exclude-module tcl \
--exclude-module _tkinter \
--exclude-module sqlite3 \
--exclude-module IPython \
--exclude-module jedi \
--exclude-module matplotlib.tests \
--exclude-module numpy.tests \
--name graphs \
main.py
```

> [!WARNING]
> So the thing is; we know that Windows is shit but I did not really know that Windows Terminal ( *more like Powershell and CMD* ) does not allow us to use the `\` character to indicate that the command is actually on a single line but we just *spread* it across multiple ones.
> 
> Therefore, the above command does not actually works... Therefore, you are going to have to use this one:
> 
> ```powershell
> pyinstaller --onedir `
> --exclude-module tkinter `
> --exclude-module tcl `
> --exclude-module _tkinter `
> --exclude-module sqlite3 `
> --exclude-module IPython `
> --exclude-module jedi `
> --exclude-module matplotlib.tests `
> --exclude-module numpy.tests `
> --name graphs `
> main.py
> ```

> [!SUCCESS]
> While still having all our dependencies like `matplotlib`, `numpy` and `pandas`; we see that the `_internal` folder size has been reduced to 79.7 MB!
> 
> Additionally if we change out the `--onedir` flag for the `--onefile` flag we should also see that the file size is now 42.5 MB.

> [!INFO]
> I just ran the above command but instead of using `--onedir`; I used `--onefile` and moved my `graphs.exe` ( *output* ) from the `dist` folder to my *current* working `~/Desktop/mouse-c-py` folder and compile the `main.c` and therefore ran it
> 
> > [!SUCCESS]
> > It code runs and our images are created successfully in our `~/Downloads` folder.

> [!NOTE]
> So I am going to update the `main.py` file code later so that we can optimise it more and more and also Gemini did tell me that `pandas` is **too overkill** for the thing that I am trying to do.
> 
> > Its also the thing that is contributing to our *large* file size.
> 
> Therefore, we are later going to simply use the *built-in* `csv` module to do all of our processing for our `mouse_data.csv` file.

## Optimising Our C File

Okay, this is basically going to be me talking with Claude and learning how to actually optimise our `main.c` codebase so that we have more *security*, *better readability* and if we can, *further optimise* it.

> We are going to be solving *major* issues that is found inside the code and also fix shit up wherever we can!

### Create Process Instead Of Calling System

> [!INFO] Resource(s)
> - `CreateProcessW`: https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/nf-processthreadsapi-createprocessw
> - `STARTUPINFOW`: https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-startupinfow
> - `PROCESS_INFORMATION`: https://learn.microsoft.com/en-us/windows/win32/api/processthreadsapi/ns-processthreadsapi-process_information
> - `WaitForSingleObject`: https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-waitforsingleobject

We are now going to swap out the *dangerous* `system` function call to the `graphs.exe` executable file in favour of `CreateProcessW`.

- Therefore, remove the `system("graphs.exe");` function call in favour of this one:

```C
// INFO: declared at the top of the file --> global variables
// create array for new process start-up and process information
STARTUPINFOW si = {0};
PROCESS_INFORMATION pi = {0};

// other codes

  // other codes for `main` function
  
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

    // other codes for `main` function
```

### Using More Macro Definition

Added more macro definition so that we have a general place to change these "*settings*"

```C
// macro definition for how to long to run the program
#define TIME_TO_RUN 60.0
// macro definition for our CSV file name
#define CSV_FILENAME "mouse_data.csv"
// macro definition for our CSV writing "timer" / polling time
#define CSV_POLLING_WRITING_INTERVAL 50
```

> [!WARNING]
> I did also swap out the original values to use these macro "*variables*" in the actual code!

### Flush To Disk Immediately

- Add the following line of code just after writing ( *coordinates and mouse click counts* ) to `csv_file`:

```C
// global macro definition

// macro definition for our flushing count ( write immediately to disk )
#define CSV_FLUSH_WRITE_COUNT 500

      // other codes ( current inside the `WindowProc` function )

      // see below for "flushing" to disk immediately
      static int write_count = 0;

      // write the mouse data to the CSV file we created
      fprintf(csv_file, "%ld,%ld,%d,%d,%d,%d,%d,%d,%d\n", coordinates.x, coordinates.y, left, right, middle, back, forward, scroll_up, scroll_down);

      // force data to disk immediately ==> in case of program crash when "logging time" is increased
      // NOTE: writing to disk immediately ( instead of the buffer ) is costly ==> flush for every 500 writes
      if (++write_count % CSV_FLUSH_WRITE_COUNT == 0) {
        // write to the disk for these `CSV_FLUSH_WRITE_COUNT` lines
        fflush(csv_file);
      }
	  
      // other codes ( current inside the `WindowProc` function )
```

Claude Haiku ( *from Zed Agent Panel* ) told me to add this but the Claude Sonnet ( *from the actual Website* ) is telling me that its totally fine to skip this!

> Who should I thrust?

I **added** it because of the `TIME_TO_RUN`; the thing is when I plan to add a GUI to this project; I plan to also allow the user to configure the `TIME_TO_RUN` so that they can either *increase* or *decrease* the time for the program to log the mouse coordinates and button click "*counts*".

Therefore, if the user types '60 minutes' ( *current its in seconds so 3600 seconds* )... What is the program crashes to *for the logging* part or *in its entirety*.

Well, the user is going to still have some data inside the `mouse_data.csv` file and then non-technical user could simply double-click on the `graphs.exe` executable file ( *I should updated the `README.md` file* ) and still get an **output**!

> I am doing the trade-off of performance v/s data-loss here!

### Error Messages

Currently we were doing something like:

```C
// output appropriate messsage
printf("...", GetLastError());
```

But we all know that `printf` outputs to `stdout` but if we are *displaying* and error... We **should** display / redirect to `stderr`.

Therefore, switch out from `printf` to `fprintf` as `fprintf` can *output* to any stream.

> Something along the lines of this:

```C
// for example
fprintf(stderr, "\nFailed to get raw input data! Error: %lu\n", GetLastError());
// flush 'stderr' immediately for new ones
fflush(stderr);
```

### Function To Check If A File

> [!INFO] Resource(s)
> 
> - `GetCurrentDirectory`: https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-getcurrentdirectory
> - `PathCombineW`: https://learn.microsoft.com/en-us/windows/win32/api/shlwapi/nf-shlwapi-pathcombinew
> 	- `PathCchCombine`: https://learn.microsoft.com/en-us/windows/win32/api/pathcch/nf-pathcch-pathcchcombine
> - `GetFileAttributesW`: https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfileattributesw

I am now going to take Claude Haiku's advice and make a file checker function that is going to take a file name as argument and then check if that file name is present inside the current working directory.

> [!INFO]
> The function below is simply for me to learn about `os.path.getcwd`, `os.path.join` and `os.path.isfile` from Python but in C

> [!WARNING]
> The code below was written in a separate `test.c` file...
> 
> > I have yet to implement it inside our `main.c` code!

```C
#include <fileapi.h>
#include <processenv.h>
#include <windows.h>
#include <pathcch.h>
#include <stdio.h>
#include <wchar.h>
#include <wingdi.h>
#include <winnt.h>

#define CSV_FILENAME L"mouse_data.csv"
#define EXE_FILENAME L"graphs.exe"

// function to check if a file exists
int check_file_existance(const wchar_t *filename) {
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
    fprintf(stderr, "\nGet current working directory buffer is too small!\n");
    fflush(stderr);

    // exit the function with an error
    return 1;
  }

  // display the current working directory
  wprintf(L"\nCurrent Working Directory Buffer: %ls\n", getcwd_buffer);

  // part 2: combine the paths

  // create a buffer to hold the full path
  wchar_t fullpath[MAX_PATH] = {0};

  // try to combine paths by calling function
  HRESULT hr = PathCchCombine(fullpath, MAX_PATH, getcwd_buffer, filename);

  // check if we have been able to combine the paths
  if (FAILED(hr)) {
    // meaning that the paths were not able to be combined
    fprintf(stderr, "\nFailed to combine paths ( Error Code: 0x%08lx )!\n", (unsigned long)hr);
    fflush(stderr);

    // exit the function with an error
    return 1;
  }

  // display the full combined paths
  wprintf(L"\nCombined Path Buffer: %ls\n", fullpath);

  // part 3: check if the file actually exists in the directory

  // try to check if file exists by calling function
  DWORD attrs = GetFileAttributesW(fullpath);

  // check if the file actually exists
  if (attrs == INVALID_FILE_ATTRIBUTES) {
    // meaning that the file does NOT exists
    fprintf(stderr, "\nERROR: File does not exists!\n");
    fflush(stderr);

    // exit the function with an error
    return 1;
  } else {
    // meaning that the file does exists
    printf("\nFile Attributes Checker: File does exists!\n");
  }

  // part 4: check if the file is an actual file or directory
  if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
    // meaning that its actually a directory
    fprintf(stderr, "\nERROR: Path is a directory!\n");
    fflush(stderr);

    // exit the function with an error
    return 1;
  } else {
    // meaning that its actually a file
    printf("\nFile Checker: Path is a file!\n");
  }

  return 0;
}

// our main function
int main() {
  // call the function to check if a file exists
  check_file_existance(CSV_FILENAME);

  return 0;
}
```

## Optimising Our Python File

### Updated File Checker

Here is our updated `check_file_existence` checker function:

```python
# function to check if a file exists
def check_file_existence(filename: str) -> int:
  # create the full file path ( current working directory + file name )
  fullpath: str = path.join(getcwd(), filename)

  # check if full path exists is actually a file

  # NOTE: I know that these checks should have been in different `if` statements
  # but given my use-case; I think is a good tradeoff
  # INFO: today on 17/05/2025 @12:30... I tried creating a folder with the name `mouse_data.csv` file
  # and I did not expect this; Windows DOES let you create folders with `.` in the name!
  # thank god there is the `path.isfile` in Python!

  if path.exists(fullpath) and path.isfile(fullpath):
    print("\n" + "-" * 50, "\n")
    print(f"\tFile '{CSV_FILENAME}' exists!")
    print("\n" + "-" * 50, "\n")

    return 0

  # if the file has not been found!
  else:
    # INFO: again given my use-case whereby the `main.c` program is going to create a `mouse_data.csv` file
    print("\n" + "-" * 50, "\n")
    print(f"  File '{CSV_FILENAME}' does not exists or not a file!")
    print("\n" + "-" * 50, "\n")

    return 1
```

### Data Reading

> [!INFO] Resource(s)
> - Official Python Documentation: https://docs.python.org/3/library/csv.html#module-csv
> - Corey Schafer: https://youtu.be/q5uM4VKywbA?si=GMb2jAiYs62ZEzbY
> - Earthly: https://www.youtube.com/watch?v=H-hagjt65cE

Like I have been saying above, I want to stop using `pandas` for the CSV file **reading** part so that our `graphs.exe` does not have a big file size!

Therefore, I started watching Corey Schafer's video on CSV ( *I think it was 10 years ago... but still "Could be worst"* ).

But then I realised that he is showing the basics of the basics like the `with` context in Python and how to also write data to a CSV file ( *with the actual `csv` module* ).

Hence, I came across Earthly's video whereby he used something called the `DictReader`... Basically the `csv` module has **readers** and **writers** whereby it uses these *function* to be able to... Well *read* and *write* from / to a `.csv` file!

> I do suggest reading the official Python documentation as it talks about the "*reader object*"!

Therefore, this is the current program looks like:

```python
  # open the CSV file for reading the data gathered
  with open(CSV_FILENAME, 'r') as csv_file:
    # use the `DictReader` function / method to read data as a directionary
    mouse_data_reader = csv.DictReader(csv_file)

    # get the rows of the file
    mouse_data_rows = list(mouse_data_reader)

    # extract the required data from CSV file as lists

    # extract the actual coordinates gathered
    coord_x = [int(row["x"]) for row in mouse_data_rows]
    coord_y = [int(row["y"]) for row in mouse_data_rows]

    # extract the different "click" and "scroll" gathered
    left = [int(row["left"]) for row in mouse_data_rows]
    right = [int(row["right"]) for row in mouse_data_rows]
    middle = [int(row["middle"]) for row in mouse_data_rows]
    back = [int(row["back"]) for row in mouse_data_rows]
    forward = [int(row["forward"]) for row in mouse_data_rows]
    scroll_up = [int(row["scroll_up"]) for row in mouse_data_rows]
    scroll_down = [int(row["scroll_down"]) for row in mouse_data_rows]

    # create another array to sum-up those "click" and "scroll" data
    mouse_click_scroll_count = [
      sum(left),
      sum(right),
      sum(middle),
      sum(back),
      sum(forward),
      sum(scroll_up),
      sum(scroll_down),
    ]
```

The above code basically read the whole `mouse_data.csv` file as a dictionary and then converts *that* data into a list whereby we iterate through that list **nine fucking times**.

> [!NOTE]
> The above code was kind-of created by Claude.
> 
> I am going to be honest with you; I did not tell it to create the code; actually for the first part Claude generated this:
> 
> ```python
> import csv
> 
> # reading the CSV
> with open(FILE_NAME, newline="") as f:
>     reader = csv.DictReader(f)
>     rows = list(reader)
> 
> # extracting columns as lists instead of Series
> coords_x = [int(row["x"]) for row in rows]
> coords_y = [int(row["y"]) for row in rows]
> left     = [int(row["left"]) for row in rows]
> # ... etc
> ```
> 
> Very nice! This continued me to left my mind wander around and look at other resources. But then I provided it my initial code and then it gave me the above, "*full*", code block.
> 
> Now, talking back and forth about things like optimisation and stuff. The above code is kind-of shit because it loads in that whole `mouse_data.csv` file which could have more than 4000 lines of data and then we also do iterate over it **nine fucking times**.
> 
> Hence, I started asking for solutions and it says better to create those array first and then use `.append()` ( *as it has a time complexity of O(1)* ).
> 
> But then I started asking if that is really optimal and also asked it about Numpy. As for right now and even after this project finishes. I still have not dealt with Numpy as I would have liked to but I know that Numpy arrays are **super fast** and therefore its going to be more optimised if we used it!
> 
> > "But wait... Are you not are going to be increasing the size of `graphs.exe`?" I hear you ask!
> 
> The thing is `numpy` is already being installed and used as a **dependency** for `matplotlib`. Thus, in my eyes, it makes complete sense to use it!
> 
> > Therefore, I am going to now update the code so that we create a function for the "*gathering of data*" part and also use more optimise *stuffs*.

#### Python CSV File Reading Function

```python
# function to read and gather required mouse data from CSV file
def read_csv_file() -> dict:
  # open the CSV file for reading the data gathered
  # NOTE: `newline=''` added as per the documentation
  with open(CSV_FILENAME, 'r', newline='') as csv_file:
    # use the `DictReader` function / method to read data as a directionary
    mouse_data_reader = csv.DictReader(csv_file)

    # create the required Python lists
    # INFO: Python list's `.append` function is O(1) compared to numpy's O(n)
    # this is due to the fact that Python lists are dynamic while numpy's are fixed length
    # meaning that it has to do the C thing of creating a temporary array etc...
    coords_x: list[int] = []
    coords_y: list[int] = []
    left: list[int] = []
    right: list[int] = []
    middle: list[int] = []
    back: list[int] = []
    forward: list[int] = []
    scroll_up: list[int] = []
    scroll_down: list[int] = []

    # extract the required data from CSV file as lists
    for row in mouse_data_reader:
      # extract the actual coordinates gathered
      coords_x.append(int(row["x"]))
      coords_y.append(int(row["y"]))

      # extract the different "click" and "scroll" gathered
      left.append(int(row["left"]))
      right.append(int(row["right"]))
      middle.append(int(row["middle"]))
      back.append(int(row["back"]))
      forward.append(int(row["forward"]))
      scroll_up.append(int(row["scroll_up"]))
      scroll_down.append(int(row["scroll_down"]))

  # convert Python lists into numpy's array
  # INFO: I would have loved to add 'type annotations'
  # but then I would have import another thing from numpy
  coords_x = np.array(coords_x)
  coords_y = np.array(coords_y)
  left = np.array(left)
  right = np.array(right)
  middle = np.array(middle)
  back = np.array(back)
  forward = np.array(forward)
  scroll_up = np.array(scroll_up)
  scroll_down = np.array(scroll_down)

  # create another array to sum-up those "click" and "scroll" data
  mouse_click_scroll_count = [
    np.sum(left),
    np.sum(right),
    np.sum(middle),
    np.sum(back),
    np.sum(forward),
    np.sum(scroll_up),
    np.sum(scroll_down),
  ]

  # return the data gathered as a dictionary
  return {
    "x_coordinates": coords_x,
    "y_coordinates": coords_y,
    "button_click_scroll_count": mouse_click_scroll_count
  }
```

Therefore the usage is now like so:

```python
  # inside of main somwhere down below
  mouse_data_dict = read_csv_file()

  # call the function to generate our heatmap graph
  heatmap_generation(mouse_data_dict["x_coordinates"], mouse_data_dict["y_coordinates"])

  # call the function to generate our barchart graph
  barchart_generation(mouse_data_dict["button_click_scroll_count"])
```

### Pyinstaller Shit!

Now, we are going to actually try to compile the **new** `graphs.exe` file and we should see that we have reduced size of it!

So currently from above, we know that we have the following:

```powershell
pyinstaller --onedir `
--exclude-module tkinter `
--exclude-module tcl `
--exclude-module _tkinter `
--exclude-module sqlite3 `
--exclude-module IPython `
--exclude-module jedi `
--exclude-module matplotlib.tests `
--exclude-module numpy.tests `
--name graphs `
main.py
```

Then talking with Gemini again but specifically for Matplotlib module; it gave me the following command:

```powershell
pyinstaller --noconfirm --onefile --windowed `
    --exclude-module tkinter `
    --exclude-module _tkinter `
    --exclude-module matplotlib.backends.backend_tkagg `
    --exclude-module matplotlib.backends.backend_qt5agg `
    --exclude-module notebook `
    --exclude-module scipy `
    main.py
```

Therefore, I am first going go learn more about the `--windowed` flag and see what it does!

> [!INFO]
> - https://pyinstaller.org/en/stable/operating-mode.html
> 
> The `--windowed` flag, from what I can see, it going to not show up any **console** window!

Therefore, right now, I am going **combine** the above command and we see what will happen!

```powershell
pyinstaller --onefile `
--name graphs `
--exclude-module tkinter `
--exclude-module tcl `
--exclude-module _tkinter `
--exclude-module sqlite3 `
--exclude-module IPython `
--exclude-module jedi `
--exclude-module matplotlib.tests `
--exclude-module numpy.tests `
--exclude-module matplotlib.backends.backend_tkagg `
--exclude-module matplotlib.backends.backend_qt5agg `
--exclude-module notebook `
--exclude-module scipy `
--exclude-module pillow `
--exclude-module matplotlib.backends.backend_pdf `
--exclude-module matplotlib.backends.backend_ps `
--exclude-module matplotlib.backends.backend_svg `
--exclude-module matplotlib.backends.backend_webagg `
--exclude-module xmlrpc `
--exclude-module email `
--exclude-module http `
main.py
```

> [!INFO]
> I did ask Claude to give me more things to `exclude`.

> [!SUCCESS]
> Our `graphs.exe` file is now 33.8 MB...
> 
> > Fuck Yeah.

---

# Socials

- **Instagram**: https://www.instagram.com/s.sunhaloo
- **YouTube**: https://www.youtube.com/@s.sunhaloo
- **GitHub**: https://www.github.com/Sunhaloo

---

S.Sunhaloo
Thank You!