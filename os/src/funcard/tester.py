import pysimulavr
from ex_utils import SimulavrAdapter
from smartcard.util import toHexString


def wait_for_toggle():
    old_val = io_val()
    while (io_val() is old_val):
        sim.doStep()

def wait_for_high():
    while (io_val() is not "H"):
        sim.doStep()

def wait_for_low():
    while (io_val() is not "L"):
        sim.doStep()

def wait_for_start():
    wait_for_low()
    sim.doRun(sim.getCurrentTime() + etu*0.5)

    if io_val() is not "L":
        wait_for_start()
    else:
        if DEBUG: print "START BIT"

def readbyte():
    wait_for_start()
    data = 0
    parity = 0
    for x in range(0,8):
        sim.doRun(sim.getCurrentTime() + etu)
        if io_val() is "H":
            parity += 1
            data = data | 1 << x
    sim.doRun(sim.getCurrentTime() + etu)
    if (((parity % 2) > 0) and (io_val() is not "H")) or \
       (((parity % 2) == 0) and (io_val() is not "L")):
        print "Parity is not correct"
        sim.doRun(sim.getCurrentTime() + 2*etu)
        return
    sim.doRun(sim.getCurrentTime() + 2*etu)
    if DEBUG: print "DATA READ :", hex(data)
    return data

def send_start():
    io_pin.SetPin('L')
    sim.doRun(sim.getCurrentTime() + etu)

def writebyte(data):
    if DEBUG: print "DATA WRITE :", hex(data)
    send_start()
    parity = 0
    for x in range(0,8):
        bit = (data >> x) & 0x01
        if bit > 0:
            io_pin.SetPin("H")
            parity += 1
        else:
            io_pin.SetPin("L")
        sim.doRun(sim.getCurrentTime() + etu)
    if (parity % 2) > 0:
        io_pin.SetPin("H")
    else:
        io_pin.SetPin("L")
    sim.doRun(sim.getCurrentTime() + etu)
    io_pin.SetPin("H")
    sim.doRun(sim.getCurrentTime() + etu)
    io_pin.SetPin("t")
    sim.doRun(sim.getCurrentTime() + etu)


def getATR():
    atr = []
    for x in range(0,15):
        atr.append(readbyte())
    print "ATR = ", atr

def testWrite(data, offset):
    writebyte(0x80)
    writebyte(0x02)
    writebyte(0x00)
    writebyte(offset)
    writebyte(len(data))
    for x in data:
        readbyte()
        writebyte(x)
    readbyte()
    readbyte()

def testRead(length, offset):
    writebyte(0x80)
    writebyte(0x04)
    writebyte(0x00)
    writebyte(offset)
    writebyte(length)
    readbyte()
    data = []
    for x in range(0,length):
        data.append(readbyte())
    readbyte()
    readbyte()
    return data

def testGetCurrent():
    writebyte(0x80)
    writebyte(0x22)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(0x02)
    readbyte()
    data = []
    for x in range(0,2):
        data.append(readbyte())
    readbyte()
    readbyte()
    return data

def testGetSecurity():
    writebyte(0x80)
    writebyte(0x24)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(0x01)
    readbyte()
    security = readbyte()
    readbyte()
    readbyte()
    return security

def testGetChallenge():
    writebyte(0x80)
    writebyte(0x28)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(0x08)
    readbyte()
    data = []
    for x in range(0,8):
        data.append(readbyte())
    readbyte()
    readbyte()
    return data

def Select(fid):
    writebyte(0x80)
    writebyte(0xA4)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(0x02)
    if(readbyte()==0xA4):
        writebyte((fid >> 8) & 0xff)
        if(readbyte()==0xA4):
            writebyte(fid & 0xff)
            readbyte()
            readbyte()

def ReadBinary(length):
    writebyte(0x80)
    writebyte(0xB0)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(length)
    readbyte()
    readbyte()
    data = []
    for x in range (0,length):
        data.append(readbyte())
    readbyte()
    readbyte()
    return data

def UpdateBinary(data):
    writebyte(0x80)
    writebyte(0xD6)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(len(data))
    readbyte()
    readbyte()
    for x in data:
        readbyte()
        writebyte(x)
    readbyte()
    readbyte()

def CreateEF(fid,size,acread,acupdate):
    writebyte(0x80)
    writebyte(0xE0)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(0x07)
    if(readbyte() == 0xE0):
        writebyte(0x5f)
    if(readbyte() == 0xE0):
        writebyte((fid >> 8) & 0xff)
    if(readbyte() == 0xE0):
        writebyte(fid & 0xff)
    if(readbyte() == 0xE0):
        writebyte((size >> 8) & 0xff)
    if(readbyte() == 0xE0):
        writebyte(size & 0xff)
    if(readbyte() == 0xE0):
        writebyte(acread & 0xff)
    if(readbyte() == 0xE0):
        writebyte(acupdate & 0xff)
    readbyte()
    readbyte()

def CreateDF(fid):
    writebyte(0x80)
    writebyte(0xE0)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(0x05)
    if(readbyte() == 0xE0):
        writebyte(0x4f)
    if(readbyte() == 0xE0):
        writebyte((fid >> 8) & 0xff)
    if(readbyte() == 0xE0):
        writebyte(fid & 0xff)
    readbyte()
    readbyte()

def DeleteFile(fid):
    writebyte(0x80)
    writebyte(0xE4)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(0x05)
    if(readbyte() == 0xE4):
        writebyte((fid >> 8) & 0xff)
    if(readbyte() == 0xE4):
        writebyte(fid & 0xff)
    readbyte()
    readbyte()


def Verify(pin):
    writebyte(0x80)
    writebyte(0x20)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(0x04)
    for x in pin:
        if(readbyte() == 0xdf):
            writebyte(x)
    readbyte()
    readbyte()

def GetChallenge():
    writebyte(0x80)
    writebyte(0x84)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(0x08)
    if(readbyte() == 0x84):
        challenge = []
        for x in range(0,8):
            challenge.append(readbyte())
        readbyte()
        readbyte()
        return challenge
    else:
        readbyte()

def testEncrypt(data):
    writebyte(0x80)
    writebyte(0x26)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(0x08)
    for x in data:
        if(readbyte() == 0xd9):
            writebyte(x)
    readbyte()
    readbyte()

def Ext_Auth(enc):
    writebyte(0x80)
    writebyte(0x82)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(0x08)
    for x in enc:
        if(readbyte() == 0x7d):
            writebyte(x)
    readbyte()
    readbyte()

def GetResponse(length):
    writebyte(0x80)
    writebyte(0xC0)
    writebyte(0x00)
    writebyte(0x00)
    writebyte(length)
    if(readbyte() == 0xC0):
        response = []
        for x in range(0,length):
            response.append(readbyte())
        readbyte()
        readbyte()
        return response
    else:
        readbyte()


DEBUG = True

baudrate = 9600
etu = 1000000000/baudrate
print "ETU =", etu, "ns"

freq = 3579000
period = 1000000000/freq
print "1/Freq =", period, "ns"

sim = SimulavrAdapter()
sim.dmanSingleDeviceApplication()

dev = sim.loadDevice('at90s8515','pintaros')
dev.SetClockFreq(period)

sim.setVCDDump("atr.vcd", ["PORTB.B6-Out","PORTB.DDR","PORTB.PIN","PORTB.PORT"])

io_pin = dev.GetPin("B6")
io_val = io_pin.toChar

sim.dmanStart()

getATR()

# readbyte()
# readbyte()
# readbyte()
# readbyte()
# readbyte()
# readbyte()
# readbyte()
# readbyte()
# readbyte()
# readbyte()

sim.doRun(sim.getCurrentTime() + etu)

# testWrite([0x1,0x2,0x3,0x4,0x5],0x80)
# testRead(5,0x80)

#Select(0x1234)

sim.dmanStop()
