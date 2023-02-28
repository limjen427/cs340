import requests
import numpy as np
import time
from pixelation import *
from PIL import Image

# The image to be drawn
filename = "resizedYoshi.jpg"
IMAGE_DRAW = Image.open(filename)
WIDTH, HEIGHT = IMAGE_DRAW.size

# the x-coordinate of the top left corner of the image (starting point)
X_COORD = 125
# the y-coordinate of the top left corner of the image (starting point)
Y_COORD = 20

# Microservice address to middleware
MIDDLEWARE_URL = "http://fa22-cs340-adm.cs.illinois.edu:34340/"

settings_ = requests.get(MIDDLEWARE_URL + "/settings")
settings = settings_.json()
BOARD_WIDTH = settings["width"]
BOARD_HEIGHT = settings["height"]
PALETTE = settings["palette"]

RESIZED_HEIGHT = BOARD_HEIGHT // 5
RESIZED_WIDTH = BOARD_HEIGHT // 5
ranges = {
    "nkapse2": range(0, int((1/5) * RESIZED_HEIGHT)),
    "jsku2": range(int((1/5) * RESIZED_HEIGHT), int((2/5) * RESIZED_HEIGHT)),
    "kl35": range(int((2/5) * RESIZED_HEIGHT), int((3/5) * RESIZED_HEIGHT)),
    "jylim3": range(int((3/5) * RESIZED_HEIGHT), int((4/5) * RESIZED_HEIGHT)),
    "ahjaber3": range(int((4/5) * RESIZED_HEIGHT), RESIZED_HEIGHT),
}

print("Enter NetID: ")
netid = "jylim3"

print("Enter secret: ")
secret = "PG+anlsaW0z+AeMYQ3DRZWY1Rwp19_bou8rdx3VHeu"

RANGE_TO_DEFEND = ranges[netid]

# Dictionary stores the registration information for this microservice
info_dict = {
    "name": "Yoshi",
    "author": "CS340_Taylors_Version",
    "secret": secret
}

# Register this PG with the middleware
response = requests.put(MIDDLEWARE_URL + "/register-pg", json=info_dict)
response = response.json()
TOKEN = response['id']

IMAGE_DRAW = pixelation(filename, RESIZED_HEIGHT, RESIZED_WIDTH, PALETTE)
Image_draw_numpy = np.array(IMAGE_DRAW)

# Attempts to draw and maintain the image in the pixel board at the given coordinates
def defend_image() :
    # Get the current state of the CS 340 pixel board
    data = { "id": TOKEN }
    r = requests.get(MIDDLEWARE_URL + "/pixels", json=data)
    pixels = r.json()
    pixelboard_state = np.array(pixels['pixels'], dtype=np.uint8)

    # counter = 0

    for x in RANGE_TO_DEFEND:
        for y in range(Y_COORD, Y_COORD + RESIZED_HEIGHT):
            # if counter % 20 == 0:
            r = requests.get(MIDDLEWARE_URL + "/pixels", json=data)
            pixels = r.json()
            pixelboard_state = np.array(pixels['pixels'], dtype=np.uint8)

            # counter += 1
            pixel_color = IMAGE_DRAW[y - Y_COORD][x]
            if pixelboard_state[y][x + X_COORD] != np.uint8(pixel_color):
                print(y, x + X_COORD)
                response_dict = {
                    "id": TOKEN,
                    "row": y,
                    "col": x + X_COORD,
                    "color": int(pixel_color)
                }
                try:
                    r = requests.put(MIDDLEWARE_URL + "/update-pixel", json = response_dict)
                    delay_time = r.json()["rate"]
                    time.sleep(delay_time / 1000)
                except Exception as ex:
                    print(f"{str(ex)}")
    return "no change" 

while True:
    defend_image()
