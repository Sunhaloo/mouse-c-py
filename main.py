# import `path` from the `os` module
from os import getcwd, path

# import the matplotlib 2D plots to make the bar chart and heatmap graph
import matplotlib.pyplot as plt

# import the entire pandas library and use `pd` as alias
import pandas as pd

# import the numpy library and use `np` as alias
import numpy as np

# global constants
FILE_NAME: str = "mouse_data.csv"
BAR_CHART_PNG_NAME: str = "mouse_clicks.png"
HIST2D_CHART_PNG_NAME: str = "mouse_coordinates.png"
IMSHOW_PNG_NAME: str = "mouse_coordinates_screen.png"
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
        print("\n" + "-" * 50, "\n")
        print(f"CSV File '{file_name}' is MISSING at '{getcwd()}'!")

        exit(1)

    # if the file has been found
    print("\n" + "-" * 50, "\n")
    print(f"CSV File '{file_name}' has been FOUND at '{getcwd()}'!")
    print("\n" + "-" * 50, "\n")


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
      SAVE_FILE_DIR + IMSHOW_PNG_NAME,
      dpi=600,
      bbox_inches="tight",
)



# function to be able to create the mouse coordinates heatmap graph / image
def coordinates_heatmap(coords_x: pd.Series, coords_y: pd.Series):
    # create the figure and the axes
    fig, axes = plt.subplots()

    # invert y axis for the heatmap
    axes.invert_yaxis()

    # add more description to the graph / image
    axes.set_title("Mouse Movement Heatmap")

    # label the axes
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
    print("\n" + "=" * 50, "\n")
    print("\tPython Program Starting!")
    print("\n" + "=" * 50, "\n")

    # call the function to check if the CSV data file exists
    # WARNING: I have not check this; but I think you are going to have to comment
    # this file checker function below if you are using Google Collab!
    csv_file_existance(FILE_NAME)

    # create a pandas object from the csv file
    csv_file = pd.read_csv(FILE_NAME)

    # call the function to create the 'imshow' graph - 1:1 mouse-screen
    coordinates_heatmap_imshow(csv_file["x"], csv_file["y"])

    # call the function to create the 'hist2d' graph / heatmap-like graph
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
    print("\n" + "-" * 50, "\n")

    print("\n" + "=" * 50, "\n")
    print("\tPython Program Ending!")
    print("\n" + "=" * 50, "\n")


if __name__ == "__main__":
    main()
