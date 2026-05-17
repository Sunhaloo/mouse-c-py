# import the `getcwd` function and `path` sub-module from 'os' module
from os import getcwd, path

# import the 'csv' module that is built into Python
import csv
from tkinter.constants import W

# import the numpy library and use `np` as alias
import numpy as np


# define our constants
CSV_FILENAME: str = "mouse_data.csv"
BARCHART_IMG: str = "mouse_clicks_count.png"
HEATMAP_IMG: str = "mouse_coords_heatmap.png"
SAVE_IMG_DIR: str = "~/Downloads"

# create a global dictionary to hold data required by matplotlib
MOUSE_DATA_DICT: dict = {}


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
    print(f"\tFile '{filename}' exists!")
    print("\n" + "-" * 50, "\n")

    return 0

  # if the file has not been found!
  else:
    # INFO: again given my use-case whereby the `main.c` program is going to create a `mouse_data.csv` file
    print("\n" + "-" * 50, "\n")
    print(f"  File '{CSV_FILENAME}' does not exists or not a file!")
    print("\n" + "-" * 50, "\n")

    return 1


# function to read and gather required mouse data from CSV file
def read_csv_file():
  # open the CSV file for reading the data gathered
  with open(CSV_FILENAME, 'r') as csv_file:
    # use the `DictReader` function / method to read data as a directionary
    mouse_data_reader = csv.DictReader(csv_file)

    # create the required Python lists
    # INFO: Python list's `.append` function is O(1) compared to numpy's O(n)
    # this is due to the fact that Python lists are dynamic while numpy's are fixed length
    # meaning that it has to do the C thing of creating a temporary array etc...
    coord_x: list[int] = []
    coord_y: list[int] = []
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
      coord_x.append(int(row["x"]))
      coord_y.append(int(row["y"]))

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
  coord_x = np.array(coord_x)
  coord_y = np.array(coord_y)
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

  # add to global dictionary for easier return ( for the current function )
  # and also for each of access and use in the different "matplotlib" functions
  MOUSE_DATA_DICT['x_coordinates'] = coord_x
  MOUSE_DATA_DICT['y_coordinates'] = coord_y
  MOUSE_DATA_DICT["counts"] = mouse_click_scroll_count


# our main function
def main():
  # display a little message that the Python program is starting
  print("\n\n" + "=" * 50, "\n")
  print(f"\t    Python Program Starting")
  print("\n" + "=" * 50, "\n")

  # check if the CSV file is present inside the current working directory
  if check_file_existence(CSV_FILENAME) == 1:
    # meaning that the file has not been found ==> simply exit the program
    print("\n" + "-" * 50, "\n")
    print("\t\tExiting Program...")
    print("\n" + "-" * 50, "\n")

    # exit the program with "error"!
    exit(1)

  print("\n" + "=" * 50, "\n")
  print("     Gathering Required Data From CSV File")
  print("\n" + "=" * 50, "\n")

  # call the function to read data from the CSV file and populate dictionary
  read_csv_file()


# source the main function
if __name__ == "__main__":
  main()
