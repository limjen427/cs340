from flask import Flask, render_template, request, jsonify
import requests
import random


app = Flask(__name__)


# same maze
@app.route('/generate', methods=['GET'])
def new_maze():
    num = random.randrange(56) # 0 - 3

    if num == 0:
        return jsonify({"geom": ["98a0a8c", "1644424", "5020244", "4464640", "5202064", "1420604", "3220226"]})
    elif num == 1:
        return jsonify({"geom": ["9aa2aac", "5020244", "5402444", "4444440", "5422644", "5220264", "3220226"]})
    elif num == 2:
        return jsonify({"geom": ["9aa0aac", "5044044", "5264264", "0222220", "5000044", "5222264", "3220226"]})
    elif num == 3:
        return jsonify({"geom": ["9aa0aac", "5044044", "5264044", "0224040", "5044044", "5265264", "3220226"]})
    elif num == 4:
        return jsonify({"geom": ["9aa0aac", "5000044", "5222264", "0220220", "5044044", "5264264", "3220226"]})
    elif num == 5:
        return jsonify({"geom": ["9aa0aac", "5044044", "5044264", "4040220", "5044044", "5264264", "3220226"]})
    elif num == 6:
        return jsonify({"geom": ["9aa0aac", "5064044", "5406044", "4440040", "5424044", "5264264", "3220226"]})
    elif num == 7:
        return jsonify({"geom": ["9aa2aac", "5022244", "5602454", "0264420", "5000044", "5220264", "3220226"]})
    elif num == 8:
        return jsonify({"geom": ["9aa0aac", "5044044", "5042C44", "4002440", "5040644", "5264264", "3220226"]})
    elif num == 9:
        return jsonify({"geom": ["9aa2aac", "5000044", "5220264", "0206020", "5422644", "5222264", "3220226"]})
    elif num == 10:
        return jsonify({"geom": ["988088C", "1020204", "1464604", "0222220", "5222264", "1000004", "3220226"]})
    elif num == 11:
        return jsonify({"geom": ["988088C", "1222224", "5222264", "0000000", "1020204", "1464604", "3220226"]})
    elif num == 12:
        return jsonify({"geom": ["98a088c", "1440024", "1440464", "0440020", "1440464", "1460004", "3220226"]})
    elif num == 13:
        return jsonify({"geom": ["9880a8c", "1204404", "5604404", "0204400", "5604404", "1004604", "3220226"]})
    elif num == 14:
        return jsonify({"geom": ["9aa0aac", "5020244", "5222264", "4020240", "5646464", "1222224", "3220226"]})
    elif num == 15:
        return jsonify({"geom": ["98a2aac", "5420244", "5464644", "4420200", "5064644", "5422264", "3220226"]})
    elif num == 16:
        return jsonify({"geom": ["9aa2aac", "1200024", "5464644", "4020240", "5464644", "5222264", "3220226"]})
    elif num == 17:
        return jsonify({"geom": ["9aa2a8c", "5020644", "5464244", "4020640", "5464244", "5220644", "3220226"]})
    elif num == 18:
        return jsonify({"geom": ["9a828ac", "5664664", "1600424", "4246060", "1620624", "5646444", "3220226"]})
    elif num == 19:
        return jsonify({"geom": ["9aa2aac", "5002044", "5024044", "4044040", "5200064", "1420604", "3220226"]})
    elif num == 20:
        return jsonify({"geom": ["98a2aac", "16888c4", "5002244", "4042600", "5200044", "1420264", "3220226"]})
    elif num == 21:
        return jsonify({"geom": ["98a2a8c", "1688c24", "5000044", "4044040", "5046044", "5220264", "3220226"]})
    elif num == 22:
        return jsonify({"geom": ["9aa2a8c", "5000424", "5020044", "4426000", "5000064", "5222604", "3220226"]})
    elif num == 23:
        return jsonify({"geom": ["9aa0aac", "5064244", "5602464", "0242020", "5420644", "5264264", "3220226"]})
    elif num == 24:
        return jsonify({"geom": ["9aa2aac", "5022244", "5422644", "4000040", "5020244", "5644464", "3220226"]})
    elif num == 25:
        return jsonify({"geom": ["9aa2aac", "5200244", "1604444", "4204440", "1604644", "5220264", "3220226"]})
    elif num == 26:
        return jsonify({"geom": ["9a828ac", "5464644", "5000044", "4022240", "5422644", "5220264", "3220226"]})
    elif num == 27:
        return jsonify({"geom": ["9aa2aac", "5020064", "5440424", "0440060", "5460424", "5222264", "3220226"]})
    elif num == 28:
        return jsonify({"geom": ["9aa2aac", "5020244", "5464644", "4020240", "5602464", "1264224", "3220226"]})
    elif num == 29:
        return jsonify({"geom": ["98a2aac", "5420244", "5244644", "0600240", "5064644", "5422264", "3220226"]})
    elif num == 30:
        return jsonify({"geom": ["9aa0aac", "1246024", "5420644", "4020240", "5464644", "5222264", "3220226"]})
    elif num == 31:
        return jsonify({"geom": ["9aa2a8c", "5020644", "5464244", "4020420", "5044044", "5222644", "3220226"]})
    elif num == 32:
        return jsonify({"geom": ["98a2aac", "1600244", "5024444", "4064640", "5202244", "1460464", "3220226"]})
    elif num == 33:
        return jsonify({"geom": ["98a2a8c", "1602424", "5246044", "0002240", "1642644", "5222264", "3220226"]})
    elif num == 34:
        return jsonify({"geom": ["9A80A8C", "5424424", "5020244", "4444640", "5460064", "5222604", "3220226"]})
    elif num == 35:
        return jsonify({"geom": ["98a2a8c", "1602424", "5046064", "4022000", "5426424", "5222264", "3220226"]})
    elif num == 36:
        return jsonify({"geom": ["9a808ac", "5602464", "1026004", "0202020", "5464644", "5646464", "3220226"]})
    elif num == 37:
        return jsonify({"geom": ["9aa08ac", "5260464", "1620204", "4264600", "1620024", "5260464", "3220226"]})
    elif num == 38:
        return jsonify({"geom": ["9a828ac", "5464644", "5646464", "0002000", "1246024", "5600464", "3220226"]})
    elif num == 39:
        return jsonify({"geom": ["9a80aac", "5604264", "1020624", "0464260", "1200624", "5604264", "3220226"]})
    elif num == 40:
        return jsonify({"geom": ["9a808ac", "5420644", "5264264", "0200020", "5422644", "5222264", "3220226"]})
    elif num == 41:
        return jsonify({"geom": ["9aa0aac", "5064064", "5404604", "4400200", "5424424", "5264264", "3220226"]})
    elif num == 42:
        return jsonify({"geom": ["9aa2aac", "5022244", "5600464", "0220220", "5064244", "5600464", "3220226"]})
    elif num == 43:
        return jsonify({"geom": ["9aa0aac", "5244244", "1460444", "0020440", "1640644", "5264264", "3220226"]})
    elif num == 44:
        return jsonify({"geom": ["9aa0aac", "5020244", "5464644", "4200060", "1600424", "5222264", "3220226"]})
    elif num == 45:
        return jsonify({"geom": ["9a82aac", "5460244", "5004640", "0000240", "5024644", "5642264", "3220226"]})
    elif num == 46:
        return jsonify({"geom": ["9aa2aac", "5200064", "1600424", "4020240", "5464644", "5220264", "3220226"]})
    elif num == 47:
        return jsonify({"geom": ["9aa28ac", "5024444", "5460044", "4020040", "5460244", "5226464", "3220226"]})
    elif num == 48:
        return jsonify({"geom": ["988088c", "1020204", "1464604", "0020200", "1464604", "1000004", "3220226"]})
    elif num == 49:
        return jsonify({"geom": ["9aa0aac", "5044044", "5266264", "0264220", "5044044", "5264264", "3220226"]})
    elif num == 50:
        return jsonify({"geom": ["9aa0aac", "5044044", "5264264", "0202020", "5666664", "1464604", "3220226"]})
    elif num == 51:
        return jsonify({"geom": ["98a0a8c", "1666624", "5646464", "0220220", "5044044", "5264264", "3220226"]})
    elif num == 52:
        return jsonify({"geom": ["98a0a8c", "1666624", "5646464", "0220220", "5044044", "5264264", "3220226"]})
    elif num == 53:
        return jsonify({"geom": ["9aa0a8c", "5044624", "5260664", "0224620", "5040664", "5264604", "3220226"]})
    elif num == 54:
        return jsonify({"geom": ["9aa0aac", "5064244", "5600464", "0222220", "5202064", "1464604", "3220226"]})
    elif num == 55:
        return jsonify({"geom": ["98a0aac", "1644244", "5240464", "0640020", "5240644", "1464264", "3220226"]})
    elif num == 56:
        return jsonify({"geom": ["98a0a8a", "1646424", "5222264", "0200020", "5620644", "5264264", "3220226"]})
    elif num == 57:
        return jsonify({"geom": ["9aa0a8c", "5064424", "5604064", "0204420", "5424064", "5264604", "3220226"]})
    
r = requests.put('http://fa22-cs340-adm.cs.illinois.edu:34000/addMG', json= { "name": "(:geometry dash:)", "url": "http://fa22-cs340-049.cs.illinois.edu:8000/", "author": "Jennifer Lim", "weight": 1 })
print(r.status_code)