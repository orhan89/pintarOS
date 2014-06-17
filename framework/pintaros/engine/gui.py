import pygtk; pygtk.require('2.0')
import gtk, gobject

from pintaros import reader
from pintaros.model import model_pool
from pintaros.view import view_pool, VIEW_TYPE

DEBUG = 1

def progress_timeout(pbobj):
    print "test"
    if False:
        pbobj.pbar.pulse()
    else:
        # Calculate the value of the progress bar using the
        # value range set in the adjustment object
        new_val = pbobj.pbar.get_fraction() + 0.01
        if new_val > 1.0:
            return False
        # Set the new value
        pbobj.pbar.set_fraction(new_val)

    # As this is a timeout function, return TRUE so that it
    # continues to get called
    return True

class Gui:
    def __init__(self):
        self.models = model_pool.values()
        self.views = view_pool.values()
        self.widget_pool = {}

    def run(self):
        reader.run()
        print "GUI Running..."
        # for model in self.models:            
        #     model()
        # for view in self.views:
        #     view().render(self.notebook)

        self.render()

    def render(self):
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.set_border_width(10)
        self.window.set_usize(200,100)

        self.window.connect("delete_event", self.delete_event)
        self.window.connect("destroy", self.destroy)

        vbox = gtk.VBox(False, 5)
        vbox.set_border_width(10)
        self.window.add(vbox)
        vbox.show()
  
        self.notebook = gtk.Notebook()
        self.notebook.set_tab_pos(gtk.POS_TOP)
        #self.window.add(self.notebook)
        vbox.pack_start(self.notebook, True, True, 0)
        self.notebook.show()

        separator = gtk.HSeparator()
        vbox.pack_start(separator, False, False, 0)
        separator.show()

        # Create the ProgressBar
        self.pbar = gtk.ProgressBar()
        vbox.pack_end(self.pbar, False, False, 0)
        self.pbar.show()

        #self.timer = gobject.timeout_add (100, progress_timeout, self)
        self.set_progress(float(1))

        for viewClass in self.views:
            view = viewClass()
            view.register_engine(self)

            model = view.get_model()
    
            namelabel = gtk.Label(view._name)
            
            scrolled_grid = gtk.ScrolledWindow()
            scrolled_grid.set_border_width(10)
    
            scrolled_grid.set_policy(gtk.POLICY_AUTOMATIC, gtk.POLICY_AUTOMATIC)
    
            self.notebook.append_page(scrolled_grid,namelabel)
            scrolled_grid.show()
    
            grid = gtk.Table(rows=len(view._columns)+len(view._pre_columns)+1, columns=2, homogeneous=gtk.FALSE)
            scrolled_grid.add_with_viewport(grid)
            grid.show()

            # if view._type is VIEW_TYPE['RECORD'] :

            for idx,item in enumerate(view._pre_columns):
                self.widget_pool[item] = view._pre_columns[idx].render()
                if DEBUG : print self.widget_pool[item]

                if 'label' in self.widget_pool[item] :
                    grid.attach(self.widget_pool[item]['label'],0,1,idx,idx+1)

                hbox = gtk.HBox(gtk.TRUE, 0)
                for content_item in self.widget_pool[item]['content']:
                    hbox.pack_start(content_item, gtk.TRUE, gtk.TRUE, 0)
                hbox.show()
                grid.attach(hbox,1 if 'label' in self.widget_pool[item] else 0, 2, idx, idx+1)

            for idx,item in enumerate(view._columns):
                self.widget_pool[item] = model._columns[item].render()
                if DEBUG : print self.widget_pool[item]
                grid.attach(self.widget_pool[item]['label'],0,1,idx+len(view._pre_columns),idx+1+len(view._pre_columns))
    
                hbox = gtk.HBox(gtk.TRUE, 0)
                for content_item in self.widget_pool[item]['content']:
                    hbox.pack_start(content_item, gtk.TRUE, gtk.TRUE, 0)
                hbox.show()
                grid.attach(hbox,1,2,idx+len(view._pre_columns),idx+1+len(view._pre_columns))
    
            hbox = gtk.HBox(gtk.FALSE, 0)
            grid.attach(hbox,0,2,len(view._columns)+len(view._pre_columns)+1, len(view._columns)+len(view._pre_columns)+2)
            hbox.show()
    	        
            readBtn = gtk.Button(label="Read", stock=None)
            readBtn.connect("clicked", view.read_bulk)
            readBtn.show()
            hbox.pack_start(readBtn, gtk.FALSE, gtk.TRUE, 0)
    	        
            updateBtn = gtk.Button(label="Update", stock=None)
            updateBtn.connect("clicked", view.update)
            updateBtn.show()
            hbox.pack_start(updateBtn, gtk.FALSE, gtk.TRUE, 0)
    
        #main.connect("switch-page", self.on_switch_page)
        #self.read_bulk()

        self.window.show()
        gtk.main()

    def init_progress(self):
        print "init progress"
        self.pbar.set_fraction(0)

    def set_progress(self, value):
        print "set progress", value
        self.pbar.set_fraction(value)
        print "get progress", self.pbar.get_fraction()

        while gtk.events_pending():
            gtk.main_iteration()

    def delete_event(self, widget, event, data=None):
        print "GUI exited..."
        return False

    def destroy(self, widget, data=None):
        gtk.main_quit()
