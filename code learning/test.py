# import the C low-level module to access 'Win32' API stuff
import ctypes

# function to get the user's screen resolution as a tuple
def get_user_screen_resolution():
  # INFO: for more information about this function
  # please do head to the shitty Microsoft Windows Win32 API docs

  # ignore display scaling ==> get the actual pixel coordinates of the screen
  ctypes.windll.user32.SetProcessDPIAware()

  # get the width and height of the user's screen
  width = ctypes.windll.user32.GetSystemMetrics(0)
  height = ctypes.windll.user32.GetSystemMetrics(1)

  # return the values as a tuple
  return width, height


# our main function
def main():
  # call the function to get the user's screen resolution
  print(get_user_screen_resolution())


# source the main function
if __name__ == "__main__":
  main()
