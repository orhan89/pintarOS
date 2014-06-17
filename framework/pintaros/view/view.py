from pintaros import fields
from pintaros.model import model_pool
import gtk

DEBUG = 1

view_pool = {}

VIEW_TYPE = { 'SIMPLE' : 0,
              'RECORD' : 1 }
              
class ViewMeta(type):
    def __init__(cls, name, bases, attrs):
        super(ViewMeta, cls).__init__(name, bases, attrs)
        if cls.__name__ not in ['View', 'SimpleView', 'RecordView']:
            print "Initialize View : %s(%s)" %(name,bases)

            view_pool[name] = cls

            
class View(object):
    __metaclass__ = ViewMeta

    _model_name = ''
    _model = None
    _columns = []
    _pre_columns = []
    _value = {}
    _name = None
    _engine = None

    def __init__(self):
        self.widget_pool = {}
        self._model = self.get_model_from_pool()
        
    def register_engine(self, engine):
        self._engine = engine

    def get_model(self):
        return self._model

    def get_model_from_pool(self):
        from pintaros.model import model_pool
        if(model_pool.has_key(self._model_name)):
            return model_pool[self._model_name]()

    def read(self, widget=None, engine=None):
        model = self.get_model()

        if self._engine is not None:
            self._engine.init_progress()

        for idx, item in enumerate(self._columns):
            content = model.read(item)
            model._columns[item].set_value(content)
            self._value[item] = content
            if self._engine is not None:
                # print "progress %", (float(offset_tmp)/self._length)
                self._engine.set_progress(float(idx)/len(self._columns))

        if self._engine is not None:
            # print "progress %", (float(offset_tmp)/self._length)
            self._engine.set_progress(float(1))

        if DEBUG: print "VALUE =",self._value

    def read_bulk(self, widget=None):
        model = self.get_model()

        content_bulk = model.read_bulk(self._engine)

        for item in self._columns:
            model._columns[item].set_value(content_bulk[item])
            self._value[item] = content_bulk[item]

        if DEBUG: print "VALUE =",self._value

    def update(self, widget=None):
        model = self.get_model()

        for item in self._columns:
            oldValue = self._value[item]
            newValue = model._columns[item].get_value()

            if (newValue!=oldValue):
                if DEBUG : print "Updating ",item," from ",oldValue," to ",newValue
                model.update(item, newValue)
                self._value[item] = newValue

    def on_switch_page(self,page,page_num,user_param):
        self.read_bulk()
        

class SimpleView(View):
    _type = VIEW_TYPE['SIMPLE']

    def __init__(self):
        super(SimpleView, self).__init__()

class RecordView(View):
    _type = VIEW_TYPE['RECORD']

    def __init__(self):
        super(RecordView, self).__init__()
        self.records_set = self.get_records()
        self._pre_columns.append(fields.TreeFields('records_set',self.records_set,self.on_table_click))

    def get_records(self):
        model = self.get_model()
        records = []

        for x in range(1,10):
            model.set_record(x)
            if (str(model.read('tanggal_periksa')) != '0001-01-01'):
                records.append([ model.read('tanggal_periksa'),
                                 model.read('keluhan_utama'),
                                 model.read('systole'),
                                 model.read('diastole'),
                                 model.read('nadi'), ])
                model.lastrecord = x

        return records

    def render(self, main):
        model = self.get_model()

        namelabel = gtk.Label(self._name)

        scrolled_grid = gtk.ScrolledWindow()
        scrolled_grid.set_border_width(10)

        scrolled_grid.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)

        main.append_page(scrolled_grid,namelabel)
        scrolled_grid.show()

        grid = gtk.Table(rows=len(self._columns)+8, columns=2, homogeneous=gtk.TRUE)
        scrolled_grid.add_with_viewport(grid)
        grid.show()


        for idx,item in enumerate(self._columns):
            self.widget_pool[item] = model._columns[item].render()
            if DEBUG : print self.widget_pool[item]
            grid.attach(self.widget_pool[item]['label'],0,1,idx+6,idx+7)

            hbox = gtk.HBox(gtk.TRUE, 0)
            for content_item in self.widget_pool[item]['content']:
                hbox.pack_start(content_item, gtk.TRUE, gtk.TRUE, 0)
            hbox.show()
            grid.attach(hbox,1,2,idx+6,idx+7)

        hbox = gtk.HBox(gtk.FALSE, 0)
        grid.attach(hbox,0,2,len(self._columns)+7, len(self._columns)+8)
        hbox.show()

        readBtn = gtk.Button(label="Read", stock=None)
        readBtn.connect("clicked", self.read)
        readBtn.show()
        hbox.pack_start(readBtn, gtk.FALSE, gtk.TRUE, 0)

        updateBtn = gtk.Button(label="Update", stock=None)
        updateBtn.connect("clicked", self.update)
        updateBtn.show()
        hbox.pack_start(updateBtn, gtk.FALSE, gtk.TRUE, 0)

        createBtn = gtk.Button(label="Create", stock=None)
        createBtn.connect("clicked", self.create_record)
        createBtn.show()
        hbox.pack_start(createBtn, gtk.FALSE, gtk.TRUE, 0)

                
        #main.connect("switch-page", self.on_switch_page)
        #self.read_bulk()

    def on_table_click(self,treeview, path, column):
        model = self.get_model()
        print "PATH ",path
        model.set_record(path[0]+1)
        self.read()

    def create_record(self, widget=None):
        model = self.get_model()
        model.set_record(model.lastrecord+1)
        self.record_list.append(['','','','',''])
        self.record_table.set_cursor((model.lastrecord,),None, False)
        self.clearwidget()

        for item in self._columns:
            self._value[item] = None

        model.lastrecord += 1

    def clearwidget(self):
        for widget in self.widget_pool:
            if isinstance(self.widget_pool[widget]['content'][0], gtk.Entry):
                print "Entry"
                print self.widget_pool[widget]['content'][0]
                self.widget_pool[widget]['content'][0].set_text('')
            elif isinstance(self.widget_pool[widget]['content'][0], gtk.RadioButton):
                print "Radio"
                self.widget_pool[widget]['content'][0].set_active(0)
            elif isinstance(self.widget_pool[widget]['content'][0], gtk.ComboBox):
                print "Combo"
                self.widget_pool[widget]['content'][0].set_active(0)
