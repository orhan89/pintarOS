import math
import reader
import fields
import model

from collections import OrderedDict

DEBUG = 1

model_pool = {}

class ModelMeta(type):
    def __init__(cls, name, bases, attrs):
        super(ModelMeta, cls).__init__(name, bases, attrs)
        if cls.__name__ not in ['Model', 'SimpleModel', 'RecordModel']:
            print "Initialize Model : %s(%s)" %(name,bases)

            model_pool[cls._name] = cls

            cls._ordered_columns = OrderedDict(sorted(cls._columns.items(), key=lambda t: t[0]))
            if DEBUG : print "Model %s has column %s" %(name,[item for item in cls._ordered_columns])

            for item in cls._columns.values():
                cls._length += item.length
            if DEBUG : print "Model %s has length %d bytes" %(name,cls._length)

            if hasattr(cls, '_length_total'):
                cls._length_total = cls._length * cls._max_record

class Model(object):
    __metaclass__ = ModelMeta

    _name = ''
    _fid = None
    _columns = {}
    _ordered_columns = {}

    _length = 0
    
    def install(self):
        reader.select_file(0x3f00)
        if(reader.create_file(self._length, self._fid, 0x00, 0x00)):
            if DEBUG : print "Installed : %s" %self._name
            return True

    def update(self, column,  data):
        reader.select_file(self._fid)
        offset = self.get_offset(column)
        data_list = self._ordered_columns[column].format_to_card(data)
        if(reader.update_binary(offset, len(data_list), data_list)):
            if DEBUG : print column," Updated %s:%s with data : %s" %(self._name,column,data) 
            return True

    def read(self, column):
        reader.select_file(self._fid)
        offset = self.get_offset(column)
        column_length = self._ordered_columns[column].length
        data = reader.read_binary(offset, column_length)
        data_fmt = self._ordered_columns[column].format_to_terminal(data)
        if DEBUG : print "DATA : ", str(data_fmt)
        return data_fmt

    def read_bulk(self, engine=None):
        reader.select_file(self._fid)
        data = []
        length_tmp = self._length
        offset_tmp = 0

        if engine is not None:
            engine.init_progress()

        while length_tmp > 255:
            data += reader.read_binary(offset_tmp, 255)
            length_tmp -= 255
            offset_tmp += 255
            if engine is not None:
                # print "progress %", (float(offset_tmp)/self._length)
                engine.set_progress(float(offset_tmp)/self._length)

        data += reader.read_binary(offset_tmp, length_tmp)
        if engine is not None:
            engine.set_progress(1)

        data_bulk = {}
        for item in self._ordered_columns:
            item_offset = self.get_offset(item)
            item_length = self._ordered_columns[item].length
            data_item = data[(item_offset):(item_offset+item_length)]

            data_fmt = self._ordered_columns[item].format_to_terminal(data_item)
            data_bulk[item] = data_fmt
        if DEBUG : print "data formated = %s" %(str(data_bulk))

        return data_bulk

    def get_offset(self, key):
        offset = 0
        for item in self._ordered_columns:
            if (item == key):
                return offset
            else:
                offset += self._ordered_columns[item].length

class SimpleModel(Model):
    def __init__(self):
        super(SimpleModel, self).__init__()

class RecordModel(Model):
    _max_record = 1
    _last_record = 1
    _current_record = 1
    _length_total = 0

    def __init__(self):
        super(RecordModel,self).__init__()
        self._length_total = self._length * self._max_record

    def install(self):
        reader.select_file(0x3f00)
        if(reader.create_file(self._length_total, self._fid, 0x00, 0x00)):
            if DEBUG : print "Install successfull"
            return True

    def set_record(self, record_num):
        self._current_record = record_num
        if DEBUG : print "set current record", self._current_record

    def get_record(self, record_num):
        return self._current_record

    def get_offset(self,key):
        if DEBUG: print "current record", self._current_record
        offset = (self._current_record - 1) * self._length
        for item in self._ordered_columns:
            if (item == key):
                if DEBUG : print "offset", offset
                return offset
            else:
                offset += self._ordered_columns[item].length
