import serial
import time
import datetime as dt
import mysql.connector
from math import floor

#TODO:
#Aggiungere on/off della lampada
#finire i collegamenti
#cambiare i sensori
#creare la valvola
#Aggiungere opzione per scrivere o no nel database i sensori
#Update della pompa di refill nel database

arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)
period = 3 #seconds

#Refill boolean control
isUnderLevel = [False, False, False, False]
isRefilling = [False, False, False, False]
level = 2

mydb = mysql.connector.connect(
    host="localhost",
    user="Gianni",
    password="Viva.le.2005",
    database="Aquarium"
)

mycursor = mydb.cursor()

def isNowInTimePeriod(startTime, endTime, nowTime): 
    if startTime < endTime: 
        return nowTime >= startTime and nowTime <= endTime 
    else: 
        #Over midnight: 
        return nowTime >= startTime or nowTime <= endTime

def writeToArduino(msg):
    arduino.write(msg.encode("ascii"))
    time.sleep(1)

def readFromArduino():   
    data = " "
    msg = ""
    while(data != ""):
        data = arduino.readline().decode("ascii")
        msg = msg + data

    return msg

def refreshDeviceState():
    print("-----------------------------------------------------\n\n                 DEVICE ROUTINE\n\n-----------------------------------------------------\n")
    print("Reading database....")
    mycursor.execute("SELECT status FROM Device")
    myresult = mycursor.fetchall()
    mydb.commit()
    v = []
    for x in myresult:
        v.append(x[0])
    print("Sending data.....")
    str = "20|" + v[0] + "|" + v[1] + "|" + v[2]
    writeToArduino(str)
    data = readFromArduino()
    print(data + "\n\n")
    print("DEVICE        STATE\n\nFilters       " + v[0] + "\nResistors     " + v[1] + "\nBlowers       " + v[2] + "\nPump          " + v[3] +" \n\n")

def getSensorData():
    print("-----------------------------------------------------\n\n                 SENSOR ROUTINE\n\n-----------------------------------------------------\n")
    print("Sending request....\n")
    writeToArduino("10")
    print("SENSOR DATA\n")
    data = readFromArduino()
    if data == '':
        return
    data = data[0:-4]
    tmp = data.split('|')
    for i in range(1, 6):
        d = tmp[i].split('=')
        stri = d[0] + "       "
        vet = d[1].split(',')
        count = 1
        for v in vet:
            if v != '':
                if v == "err":
                    v = -1
                
                stri = stri + str(v) + "       "   
                mycursor.execute("SELECT * FROM `Aquarium` WHERE pos = " + str(count))
                myresult = mycursor.fetchall()[0]
                sql = "INSERT INTO `Lecture`(`ID`, `aquaID`, `sensorID`, `data`, `value`) VALUES (%s, %s, %s, %s, %s)"
                val = ('0', myresult[0], i, dt.datetime.now() , v)
                mycursor.execute(sql, val)
                
                if i == 5 and float(v) < level:          #Water level sensor  e under the level
                    isUnderLevel[count - 1] = True
                else:
                    isUnderLevel[count - 1] = False
                
                count = count + 1        
        print(stri + "\n")

    print("Saving data....")
    mydb.commit()
    print("Data saved.")



def ledRoutine():
    print("-----------------------------------------------------\n\n                 LED ROUTINE\n\n-----------------------------------------------------\n")
    print("Reading database....\n")
    mycursor.execute("SELECT * FROM `Aquarium`")
    myresult = mycursor.fetchall()

    print("Setting value....\n")
    for v in myresult:
        mytime = v[3]
        duration = v[4]
        perc = v[5]
        pos = v[7]

        startTime = dt.time(floor(mytime.seconds / 3600), floor(mytime.seconds / 60) % 60, mytime.seconds % 60)
        nowtime = dt.datetime.now().time()
        endTime = dt.datetime.now() + dt.timedelta(hours = duration)

        if isNowInTimePeriod(startTime, endTime.time(), nowtime):
            writeToArduino("40|" + str(pos) + "|" + str(perc))
        else:
            writeToArduino("40|" + str(pos) + "|0")
        print(readFromArduino())
    
    mydb.commit()


def automaticRefillRoutine():
    print("-----------------------------------------------------\n\n                 REFILL ROUTINE\n\n-----------------------------------------------------\n")
    print("Reading database....\n")
    mycursor.execute("SELECT * FROM `Aquarium`")
    myresult = mycursor.fetchall()

    for row in myresult:
        if row[6] != 0:                   #Automatic refill on
            if isUnderLevel[row[7] - 1]:
                print("Acquarium " + str(row[7]) + " under the level value!")
                #Controllo che nessun altro stia riempendo
                if not (isRefilling[0] or isRefilling[1] or isRefilling[2] or isRefilling[3]):
                    print("I start to refill....")
                    #attivo il refill in pos row[7] - 1
                    print("Set valve in the correct way....")
                    writeToArduino("50|" + str(row[7]))
                    print(readFromArduino())
                    print("Starting refill pump....")
                    writeToArduino("30|on")
                    print(readFromArduino())
                    isRefilling[row[7] - 1] = True
            else:
                print("Acquarium " + str(row[7]) + " ok")
                if isRefilling[row[7] - 1] == True:
                    #disattivo il refill
                    print("I stop to refill....")
                    print("Stopping refill pump....")
                    writeToArduino("30|off")
                    print(readFromArduino())
                    isRefilling[row[7] - 1] = False



while True:
    getSensorData()
    refreshDeviceState()
    ledRoutine()
    automaticRefillRoutine()
    time.sleep(period)

arduino.close()






