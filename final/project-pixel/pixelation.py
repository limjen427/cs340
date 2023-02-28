from PIL import Image
from math import sqrt

def hex_to_rgb(hex):
    tmp = hex[1:7]
    return tuple(int(tmp[i:i+2], 16) for i in (0, 2, 4))

def compare(palette, r, g, b):
    color_diffs = []
    for i in range(len(palette)):
        pr, pg, pb = hex_to_rgb(palette[i])
        color_diff = sqrt((r - pr)**2 + (g - pg)**2 + (b - pb)**2)
        color_diffs.append((color_diff, i))
    return min(color_diffs)[1]

def pixelation(filename, board, width, height, row, col, palette):
    img = Image.open(filename).resize((width, height)).rotate(270).transpose(Image.FLIP_LEFT_RIGHT)

    r, g, b = img.getpixel((row,col))
    index = compare(palette, r, g, b)
    if board[row][col] != index:
        return index
    else:
        return -10