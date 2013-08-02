import sqlite3
import serial
import time
from datetime import datetime
from subprocess import Popen
SERIAL_PORT = 'fd111'
LOGFILE = 'weather_log.txt'
DBFILE = '/Users/sxwang/Documents/myHome/weather.sql'

def setupDB():
    conn = sqlite3.connect(DBFILE)
    c = conn.cursor()
    c.execute('''create table weather (time text, temp1 float, hum1 float, 
        temp2 float, hum2 float)''')
    conn.commit()
    conn.close()

def getTimeStamp():
    # first get the current time stamp
    curtime = datetime.now()
    timestamp = curtime.strftime("%Y-%m-%d_%H:%M:%S")
    return timestamp

def readArduino():
    ser = serial.Serial(port='/dev/tty.usbmodem' + SERIAL_PORT, baudrate=9600)
    #print "arduino opened"
    attempts = 0
    while(True):
        try:
            line = ser.readline()
            attempts += 1
            print line
            if len(line.split(',\t')) == 5:
                break
        except Exception, e:
            pass
        if attempts >= 20:
            raise Exception("Arduino read error. Is it connected?")
    #print "saving last line to DB"

    [status, temp1, hum1, temp2, hum2] = line.split(',')
    temp1 = float(temp1)
    hum1 = float(hum1)
    temp2 = float(temp2)
    hum2 = float(hum2)

    return [temp1, hum1, temp2, hum2]

def insertintoDB(timestamp, data):
    conn = sqlite3.connect(DBFILE)
    c = conn.cursor()
    # this is the actual data
    [temp1, hum1, temp2, hum2] = data
    c.execute("insert into weather values (?,?,?,?,?)",
        (timestamp, temp1, hum1, temp2, hum2))
    conn.commit()
    conn.close()

def writeToLog(timestamp, text):
    f = open(LOGFILE, 'a')
    f.write(timestamp+' --- '+text+'\n')
    f.close()

if __name__ == "__main__":
    timestamp = getTimeStamp()
    try:
        data = readArduino()
        insertintoDB(timestamp, data)
    except Exception, e:
        print e
        writeToLog(timestamp, str(e))


