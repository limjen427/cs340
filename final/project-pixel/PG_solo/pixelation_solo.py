import numpy as np
from PIL import Image

# function to convert hex to rgb
def hex_to_rgb(hex):
    hex = hex.lstrip('#')
    hlen = len(hex)
    return tuple(int(hex[i:i+hlen//3], 16) for i in range(0, hlen, hlen//3))

# function to convert a palette in hex to rgb
def palette_to_rgb(palette):
    rgb_palette = []
    for color in palette:
        rgb_palette.append(hex_to_rgb(color))
    return rgb_palette

# approximate a color to the closest color in the palette
def approximate_color(color, palette_rgb):
    target = 0
    smallest_delta = 10000
    for i in range(len(palette_rgb)):
        r_delta = abs(color[0] - palette_rgb[i][0])
        g_delta = abs(color[1] - palette_rgb[i][1])
        b_delta = abs(color[2] - palette_rgb[i][2])
        delta = r_delta + g_delta + b_delta
        if delta < smallest_delta:
            smallest_delta = delta
            target = i
    return target

# function to convert a pixelated image to a array of palette indexes given width and height
def pixelation(filename, width, height, palette) :
    im = Image.open(filename, 'r')
    im_resized = im.resize((width, height))
    pixel_values = np.array(im_resized)
    palette_rgb = palette_to_rgb(palette)
    pixelated = np.zeros((height, width), dtype=np.uint8)
    for i in range(height):
        for j in range(width):
            pixelated[i][j] = approximate_color(pixel_values[i][j], palette_rgb)
    return pixelated