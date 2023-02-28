from flask import Flask, jsonify, render_template, request
import requests
import random
import os
import sys
cur = os.path.dirname(os.path.realpath(__file__))
parent = os.path.dirname(cur)
sys.path.append(parent)
from pixelation import pixelation
import time


app = Flask(__name__)

register = requests.put('http://127.0.0.1:5000/register-pg', json={"name": "Jennifer Lim", "author": "Jennifer Lim", "secret": "coffee"})
id = register.json()['id']

# update = requests.put('http://127.0.0.1:5000/update-pg', json={"row": 200, "col": 400, "color": ["#FFFFFF", "#C0C0C0", "#808080", "#000000", "#735A49", "#DABD8D", "#905C50", "#8C8074"], "id": id})
# print(update)

setting = requests.get('http://127.0.0.1:5000/settings')
# print(setting.json())
set_row = setting.json()['height']
set_col = setting.json()['width']
palette = setting.json()['palette']
rate = setting.json()['pixel_rate']


pixels = requests.get('http://127.0.0.1:5000/pixels')
board = pixels.json()['pixels']

def pixel_generate():
    row = random.randint(0, set_row)
    col = random.randint(0, set_col)
    color = random.randint(0,len(palette))

    time.sleep(rate)

    if board[row][col] == color:
        print("same color")
    else:
        r = requests.put('http://127.0.0.1:5000/update-pixel', json={"row": row, "col": col, "color": color, "id": id})

pixel_generate()