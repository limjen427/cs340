from crypt import methods
from flask import Flask, jsonify, send_file, render_template, request
import requests
import os
import io
import boto3
import base64

app = Flask(__name__)

@app.route('/')
def index():
    return render_template('index.html')

@app.route('/all')
def all():
    return render_template('all.html')

# /mandelbrot/<colormap>/<real>:<imag>:<height>:<dim>:<iter>
# /mandelbrot/twilight_shifted/0.36:-0.09:0.01:512:64

colormap = 'cividis'
real = -0.7435
imag = 0.126129
height = 0.00018972901232843951
dim = 256
iter = 512
#(x,y), unit height, render dimensions, maximum iterations of the Mandelbrot set

# moves the center of the image up by 25% of the current height
@app.route('/moveUp', methods = ['POST'])
def move_up():
    global imag
    move_height = height * 0.25
    imag -= move_height
    return {"imag": imag}, 200

#moves the center of the image down by 25% of the current height
@app.route('/moveDown', methods=['POST'])
def move_down():
    move_height = height * 0.25
    imag += move_height
    return 200

#moves the center of the image to the left by 25% of the current height
@app.route('/moveLeft', methods=['POST'])
def move_left():
    move_height = height * 0.25
    real -= move_height
    return 200

# moves the center of the image to the right by 25% of the current height
@app.route('/moveRight', methods=['POST'])
def move_right():
    move_height = height * 0.25
    real += move_height
    return 200

# modifies the height by a factor of 1 / 1.4
@app.route('/zoomIn', methods=['POST'])
def zoom_in():
    height = height * (1/1.4)
    return 200

# modifies the height by a factor of 1.4
@app.route('/zoomOut', methods=['POST'])
def zoom_out():
    height = height * (1.4)
    return 200
    # return None

#modifies the dim of the image by a factor of 1 / 1.25
@app.route('/smallerImage', methods=['POST'])
def smaller_image():
    dim = dim * (1/1.25)
    return 200

#modifies the dim of the image by a factor of 1.25
@app.route('/largerImage', methods=['POST'])
def larger_image():
    dim = dim * (1.25)
    return 200

# modifies the iter of the image by a factor of 2
@app.route('/moreIterations', methods=['POST'])
def more_iterations():
    iter = iter * (2)
    return 200

# modifies the iter of the image by a factor of 1 / 2
@app.route('/lessIterations', methods=['POST'])
def less_iterations():
    iter = iter * (1/2)
    return 200

# changes the colormap to be equal to the colormap value in the JSON in the request’s body. 
# For example, a sample request’s body to POST /changeColorMap is: {"colormap": "inferno"}
@app.route('/changeColorMap', methods=['POST'])
def change_colormap():
    r = requests.get(os.getenv('MANDELBROT_MICROSERVICE_URL'))
    print(r.json()['colormap'])
    colormap = r.json()['colormap']
    return 200

@app.route('/mandelbrot', methods=['GET'])
def mandelbrot():
    return None

@app.route('/storage', methods=['GET'])
def storage():
    return None

@app.route('/resetTo', methods=['POST'])
def reset_to():
    print(os.getenv('MANDELBROT_MICROSERVICE_URL'))
    url = os.getenv('MANDELBROT_MICROSERVICE_URL')
    r = requests.get(f'{url}/mandelbrot/{colormap}/{real}:{imag}:{height}:{dim}:{iter}')
    print("reach here?")
    print(r.json())
    print("what about here?")
    return {
        "colormap": colormap,
        "real": real,
        "imag": imag,
        "height": height,
        "dim": dim,
        "iter": iter
    }, 200

@app.route('/getState', methods=['GET'])
def get_state():
    return None