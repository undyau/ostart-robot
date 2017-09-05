#!/usr/bin/env python

import ZeroSeg.led as led
import time
import datetime
import RPi.GPIO as GPIO
import os
import commands

switch1 = 17
switch2 = 26

use_24_hour = True
offset = 0
global showing_clock
showing_clock = True

def show_text(device, deviceId, text):

    global showing_clock
    showing_clock = False
    device.write_text(deviceId, text);
    time.sleep(0.5)            

def clock(device, deviceId, seconds):

    global showing_clock
    showing_clock = True
   # for _ in xrange(seconds):
    now = datetime.datetime.now()
    now = now + datetime.timedelta(minutes = offset)
    hour = now.hour
    if not use_24_hour and hour > 12:
        hour -= 12
    minute = now.minute
    second = now.second
    # Set hours
    device.letter(deviceId, 8, int(hour / 10))     # Tens
    device.letter(deviceId, 7, hour % 10)    # Ones
    device.letter(deviceId, 6, " ")      
    # Set minutes
    device.letter(deviceId, 5, int(minute / 10))   # Tens
    device.letter(deviceId, 4, minute % 10)  # Ones
    device.letter(deviceId, 3, " ")           
    # Set seconds        
    device.letter(deviceId, 2, int(second / 10))   # Tens
    device.letter(deviceId, 1, second % 10)        # Ones        
    time.sleep(0.1)

GPIO.setmode(GPIO.BCM) # Use BCM GPIO numbers

GPIO.setup(switch1, GPIO.IN)
GPIO.setup(switch2, GPIO.IN)

device = led.sevensegment()
device.brightness(12)  # Could control via menu ?
state = 0 # 0 = clock, 1 = Add 1, 2 = Sub 1, 3 = 12 hour/24 hour, 4 = ip addr,  5 = Off
prev_state = 0


while True:
    if not GPIO.input(switch1):
        prev_state = state          
        if not showing_clock:
            state += 1
            state %= 6
        if state == 0 and (prev_state != 0 or showing_clock):
            show_text(device, 0, "cloch")
        elif state == 1 and (prev_state != 1 or showing_clock):
            show_text(device, 0, "Add1  " + str(offset))
        elif state == 2 and (prev_state != 2 or showing_clock):
            show_text(device, 0, "Sub1  " + str(offset))  
        elif state == 3 and (prev_state != 3 or showing_clock):
            if use_24_hour:
                show_text(device, 0, "12 hour")
            else:
                show_text(device, 0, "24 hour")        
        elif state == 4 and (prev_state != 4 or showing_clock):
            status = commands.getstatusoutput('hostname -I')
            addr = "-no 1p-" 
            if status[0] == 0 and "." in status[1]:
                addr = status[1].replace(".", " ")
                addr = addr[-8:]
            show_text(device, 0, addr)   
        elif state == 5 and (prev_state != 5 or showing_clock):
                show_text(device, 0, "- Off -") 
    elif not GPIO.input(switch2):
        if state == 0:
            clock(device, 0, seconds=10)
            prev_state = 0
        elif state == 1:
            offset += 1
            clock(device, 0, seconds=10)
        elif state == 2:
            offset -= 1
            clock(device, 0, seconds=10)
        elif state == 3:
            use_24_hour = not use_24_hour
            clock(device, 0, seconds=10) 
        elif state == 4:
            clock(device, 0, seconds=10)
        elif state == 5:
            show_text(device, 0, "stopping") 
            os.system('shutdown now -h')            
    else:
        if showing_clock:
            clock(device, 0, seconds=10)
