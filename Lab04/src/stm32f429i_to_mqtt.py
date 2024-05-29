#!/usr/bin/python3


import serial
import paho.mqtt.client as mqtt
import json
import ssl, socket

line = []
sata=dict()
broker="iot.eie.ucr.ac.cr"
port=1883
topic="v1/devices/me/attributes/request/1"
username="Monitor_Pendientes_B77543"
password="ae79c8vvqqc1z3x1yv2c"

