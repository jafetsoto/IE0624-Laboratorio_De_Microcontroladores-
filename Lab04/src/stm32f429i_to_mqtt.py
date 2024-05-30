import serial
import paho.mqtt.client as mqtt
import json
import time

# Serial COMM
SERIAL_COMM = serial.Serial("/dev/ttyACM0", 115200, timeout=1)
print("Conectado al puerto serial /dev/ttyACM0")

# =======================================================
# Callback function for MQTT client on connect
def on_connect(client, userdata, flags, ERROR):
    if ERROR == 0:
        print("==================================")
        print("----- ¡Conexión Broker-MQTT! -----")
    else:
        print("---- ...Error de conexión:... ----")
        print("Error:", ERROR)
        print("==================================")

# Callback function for MQTT client on disconnect
def on_disconnect(client, userdata, ERROR):
    if(ERROR == 0):
        print("---- Fin de la comunicación. -----")
        print("==================================")
    else:
        print("Sistema desconectado:")
        print("Salida:", ERROR)
        print("==================================")

# Callback function for MQTT client on publish
def on_publish(client, userdata, mid):
    print("========== Dashboard On ==========\n", mid)
# =======================================================
# MQTT client
#client = mqtt.Client("stm32f429i")
client = mqtt.Client(callback_api_version=mqtt.CallbackAPIVersion.VERSION1)
client.connected = False
client.on_connect = on_connect
client.on_disconnect = on_disconnect
client.on_publish = on_publish


# MQTT Broker details
broker = "iot.eie.ucr.ac.cr"
port = 1883
topic = "v1/devices/me/attributes/request/1"
username = "Monitor_Pendientes_B77543"
password = "ae79c8xvqqc1z3x1yv2c"

# Connect to MQTT Broker
client.username_pw_set(password)
client.connect(broker, port)

dictionary = dict()

while client.connected != True:
    client.loop()
    time.sleep(2)
    
    while client.connected != True:
        client.loop()
        time.sleep(2)

        while True:
            Serial_DATA = SERIAL_COMM.readline().decode('utf-8')

            Serial_DATA = Serial_DATA.replace('\r', "").replace('\n', "")
            Serial_DATA = Serial_DATA.split(',')

            if len(Serial_DATA) == 4:
                dictionary["Eje X"] = Serial_DATA[0]
                dictionary["Eje Y"] = Serial_DATA[1]
                dictionary["Eje Z"] = Serial_DATA[2]

                if Serial_DATA[3]:  # Check if data exists in Serial_DATA[3]
                    dictionary["Bateria"] = Serial_DATA[3]
                    try:
                        if float(Serial_DATA[3]) < 6.5:
                            dictionary["Bateria"] = "LOW"
                        else:
                            dictionary["Bateria"] = "OK"
                    except ValueError:
                        print("Invalid value for Bateria:", Serial_DATA[3])
                        continue  # Skip processing this data
                else:
                    print("No data for Bateria")
                    continue  # Skip processing this data

                payload = json.dumps(dictionary)

                topic = "v1/devices/me/telemetry"
                print(payload)
                client.publish(topic, payload)

                time.sleep(1)
