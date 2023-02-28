import requests
import os
import sys
cur = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(cur)
sys.path.append(parent)
from pixelation import pixelation
import time

filename = "color.jpg"
# MIDDLEWARE_URL = "http://fa22-cs340-adm.cs.illinois.edu:34999/"
MIDDLEWARE_URL = "http://fa22-cs340-049.cs.illinois.edu:3340/"
SECRET = "secret"
# SECRET = "PG+anlsaW0z+AeMYQ3DRZWY1Rwp19_bou8rdx3VHeu"

register_json = {
    "name": "Illini Union",
    "author": "Jennifer Lim",
    "secret": SECRET
}

register = requests.put(MIDDLEWARE_URL + '/register-pg', json=register_json)
id = register.json()['id']

setting = requests.get(MIDDLEWARE_URL+'/settings')
set_row = setting.json()['height']
set_col = setting.json()['width']
palette = setting.json()['palette']

pixels = requests.get(MIDDLEWARE_URL + '/pixels', json={"id": id})
board = pixels.json()['pixels']

def pixel_img():
    # for row in range(Y_COORD, set_row):
    #     for col in range(X_COORD, set_col):
    for row in range(set_row):
        for col in range(set_col):
            index = pixelation(filename, board, set_row, set_col, row, col, palette)
            if index != -10:
                print(row, col)
                update_json = {
                    "id": id,
                    "row": row,
                    "col": col,
                    "color": index
                }
                r = requests.put(MIDDLEWARE_URL+ '/update-pixel', json=update_json)
                rate = r.json()["rate"]
                time.sleep(rate/1000)
             
while True:
    pixel_img()
    


