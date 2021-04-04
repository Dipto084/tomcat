#!/usr/bin/python3

"""
Filename: mcglib_visualizer.py
Author: Adi Banerjee
Purpose: This program will vizualize the procedurally generated map from the semantic_map.json file. 
         It does not accept alternate filenames. The visualized map is saved in the same directory as
         script with the name "map_plot.pdf". A graph of the structures is also drawn and saved as "map_graph.pdf".
"""

import matplotlib.patches as patches
from matplotlib.collections import PatchCollection
import matplotlib.pyplot as plt
import numpy as np
import json
import pygraphviz as pgv
import os
import argparse

# Get command line arguments
parser = argparse.ArgumentParser()
parser.add_argument(
    "--color_patches",
    help="Color the plotted patches (for supported materials). Defaults to False.",
    default=False,
    type=bool,
)
parser.add_argument(
    "--font_size", help="Size of label fonts. Defaults to 3.", default=3, type=int
)
parser.add_argument(
    "--rankdir",
    help='Sets direction of graph layout. If rankdir="TB", the graph is laid out from top to bottom, i.e., directed edges tend to go from top to bottom. By default, graphs are laid out from left to right ("LR").',
    default="LR",
    type=str,
)
args = parser.parse_args()


# Colours to use for each material
color_index = {
    "planks": "#cc7904",
    "glowstone": "#a4aba9",
    "cobblestone": "#8f8e89",
    "lava": "#e81f15",
    "water": "#1569e8",
    "sand": "#ffd080",
    "gravel": "#face2f",
}

# Read the semantic_map.json file
semantic_json = "./semantic_map.json"
with open(semantic_json) as f:
    data = json.load(f)
locations = data["locations"]


# Visualization Logic
fig, ax = plt.subplots()
patch_list = []  # The rectangular patches for each building will be stored here
edge_list = []  # Edges for our graph

for location in locations:
    # We dont wan't to draw blocks or blank boxes since
    if location["bounds"]["type"] != "block" and not location["id"] == "blank_box":

        child_locations = location["child_locations"]
        for child_location in child_locations:
            edge_list.append(location["id"] + " -> " + child_location)

        # Find the coordinates of the AABB
        coordinate_list = location["bounds"]["coordinates"]
        top_left_coords = coordinate_list[0]
        bottom_right_coords = coordinate_list[1]
        x1 = int(top_left_coords["x"])
        z1 = int(top_left_coords["z"])
        x2 = int(bottom_right_coords["x"])
        z2 = int(bottom_right_coords["z"])

        # The width and height of the rectanglular patch that will represent it
        width = abs(x2 - x1)
        height = abs(z2 - z1)

        # Make and add the patch to our list
        if(args.color_patches):
            patch_color = color_index.get(location["material"], False)
            rect = patches.Rectangle(
                (x1, z1), width, height, linewidth=1, edgecolor="black", fill=patch_color
            )
        else:
            rect = patches.Rectangle(
                (x1, z1), width, height, linewidth=1, edgecolor="black", fill=False
            )

        patch_list.append(rect)

        # Annotate the patch with the name of the location
        patch_center_z = z1 + height / 2
        ax.annotate(
            location["id"],
            (x2, patch_center_z),
            color="blue",
            weight="bold",
            fontsize=args.font_size,
            ha="center",
            va="center",
        )

# Use patch collection to add the patches to plot
p = PatchCollection(patch_list, match_original=True)
ax.add_collection(p)

# Some settings
ax.set_aspect("equal")
ax.yaxis.set_ticks_position("right")
ax.xaxis.set_label_coords(0.5, -0.1)
ax.yaxis.set_label_coords(1.150, 0.5)

plt.margins(0, 0)
plt.gca().invert_xaxis()
plt.xlabel("X")
plt.ylabel("Z")

plt.savefig("map_plot.pdf")
plt.close()

# Here we create the graph
# Create a dot file representation
f = open("tempDotFile.dot", "w")
f.write("digraph sample {\n\t")
for edge in edge_list:
    f.write(edge + "\n\t")
f.write("}")
f.close()

# Use the dot file to create the digraph
G = pgv.AGraph("tempDotFile.dot")
if args.rankdir == "TB":
    G.graph_attr["rankdir"] = args.rankdir
else:
    G.graph_attr["rankdir"] = "LR"
G.layout(prog="dot")
G.draw("map_graph.pdf")

# Remove the dot file
os.remove("tempDotFile.dot")
