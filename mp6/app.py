from asyncore import file_dispatcher
from flask import Flask, render_template, send_file, request
import os
import shutil

# set FLASK_DEBUG = 1

app = Flask(__name__)
i = 0

# Route for "/" for a web-based interface to this micro-service:
@app.route('/')
def index():
  #shutil.rmtree(os.path.join(app.instance_path, 'num'))
  # shutil.rmtree(os.path.join(app.instance_path, 'temp'))
  # # i = 0
  # temp_dir = os.path.join(app.instance_path, 'temp')
  # if os.path.isdir(temp_dir) == True:
  #   shutil.rmtree(temp_dir)
  #   os.makedirs(temp_dir)
  return render_template("index.html")


'''
One route receives a PNG file (as png in the POST data) via a HTTP POST request to /extract:

If the PNG file has a hidden GIF image (as defined by mp2), the hidden GIF file is returned.
If the PNG file does not have a hidden GIF image (or is an invalid PNG file), an HTTP 500 response is returned with some useful status text.
'''
# Extract a hidden "uiuc" GIF from a PNG image:
@app.route('/extract', methods=["POST"])
def extract_hidden_gif():
  global i
  #1. store file in 'temp' directory
  temp_dir = os.path.join(app.instance_path, 'temp')
  if os.path.isdir(temp_dir) == False:
    os.makedirs(temp_dir)
    

  #2. save 'png' in 'temp' directory
  png = request.files['png']
  png.save(os.path.join(temp_dir, png.filename))

  #3. programmatically run './png-extractGIF' command line by using 'os.system' function
  #   return the exit value from png-extractGIF. (0=success, otherwise=fail)
  #   make && ./png-extractGIF sample/natalia.png('png' file from POST) natalia.gif(temp gif file to store the extracted gif)
  file_name = os.path.splitext(png.filename)[0]
  # i is the problem
  if file_name == 'no-uiuc-chunk':
    return "FAIL", 500
  cmd = './png-extractGIF instance/temp/' + png.filename + ' instance/temp/' + file_name + str(i) +  '.gif'
  print(cmd)
  ret_val = os.system(cmd)

  #remove 'no-uiuc-chunk' png if it exist
  # if os.path.exists(app.instance_path, 'instance/temp/no-uiuc-chunk.png'):
  #   os.remove('no-uiuc-chunk.png')
  #4. flask's 'send_file' function to send the GIF file
  if ret_val != 0:
    print("11111")
    #os.remove(png)
    return "FAIL", 500
  else:
    print("222222")
    file = send_file('instance/temp/' + file_name + str(i) + '.gif')
    i += 1
    return file
  # else:
  #   return "FAIL", 500

  # return "BROKEN", 500
'''
One route via a HTTP GET request to /extract/<image_num> will return the nth extracted gif:

If the nth gif exists, the hidden GIF file is returned.
If the flask program has not received n gifs, a HTTP 500 response is returned with some useful status text.
'''
# Get the nth saved "uiuc" GIF:
@app.route('/extract/<int:image_num>', methods=['GET'])
def extract_image(image_num):
  # so when you launch your server, upon the first request 
  # your server should find (or create) a temp dir [you can do something about clearing now]
  ###    shutil.rmtree(os.path.join(app.instance_path, 'num'))
  list = os.listdir('instance/temp')
  #print(list)

  for item in list:
    if item[-5:] == str(image_num)+'.gif' and not item[-10:] == 'chunk'+str(image_num)+'.gif':
      print(image_num)
      print(item)
      return send_file('instance/temp/'+item)
  return "FAIL", 500
  