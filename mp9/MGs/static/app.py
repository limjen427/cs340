from flask import Flask, render_template, request, jsonify
import requests


app = Flask(__name__)


# same maze
@app.route('/generate', methods=['GET'])
def new_maze():
    return jsonify({"geom": ["98a0a8c", "1644424", "5020244", "4464640", "5202064", "1420604", "3220226"]})

r = requests.put('http://fa22-cs340-adm.cs.illinois.edu:34000/addMG', json= { "name": "!!bot bot!!", "url": "http://fa22-cs340-049.cs.illinois.edu:6000/", "author": "Jennifer Lim", "weight": 1 })
print(r.status_code)


''''
ERROR: pip's dependency resolver does not currently take into account all the packages that are installed. This behaviour is the source of the following dependency conflicts.
spyder 4.2.5 requires pyqtwebengine<5.13, which is not installed.
conda-repo-cli 1.0.4 requires pathlib, which is not installed.
tensorboard 2.9.0 requires setuptools>=41.0.0, but you have setuptools 3.3 which is incompatible.
spyder 4.2.5 requires pyqt5<5.13, but you have pyqt5 5.15.0 which is incompatible.
spyder 4.2.5 requires setuptools>=39.0.0, but you have setuptools 3.3 which is incompatible.
ipython 7.22.0 requires setuptools>=18.5, but you have setuptools 3.3 which is incompatible.
'''