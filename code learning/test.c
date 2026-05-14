#include <fileapi.h>
#include <windows.h>
#include <pathcch.h>
#include <stdio.h>
#include <wchar.h>

#define CSV_FILENAME L"mouse_data.csv"
#define EXE_FILENAME L"graphs.exe"

// function to get the current working directory (wide)
static int getcwd_w(wchar_t result_buffer[MAX_PATH]) {
  DWORD result = GetCurrentDirectoryW(MAX_PATH, result_buffer);

  if (result == 0) {
    fprintf(stderr, "\nFailed to get the current working directory %lu!\n", GetLastError());
    fflush(stderr);
    return 1;
  }

  // If the buffer is too small, the return value is the required size (in chars)
  if (result >= MAX_PATH) {
    fprintf(stderr, "\nFailed to get the current working directory %lu!\n", GetLastError());
    fflush(stderr);
    return 1;
  }

  return 0;
}

// function to combine 2 paths (wide)
static int combine_path_w(
  wchar_t result_fullpath[MAX_PATH],
  const wchar_t *cwd_path,
  const wchar_t *file_name
) {
  HRESULT hr = PathCchCombine(result_fullpath, MAX_PATH, cwd_path, file_name);

  if (FAILED(hr)) {
    fprintf(stderr, "\nFailed to combine paths (HRESULT=0x%08lx)\n", (unsigned long)hr);
    fflush(stderr);
    return 1;
  }

  return 0;
}

// our main function
int main(void) {
  wchar_t cwd_buf[MAX_PATH] = {0};

  if (getcwd_w(cwd_buf) != 0) {
    return 1;
  }

  // print the cwd (wide string)
  wprintf(L"%ls\n", cwd_buf);

  wchar_t fullpath_combined[MAX_PATH] = {0};

  if (combine_path_w(fullpath_combined, cwd_buf, CSV_FILENAME) != 0) {
    return 1;
  }

  wprintf(L"\n%ls\n\n", fullpath_combined);

  // check if the fullpath is actually a file
  DWORD attrs = GetFileAttributesW(fullpath_combined);

  if (attrs == INVALID_FILE_ATTRIBUTES) {
      fprintf(stderr, "GetFileAttributesW failed: %lu - Files does not exists!\n", GetLastError());
      fflush(stderr);

      return 1;
  }

  if (attrs & FILE_ATTRIBUTE_DIRECTORY) {
      wprintf(L"Path is a directory\n");
      return 1;
  }

  wprintf(L"Path is a file\n");

  return 0;
}
