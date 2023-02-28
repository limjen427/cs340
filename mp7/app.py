from flask import Flask, render_template, request, jsonify
import os
import requests
from datetime import datetime, timedelta, time
# import re

app = Flask(__name__)

def convert24(time):
  # print('--------------convert24------------------')
  if time[-2:] == 'AM' and time[:2] == '12':
    return '00' + time[2:-3] + ':00'
  elif time[-2:] == 'AM':
    return time[:-3] + ':00'
  elif time[-2:] == 'PM' and time[:2] == '12':
    return time[:-3] + ':00'
  else:
    return str(int(time[:2]) + 12) + time[2:5] + ':00'
  
# Route for "/" (frontend):
@app.route('/')
def index():
  return render_template("index.html")

# Route for "/weather" (middleware):
@app.route('/weather', methods=["POST"])
def POST_weather():
  dow_dict = {'M':0, 'T':1, 'W':2, 'R':3, 'F':4}
  course = request.form["course"]

  number = course[-3:]
  subject = course[:2].upper()

  course_url = os.getenv('COURSES_MICROSERVICE_URL')
  course_r = requests.get(f'{course_url}/{subject}/{number}/')
  course_info = course_r.json()
  
  if 'error' in course_info:
    return 'course cannot be found', 400
  
  #course exist
  dsow = course_info['Days of Week']
  #char 별로 parse 후 dict에서 int list로 만들기
  days_of_week = []
  for dow in dsow:
    days_of_week.append(dow_dict[dow])


  st = course_info['Start Time']
  class_time = convert24(st)
  class_hour = class_time[0:2]

  #today days_of_week and start_time
  today_days_of_week = datetime.now().weekday()
  # M = 0, T = 1, W = 2, R = 3, F = 4, S = 5, S = 6
  today_datetime = datetime.now() #2022-10-23 10:58:37.039404
  today_hour = today_datetime.hour

  chosen_days_of_week = ''
  dsow2 = list(days_of_week)
  for i in range(1):
    for day in days_of_week:
      dsow2.append(day)
  #comepare when is the fastest days_of_week to have a class from today
  for day in dsow2:
    if len(dsow2) == 2:
      chosen_days_of_week = day
      break
    # same days of week
    if today_days_of_week == day:
      # class time not pass yet
      if today_hour < int(class_hour):
        chosen_days_of_week = day
        break
      # class time already passed move on
      elif today_hour > int(class_hour):
        chosen_days_of_week = dsow2[dsow2.index(day)+1]
        break
    # pass the day
    elif today_days_of_week > day:
      # aleady pass 'day'
      # if day == Friday
      if day == 4:
        chosen_days_of_week = dsow2[dsow2.index(day)+1]
        break
      # middle 
      elif today_days_of_week - day == 1:
        # day oen day before
        chosen_days_of_week = dsow2[dsow2.index(day)+1]
        break
      else:
        continue
    else:
      chosen_days_of_week = day
      break


  #timedelta
  print('today:',today_days_of_week, 'chosen:',chosen_days_of_week)
  add_days = chosen_days_of_week - today_days_of_week
  if add_days < 0:
    add_days += 7

  class_date = str(today_datetime + timedelta(days = add_days)).split()[0]

  weather_url = os.getenv('WEATHER_HOURLY_URL')
  weather_r = requests.get(weather_url)
  weather_data = weather_r.json().get('properties').get('periods')
  
  #find matching date
  forcastTime = ''
  temperature = ''
  shortForecast = ''

  for data in weather_data:
    w_date = data['startTime'][0:10]
    w_hour = data['startTime'][11:13]

    if class_date == w_date:
    #find matching hour
      if class_hour == w_hour:
      #bring 1. forcastTime 2. temperature 3. shortForecast 
        forcastTime = w_date + ' ' + w_hour + ':00:00'
        temperature = data['temperature']
        shortForecast = data['shortForecast']

        #return found the course and have weather data
        return jsonify(
          {
            "course": subject + ' ' + number,
            "nextCourseMeeting": class_date + ' ' + class_time,
            "forecastTime": forcastTime,
            "temperature": temperature,
            "shortForecast": shortForecast
          }
        ), 200

  #date is over 6 days + forcastTime = nextCourseMeeting - (any minutes)
  #return found the course but no data
  if (subject + ' ' + number):
    return jsonify(
      {
        "course": subject + ' ' + number,
        "nextCourseMeeting": class_date + ' ' + class_time,
        "forecastTime": forcastTime,
        "temperature": "forecast unavailable",
        "shortForecast": "forecast unavailable" 
      }
    ), 200
  
  return jsonify({"error": "Not implemented."}), 500


