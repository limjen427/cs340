from flask import Flask, jsonify
import os
import requests
from datetime import datetime, timedelta, time

@app.route('/weather', methods=["GET"])
def POST_weather():
    weather_r = requests.get("https://api.weather.gov/gridpoints/ILX/95,71/forecast/hourly")
    weather_data = weather_r.json().get('properties').get('periods')

    weather = []

    for i in range(3):
        data = weather_data[i]
        startTime = data['startTime'][0:10] + ' ' + data['startTime'][11:13] + ':00:00'
        endTime = data['endTime'][0:10] + ' ' + data['endTime'][11:13] + ':00:00'
        temperature = data['temperature']
        shortForecast = data['shortForecast']

        weather.append({
            "forecastTime": startTime + " - " + endTime,
            "temperature": temperature,
            "shortForecast": shortForecast
        })

    return weather, 200