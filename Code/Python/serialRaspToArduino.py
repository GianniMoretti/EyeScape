import serial
import time

import mysql.connector

arduino = serial.Serial(port='COM4', baudrate=115200, timeout=.1)

mydb = mysql.connector.connect(
  host="localhost",
  user="gianni",
  password="Viva.le.2005",
  database="AcquariumDB"
)

mycursor = mydb.cursor()

def writeToArduino(msg):
    arduino.write(bytes(msg, 'utf-8'))
    time.sleep(0.05)

def readFromArduino():   
    data = arduino.readline()
    return data

while True:
    mycursor.execute("SELECT * FROM Device")
    myresult = mycursor.fetchall()
    
    for x in myresult:
        print(x)

    """ num = input("Enter a number: ")
    writeToArduino(num)
    data = readFromArduino()
    print(data) """







