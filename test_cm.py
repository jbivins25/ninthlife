import json
import re


def parse_gb_header(file_path):
    with open(file_path, "r") as f:
        content = f.read()

    # 1. Extract Width and Height constants using regex
    width_match = re.search(r"cm_entryway_WIDTH\s*=\s*(\d+);", content)
    height_match = re.search(r"cm_entryway_HEIGHT\s*=\s*(\d+);", content)

    width = int(width_match.group(1)) if width_match else None
    height = int(height_match.group(1)) if height_match else None

    # 2. Clean the array text so JSON can read it
    # Find everything between the first outer '{' and the last outer '}'
    array_match = re.search(r"cm_entryway_map\[.*\]\[.*\]\s*=\s*\{([\s\S]*)\};", content)

    if not array_match:
        raise ValueError("Could not find the map array in the header file.")

    array_str = array_match.group(1)

    # Replace C-style inner braces '{ }' with JSON standard square brackets '[ ]'
    array_str = array_str.replace("{", "[").replace("}", "]")

    # Load it directly as a nested Python list
    tilemap = json.loads(f"[{array_str}]")

    return width, height, tilemap


# --- How to use it ---
# (Assuming your header file is saved as 'cm_entryway.h')
width, height, map_data = parse_gb_header("include/cm_entryway.h")

from PIL import Image
import numpy as np

collision_map = np.array(map_data)

w, h = collision_map.shape

image_data = np.zeros((w,h,4), dtype=np.uint8)

for x in range(w):
	for y in range(h):
		r = g = b = int((collision_map[x][y]/256)*255)
		if r == 0:
			a = 0
		else:
			a = 255
		image_data[x,y] = [r,g,b,a]

image = Image.fromarray(image_data)
image.save("test_cm.png")
