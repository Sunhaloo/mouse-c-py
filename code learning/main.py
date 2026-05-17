# import the `getcwd` function and `path` sub-module from 'os' module
from os import getcwd, path

# import the 'csv' module that is built into Python
import csv
from tkinter.constants import W

# import the numpy library and use `np` as alias
import numpy as np

# import the whole 'matplotlib' module so as to change backend
import matplotlib

# WARNING: changing the backend used by matplotlib
# this way, we are further optimising and improving reliability of
# our program for CI/CD stuffs as we only save images
# our program does not really open up the "Tkiner GUI" to see the image
matplotlib.use("Agg")

# import the 'matplotlib' module again but only to get graphs functions
import matplotlib.pyplot as plt


# define our constants
CSV_FILENAME: str = "mouse_data.csv"
BARCHART_IMG: str = "mouse_clicks_count.png"
HEATMAP_IMG: str = "mouse_coords_heatmap.png"
SAVE_IMG_DIR: str = path.expanduser("~/Downloads/")


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

# function to be able to generate the heatmap graph
def heatmap_generation(coords_x, coords_y):
  # create the figure and the axes
  fig, axes = plt.subplots()

  # move the x-axis from the bottom to the top
  axes.spines["bottom"].set_position(("axes", 1.0))

  # make the x-axis "ticks" and labels appear at the top
  axes.xaxis.set_ticks_position("top")
  axes.xaxis.set_label_position("top")

  # add more description to the graph / image
  axes.set_title("Mouse Movement Heatmap ( Screen Representation )")

  # label the axes
  axes.set_xlabel("X coordinate")
  axes.set_ylabel("Y coordinate")

  # get the data to compute the histogram
  # NOTE: `xedges` and `yedges` are arrays and the `heatmap` is a 2D-array
  heatmap, xedges, yedges = np.histogram2d(coords_x, coords_y, bins=30)

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
      SAVE_IMG_DIR + HEATMAP_IMG,
      dpi=600,
      bbox_inches="tight",
  )


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
  mouse_data_dict = read_csv_file()

  print(mouse_data_dict["x_coordinates"])

  # call the function to generate our heatmap graph
  heatmap_generation(mouse_data_dict["x_coordinates"], mouse_data_dict["y_coordinates"])

# source the main function
if __name__ == "__main__":
  main()
