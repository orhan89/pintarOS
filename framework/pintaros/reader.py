from smartcard.CardType import AnyCardType
from smartcard.CardRequest import CardRequest
from smartcard.util import toHexString

from smartcard.sw.ErrorCheckingChain import ErrorCheckingChain
from smartcard.sw.ISO7816_4ErrorChecker import ISO7816_4ErrorChecker
from smartcard.sw.SWExceptions import SWException, WarningProcessingException

import tea

DEBUG = 0

CLA = 0x80
errorchain = []
errorchain = [ ErrorCheckingChain( errorchain, ISO7816_4ErrorChecker()) ]
cardservice = None

selected_file = None

def run():
    global cardservice
    try:
        cardtype = AnyCardType()
        cardreq = CardRequest(timeout=30,cardType=cardtype)
        cardservice = cardreq.waitforcard()
        cardservice.connection.setErrorCheckingChain(errorchain)
        cardservice.connection.connect()
        if DEBUG : print "ATR = ", toHexString(cardservice.connection.getATR())
    except SWException, e:
        print str(e)
        return False

        
def create_file(size, fid, filetype, aclist):
    global cardservice
    try:
        header = [CLA, 0xe0, 0x00, 0x00, 0x06]
        size_h = size/256
        size_l = size%256
        fid_h = fid/256
        fid_l = fid%256
        data = [size_h, size_l, fid_h, fid_l, filetype, aclist]
        response, sw1, sw2 = cardservice.connection.transmit(header+data)

        if(sw1==0x90 and sw2==0x00):
            if DEBUG : print "File Created Successfully"
            return True
    except SWException, e:
        print str(e)
        return False

        
def select_file(fid):
    global cardservice,selected_file

    if(fid != selected_file):
        try:
            header = [CLA, 0xa4, 0x00, 0x00, 0x02]
            fid_h = fid/256
            fid_l = fid%256
            data = [fid_h, fid_l]
            response, sw1, sw2 = cardservice.connection.transmit(header+data)

            if((sw1==0x61) and (sw2==0x06)):
                if DEBUG : print "File Selected"
                response = get_response(sw2)
                return response
        except SWException, e:
            print str(e)
            return False

        
def delete_file(fid):
    global cardservice
    try:
        header = [CLA, 0xe4, 0x00, 0x00, 0x02]
        fid_h = fid/256
        fid_l = fid%256
        data = [fid_h, fid_l]
        response, sw1, sw2 = cardservice.connection.transmit(header+data)
    
        if((sw1==0x90) and (sw2==0x00)):
            if DEBUG :print "File deleted"
            return True
    except SWException, e:
        print str(e)
        return False

        
def read_binary(offset, length):
    global cardservice
    try:
        offset_h = offset/256
        offset_l = offset%256
        header = [CLA, 0xb0, offset_h, offset_l, length]
        response, sw1, sw2 = cardservice.connection.transmit(header)
        
        if((sw1==0x90) and (sw2==0x00)):
            return response
    except SWException, e:
        print str(e)
        return False

        
def update_binary(offset, length, data):
    global cardservice
    try:
        offset_h = offset/256
        offset_l = offset%256
        header = [CLA, 0xd6, offset_h, offset_l, length]
        response, sw1, sw2 = cardservice.connection.transmit(header+data)
    
        if((sw1==0x90) and (sw2==0x00)):
            if DEBUG : print "File updated"
            return True
    except SWException, e:
        print str(e)
        return False

        
def verify(key):
    global cardservice
    try:
        keyStr = str(key)
        keyList = []
        for ch in keyStr:
            keyList.append(ord(ch))
        header = [CLA, 0x20, 0x00, 0x00, 0x04]
        data = keyList
        response, sw1, sw2 = cardservice.connection.transmit(header+data)
            
        if((sw1==0x90) and (sw2==0x00)):
            if DEBUG : print "PIN Verified"
            return True
    except SWException, e:
        print str(e)
        return False


def change_PIN(oldkey, newkey):
    global cardservice
    try:
        oldkeyStr = str(oldkey)
        oldkeyList = []
        for ch in oldkeyStr:
            oldkeyList.append(ord(ch))

        newkeyStr = str(newkey)
        newkeyList = []
        for ch in newkeyStr:
            newkeyList.append(ord(ch))

        header = [CLA, 0x24, 0x00, 0x00, 0x08]
        data = oldkeyList+newkeyList
        response, sw1, sw2 = cardservice.connection.transmit(header+data)
            
        if((sw1==0x90) and (sw2==0x00)):
            if DEBUG : print "PIN Changed"
            return True
    except SWException, e:
        print str(e)
        return False


def get_response(length):
    global cardservice    
    try:
        header = [CLA, 0xc0, 0x00, 0x00, length]
        response, sw1, sw2 = cardservice.connection.transmit(header)
    
        if(sw1==0x90 and sw2==0x00):
            return response
    except SWException, e:
        print str(e)
        return False


def get_challange():
    global cardservice    
    try:
        header = [CLA, 0x84, 0x00, 0x00, 0x08]
        response, sw1, sw2 = cardservice.connection.transmit(header)
    
        if(sw1==0x90 and sw2==0x00):
            return response
    except SWException, e:
        print str(e)
        return False


def external_authenticate():
    global cardservice    
    try:
        challange = get_challange()
        challange_str = ''
        for x in challange:
            challange_str += chr(x)
        key = [0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF]
        key_str = ''
        for x in key:
            key_str += chr(x)
        encrypted = tea.encrypt(challange_str, key_str)
        data = []
        for x in encrypted:
            data.append(ord(x))
        header = [CLA, 0x82, 0x00, 0x00, 0x08]
        response, sw1, sw2 = cardservice.connection.transmit(header+data)
    
        if(sw1==0x90 and sw2==0x00):
            if DEBUG : print "Terminal (external) Authenticated"
            return response
    except SWException, e:
        print str(e)
        return False

        
def internal_authenticate():
    global cardservice    
    try:
        challange_str = 'abcdefgh'
        data = []
        for x in challange_str:
            data.append(ord(x))
        header = [CLA, 0x88, 0x00, 0x00, 0x08]
        response, sw1, sw2 = cardservice.connection.transmit(header+data)
    
        if(sw1==0x61):
            response = get_response(sw2)
            response_str = ''
            for x in response:
                response_str += chr(x)
            if DEBUG : print "Response length : ", len(response_str)

            key = [0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF]
            key_str = ''
            for x in key:
                key_str += chr(x)

            encrypted = tea.encrypt(challange_str, key_str)
            
            if(response_str==encrypted[:8]):
                if DEBUG : print "Card is authenticated"
                return True
            else:
                if DEBUG : print "Card is not authenticated"
                return False
    except SWException, e:
        print str(e)
        return False
