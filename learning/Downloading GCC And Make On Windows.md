---
id: Downloading GCC And Make On Windows
aliases: Downloading `gcc` and `make` on Windows using 'MYSYS'
tags:
  - C
author: S.Sunhaloo
date: 2026-03-10
status: Completed
---

> [!WARNING]
> This whole thing was AI generated!

# Installing GCC and Make on Windows using MSYS2

This guide explains how to install:

- **GCC (C compiler)**
- **GNU Make (build automation tool)**

on **Windows** using **MSYS2**, and how to configure your system so the tools are available globally from any terminal (Command Prompt, PowerShell, or editors like Zed).

MSYS2 provides a Unix-like development environment for Windows and includes the **pacman** package manager, similar to Arch Linux.

---

# 1. Install MSYS2

## Download

Visit the official website:

https://www.msys2.org/

Download the latest installer:

```
msys2-x86_64-*.exe
```

Run the installer and follow the default installation steps.

Recommended installation directory:

```
C:\msys64
```

Using the default path helps avoid issues with development tools.

---

# 2. Update MSYS2

After installing MSYS2:

1. Open the **MSYS2 MINGW64** terminal.
2. Update the package database and core system:

```bash
pacman -Syu
```

If prompted to close the terminal, do so.

Then reopen **MSYS2 MINGW64** and run:

```bash
pacman -Su
```

This ensures the entire MSYS2 environment is fully updated.

---

# 3. Install GCC

Install the **MinGW-w64 GCC toolchain**:

```bash
pacman -S mingw-w64-x86_64-gcc
```

This installs:

- gcc (C compiler)
- g++ (C++ compiler)
- binutils
- runtime libraries

These tools allow you to compile **native Windows executables**.

---

# 4. Install GNU Make

Install GNU Make with:

```bash
pacman -S mingw-w64-x86_64-make
```

The executable provided by MSYS2 is usually named:

```
mingw32-make.exe
```

This naming avoids conflicts with other versions of Make on Windows.

---

# 5. Install `clangd` (Recommended for Editors)

Many modern editors use **clangd** for C language features such as:

- syntax highlighting
- code completion
- diagnostics
- error detection

Install clangd using:

```bash
pacman -S mingw-w64-x86_64-clang-tools-extra
```

This installs:

```
clangd.exe
```

inside:

```
C:\msys64\mingw64\bin
```

---

# 6. Verify the Installation

Check that GCC is installed:

```bash
gcc --version
```

Example output:

```
gcc (RevX, Built by MSYS2) 14.x.x
```

Check GNU Make:

```bash
mingw32-make --version
```

Check clangd:

```bash
clangd --version
```

If all commands work, the installation succeeded.

---

# 7. Add GCC, Make and Clangd to the PATH

To make the tools accessible from **any terminal**, add the MinGW binaries directory to your system PATH.

Directory to add:

```
C:\msys64\mingw64\bin
```

## Steps

1. Press **Windows Key**
2. Search for:

```
Environment Variables
```

3. Open:

```
Edit the system environment variables
```

4. Click:

```
Environment Variables
```

5. Under **User Variables**, select:

```
Path
```

6. Click **Edit**

7. Add a new entry:

```
C:\msys64\mingw64\bin
```

8. Press **OK** to close all windows.

---

# 8. Restart Your Terminal

Close and reopen your terminal.

Test again:

```bash
gcc --version
```

```bash
mingw32-make --version
```

```bash
clangd --version
```

All commands should now work globally.

---

# 9. Fix for Zed Text Editor (Important)

When using the **Zed Text Editor**, you may see errors like:

```
'stdio.h' file not found
printf undeclared
```

even though the program compiles correctly with GCC.

This happens because Zed may use a different `clangd` that cannot detect the MinGW toolchain.

## Solution

Tell Zed to use the **MSYS2 clangd**.

Open your Zed configuration file:

```
~/.config/zed/settings.json
```

Add the following configuration:

```json
{
  "lsp": {
    "clangd": {
      "binary": {
        "path": "C:/msys64/mingw64/bin/clangd.exe"
      }
    }
  }
}
```

Restart Zed.

This ensures Zed uses the **correct clangd instance**, which understands the MinGW headers.

After this fix:

- `stdio.h` errors disappear
- `printf` is correctly recognised
- code completion works properly

---

# 10. Optional: Enable the `make` Command

Since MSYS2 names the executable `mingw32-make`, you may want the standard Linux command `make`.

Navigate to:

```
C:\msys64\mingw64\bin
```

Create a copy of:

```
mingw32-make.exe
```

Rename the copy to:

```
make.exe
```

You can now run either:

```bash
make
```

or

```bash
mingw32-make
```

---

# 11. Test Your C Compiler

Create a file called:

```
main.c
```

```c
#include <stdio.h>

int main(void) {
    printf("Hello World\n");
    return 0;
}
```

Compile the program:

```bash
gcc main.c -o program
```

Run it:

```bash
./program
```

Expected output:

```
Hello World
```

---

# 12. Example Makefile

Create a file named:

```
Makefile
```

```make
CC = gcc
CFLAGS = -Wall -Wextra -std=c17

program: main.c
	$(CC) $(CFLAGS) main.c -o program

clean:
	rm -f program
```

Build the program:

```bash
make
```

or

```bash
mingw32-make
```

---

# Final Result

After completing these steps you will have a working Windows C development environment:

```
C:\msys64\mingw64\bin
├── gcc.exe
├── g++.exe
├── clangd.exe
├── mingw32-make.exe
├── gdb.exe
└── ld.exe
```

By adding this directory to PATH, the tools become available globally across your system.

This setup works well with editors such as:

- Zed
- VS Code
- Neovim
- Sublime Text

---

# Socials

- **Instagram**: https://www.instagram.com/s.sunhaloo
- **YouTube**: https://www.youtube.com/@s.sunhaloo
- **GitHub**: https://www.github.com/Sunhaloo

---

S.Sunhaloo
Thank You!