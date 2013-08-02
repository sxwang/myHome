import sqlite3, time
from datetime import datetime
import matplotlib.pyplot as pl
import matplotlib.dates as mdates

DBFILE = '/Users/sxwang/Documents/myHome/weather.sql'
GRAPHFILE = '/Users/sxwang/Dropbox/weather.png'


def getndat():
    conn = sqlite3.connect(DBFILE)
    c = conn.cursor()
    sql = 'select * from weather order by rowid desc limit 288'
    c.execute(sql)
    data = c.fetchall()
    c.close()
    return(data)

def parsedata(data):
    date = []
    tempIn = []
    humIn = []
    tempOut = []
    humOut = []
    for row in data:
        # get temp and humidity data
        num = map(float, row[1:])
        tempIn.append(num[0])
        humIn.append(num[1])
        tempOut.append(num[2])
        humOut.append(num[3])
        # get and parse date data
        dtstr = row[0]
        dt = datetime.strptime(dtstr, "%Y-%m-%d_%H:%M:%S")
        date.append(dt)

    return date, tempIn, humIn, tempOut, humOut

def plotdata(date, tempIn, humIn, tempOut, humOut):
    fig = pl.figure(1)
    ax = fig.add_subplot(211)
    ax.plot(date, tempIn, ':.', date, tempOut, ':.')
    ax.set_ylabel('Temperature [C]')

    ax2 = fig.add_subplot(212)
    ax2.plot(date, humIn, ':.', date, humOut, ':.')
    ax2.legend(['Inside', 'Outside'], prop={'size':10},loc='lower left')
    ax2.set_xlabel('Time')
    ax2.set_ylabel('Humidity [%]')

    #pl.show()
    pl.gcf().autofmt_xdate()
    ax2.fmt_xdata = mdates.DateFormatter('%H:%M')
    pl.savefig(GRAPHFILE)

if __name__ == "__main__":
    data = getndat()
    date, tempIn, humIn, tempOut, humOut = parsedata(data) 
    plotdata(date, tempIn, humIn, tempOut, humOut)

