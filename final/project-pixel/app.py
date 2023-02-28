from flask import Flask, jsonify, render_template, request
import time
import requests

app = Flask(__name__)

@app.route('/', methods=["GET"])
def GET_index():
    '''Route for "/" (frontend)'''
    return render_template("index.html")


# middleware
# -p 10000
@app.route('/settings', methods=["GET"])
def GET_settings():

    return {
        "width": 100,
        "height": 50,
        "palette": ["c84113","13294b","ffa069","ffd0b4","ffffff","b34404","662702","0d1d35","717f93","b8bfc9","08101e"],
        "pixel_rate" : 3
    }

@app.route('/pixels', methods=["GET"])
def GET_pixels():
    
    return {"pixels": board}

@app.route('/update', methods=["PUT"])
def PUT_updates():
    update = request.json

    row = update["row"]
    col = update["col"]
    color = update["color"]


    return "", 500


def init_new_board(width, height, palette):

    board = {
        "init": True,
        "width": width,
        "height": height,
        "palette": palette,
        "pixels": [[0 for _ in range(width)] for _ in range(height)]
    }

    return board