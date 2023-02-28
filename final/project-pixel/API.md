# API.md

1. A high-level overview of your implementation of the pixel board design (including middleware and PGs)
    ##### When you open the server, you will see white board with black outline. This will be your pixel board. I made one pixel generator and when you open the server and find pixel board, you are able to see one pixel in random points which will get random color in every 3 seconds. 

2. Technical details on the APIs between your middleware and the PGs
    ##### In `/pixelgenerate` route in PGs directory, it will keep requests.put `/pixel` route. In `/pixel` from project-pixel directory, we will requests.get the url which is calling `/pixelgenerate`

3. Technical details on any changes to the API between the frontend and the middleware
    ##### I set size of the pixel board as width = 400 and hight = 200. I also set 8 different colors in palette key in `/setting` route from the middleware. I requests.get url from `/pixelgenerate` route from PGs directory in every 3 seconds.

4. Technical details on how PGs are added and removed from the middleware
    ##### I requests.put `/pixel` route in file which has `/pixelgenerate` route in there. And in `/pixel` route function, I requests.get `/pixelgenerate` url. This is when we call `/pixelgenerate` in main server. 

5. Technical details on any dependencies needed to run your pixel board (ex: python commands to run, other services, docker, etc)
    ##### First, run `python3 -m flask run` in project-pixel directory. Then, open another terminal, go to PGs directory in project-pixel by doing `cd PGs` in terminal assume your new terminal is already in project-pixel directory. Run `python3 -m flask run -p 8000`. Then comeback to the terminal which is in project-pixel directory. Use url that shows in that terminal which said, `http://127.0.0.1:5000`. You will see pixel board and a pixel that is randomly show in pixel board with random points and random color. 

6. Any additional details of the ability for your design to incorporate the Week #2 challenges (see Week #2 preview below)
    ##### First we could limit rate of how often pixel will be updated by giving time limit in `/pixel` route. We could connect any database platform and keep add change version of pixel board in database. By making new file in every change, we are able to access history of pixel board. I think giving as many color as possible in palette is the best idea. I will keep thinking more about additional details for Week #2 but this is what I could think of. 