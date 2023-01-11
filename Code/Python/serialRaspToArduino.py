import serial
import time
import datetime as dt
import mysql.connector
from math import floor

#Cancellare le letture da 10 giorni prima

#OPTIONS
SensorPeriod = 300
LightPeriod = 1
devicePeriod = 1
refillPeriod = 120
lectureDayInterval = 10


minWaterlevel = 2         #acqua min level
writeOnDB = True


arduino = serial.Serial(port='/dev/ttyACM0', baudrate=115200, timeout=.1)

#Refill boolean control
isUnderLevel = [False, False, False, False]
isRefilling = [False, False, False, False]


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
                if writeOnDB:
                    mycursor.execute("SELECT * FROM `aquarium` WHERE pos = " + str(count))
                    myresult = mycursor.fetchall()[0]
                    sql = "INSERT INTO `Lecture`(`ID`, `aquaID`, `sensorID`, `data`, `value`) VALUES (%s, %s, %s, %s, %s)"
                    val = ('0', myresult[0], i, dt.datetime.now() , v)
                    mycursor.execute(sql, val)
                if i == 5 and float(v) < minWaterlevel:          #Water level sensor  e under the level
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
    mycursor.execute("SELECT * FROM `aquarium`")
    myresult = mycursor.fetchall()

    print("Setting value....\n")
    for v in myresult:
        mytime = v[3]
        duration = v[4]
        perc = v[5]
        pos = v[7]
        onOffAut = v[8]

        startTime = dt.time(floor(mytime.seconds / 3600), floor(mytime.seconds / 60) % 60, mytime.seconds % 60)
        nowtime = dt.datetime.now().time()
        endTime = dt.datetime.now() + dt.timedelta(hours = duration)

        if onOffAut == 0:
            print("Aquarium " + str(pos) + " led off manually")
            writeToArduino("40|" + str(pos) + "|0")
            print(readFromArduino())
        elif onOffAut == 1:
            print("Aquarium " + str(pos) + " led on manually")
            writeToArduino("40|" + str(pos) + "|" + str(perc))
            print(readFromArduino())
        else:
            if isNowInTimePeriod(startTime, endTime.time(), nowtime):
                writeToArduino("40|" + str(pos) + "|" + str(perc))
            else:
                writeToArduino("40|" + str(pos) + "|0")
            print(readFromArduino())
    mydb.commit()


def automaticRefillRoutine():
    print("-----------------------------------------------------\n\n                 REFILL ROUTINE\n\n-----------------------------------------------------\n")
    print("Reading database....\n")
    mycursor.execute("SELECT * FROM `aquarium`")
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
                    mycursor.execute("UPDATE `Device` SET `status` = 'on' WHERE `name` = 'Pump'")
                    myresult = mycursor.fetchall()
                    mydb.commit()
                    isRefilling[row[7] - 1] = True
            else:
                print("Acquarium " + str(row[7]) + " ok")
                if isRefilling[row[7] - 1] == True:
                    #disattivo il refill
                    print("I stop to refill....")
                    print("Stopping refill pump....")
                    writeToArduino("30|off")
                    print(readFromArduino())
                    mycursor.execute("UPDATE `Device` SET `status` = 'off' WHERE `name` = 'Pump'")
                    myresult = mycursor.fetchall()
                    mydb.commit()
                    isRefilling[row[7] - 1] = False

def deleteDataFromDatabase(day):
    print("-----------------------------------------------------\n\n              DELETE DATA ROUTINE\n\n-----------------------------------------------------\n")
    print("Deleting last "+ str(day) + "from lecture in database....\n")
    sql = "DELETE FROM `Lecture` WHERE DATEDIFF(NOW(), `data`) > %s"
    val = (day,)
    mycursor.execute(sql, val)
    mydb.commit()
    print(mycursor.rowcount, "record(s) deleted")


startSensorTime = time.time()
startDeviceTime = time.time()
startLedTime = time.time()
startRefillTime = time.time()
lastDeleteData = dt.datetime.now()

while True:
    now = time.time()
    if now - startSensorTime > SensorPeriod:
        getSensorData()
        startSensorTime = time.time()
    if now - startDeviceTime > devicePeriod:
        refreshDeviceState()
        startDeviceTime = time.time()
    if now - startLedTime > LightPeriod:
        ledRoutine()
        startLedTime = time.time()
    if now - startRefillTime > refillPeriod:
        automaticRefillRoutine()
        startRefillTime = time.time()
    if dt.datetime.now() - dt.timedelta(days=1) > lastDeleteData:
        deleteDataFromDatabase(lectureDayInterval)
        lastDeleteData = dt.datetime.now()
    time.sleep(0.5)

arduino.close()