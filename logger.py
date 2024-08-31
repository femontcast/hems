import serial
import datetime
from matplotlib import pyplot as plt
from drawnow import drawnow

# Inicializa el serial
hems = serial.Serial('COM3',
                     baudrate=9600,
                     bytesize=8,
                     parity='N',
                     stopbits=1,
                     timeout=1.5
                     )

hems.close()
hems.open()
hems.flush()

time = []
temp1 = []
temp2 = []
pres1 = []
pres2 = []
humi1 = []
humi2 = []

# plt.ion()     # tell matplotlib you want interactive mode to plot data
# fig = plt.figure()

def in_figure() -> None:
    # Función para "imprimir" los últimos 100 datas de distancia
    ax1 = plt.subplot()
    plt.plot(time[-100:], temp1[-100:])
    ax1.set(xlabel='time (s)', ylabel='X',
            title='measurements')

while True:
    line = hems.readline()
    print(line)
    if (line[:4].decode("ascii") == str(datetime.date.today().year)):
        data = line[:-2].decode("ascii").split(sep=",")
        time.append(data[0])
        if data[1] == '1':
            temp1.append(data[2])
            pres1.append(data[3])
            humi1.append(data[4])
        
        if data[1] == '2':
            temp2.append(data[2])
            pres2.append(data[3])
            humi2.append(data[4])

    #drawnow(in_figure)


    