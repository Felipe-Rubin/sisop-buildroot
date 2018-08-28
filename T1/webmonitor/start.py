#!/usr/bin/env python2.6
# -*- coding: utf-8 -*
#
#Authors: Felipe Pfeifer Rubin & Guilherme Girotto
#Contact: {felipe.rubin,guilherme.girotto}@acad.pucrs.br
#
#Description: This file starts the webmonitor application
#
#

#Everything here is installed when you install flask
from flask import Flask, jsonify, render_template, request, send_from_directory

#This two are common python libraries
import os
import subprocess
import json


#Create the flask app
app = Flask(__name__)




#NOTE:
#If any javascript/stylesheet changes, 
#it may be necessary to delete cookies of this webpage to reload them


#Executes a script and returns the output
def execscript(filename):
    scripts_dir = os.path.dirname(os.path.abspath(__file__))+"/scripts/"
    p = subprocess.Popen(['sh',scripts_dir+filename],stdout=subprocess.PIPE,stderr=subprocess.PIPE,universal_newlines=True)
    out,err = p.communicate()
    return out

#ROUTES

#Accessing any javascript script
@app.route('/js/<path:filename>')
def serve_static(filename):
    root_dir = os.path.dirname(os.path.abspath(__file__))
    return send_from_directory(os.path.join(root_dir, 'static', 'js'), filename)

# Acessing any stylesheet
@app.route('/styles/<path:filename>')
def serve_styles(filename):
    root_dir = os.path.dirname(os.path.abspath(__file__))
    return send_from_directory(os.path.join(root_dir, 'static', 'styles'), filename)


#Routes our index, which by default (flask's default) is located at templates/
#The Index page
@app.route('/')
def index():
    return render_template('index.html')


#Date
@app.route('/components/date')
def get_date():
    return render_template('/components/date.html',date=execscript('getdate.sh'))

#Time
@app.route('/components/time')
def get_time():
    return render_template('/components/time.html',time=execscript('gettime.sh'))

#Uptime
@app.route('/components/uptime')
def get_uptime():
    return render_template('/components/uptime.html',uptime=execscript('getuptime.sh'))

#Processor information
@app.route('/components/procinfo')
def get_procinfo():
    full_list = execscript('getprocinfo.sh').split('\n')
    full_list.pop()
    processor_list = []
    for processor in full_list:
        processor_list.append(processor.split('\t'))
    return render_template('/components/procinfo.html',procinfo=processor_list)

#Processor Load %
@app.route('/components/procload')
def get_procload():
    pload = execscript('getprocload.sh').split(' ')
    return render_template('/components/procload.html',load1min=pload[0],load5min=pload[1],load15min=pload[2])

#RAM Total and RAM in use (MB)
@app.route('/components/ramusage')
def get_ramusage():
    rusage = execscript('getramusage.sh').split(',')
    return render_template('/components/ramusage.html',ramtotal=rusage[0],ramused=rusage[1])

#System Version
@app.route('/components/sysversion')
def get_sysversion():
    return render_template('/components/sysversion.html',sysversion=execscript('getsysversion.sh'))

#Process List PID,Name
@app.route('/components/procslist')
def get_process_list():
    full_list = execscript('getprocslist.sh').split('\n')
    full_list.pop()
    process_list = []
    for proc in full_list:
        process_list.append(proc.split(' '))    
    return render_template('/components/procslist.html',proc_list=process_list)

app.run(host='0.0.0.0',port=5000)