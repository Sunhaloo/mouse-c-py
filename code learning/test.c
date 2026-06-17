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
