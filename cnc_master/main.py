import tkinter as tk
from tkinter import filedialog
import g_code_parser
from serial.tools import list_ports
import comm


class ConnectionFrame:
    def __init__(self, parent, connect_cb):
        comm_list = []
        for port, desc, hwid in list_ports.comports():
            comm_list.append(port)

        self.frame = tk.Frame(parent, borderwidth=2, relief='groove')

        desc = tk.Label(self.frame, text="Connection")
        desc.grid(row=0)

        self.current_text = tk.StringVar()
        self.current_text.set("None")
        current_label = tk.Label(self.frame, textvariable=self.current_text)
        current_label.grid(row=1)

        connect = tk.Button(self.frame, text="Connect", command=connect_cb)
        connect.grid(row=2)

        self.current_port = tk.StringVar()

        self.dropdown = tk.OptionMenu(self.frame, self.current_port, comm_list)
        self.dropdown.grid(row=3)

    def grid(self, *args, **kwargs):
        self.frame.grid(*args, **kwargs)

    def get_selected(self):
        return self.current_port.get()[2:-3]

    def set_connected(self, text):
        self.current_text.set(text)


class MainWindow:
    def import_g_code(self):
        print("Selecting G Code")
        file = filedialog.askopenfilename(title="Select G-Code", filetypes=(("G-Code", "*.gcode"), ("All Files", "*.*")))
        if not file:
            print("Dialog exited with no selection")
            return

        print("Parsing " + str(file))
        self.file_parser.parse_file(file)

        self.file_name.set(str(file))
        print("G Code parsed")

        self.run_button.config(state='active')

    def set_state(self, state):
        print("State is " + str(state))
        self.state_text.set("State: " + str(state))
        self.root.update()

    def is_connected(self):
        print("Connection established")

        self.set_state("Connected")
        self.menubar.entryconfig(1, state='active')

    def connect(self):
        print("Attempting to connect")
        port = self.connection_box.get_selected()
        if not port:
            print("No port selected")
            return
        print("Connecting on port " + str(port))
        self.pic = comm.PicComm(port, 115200)
        try:
            ticks_per_meter, maximums, inc_values = self.pic.connect()
        except ConnectionError:
            print("Connection failed")
            return
        print(inc_values)
        self.file_parser = g_code_parser.Parser(maximums[0], maximums[1], inc_values, ticks_per_meter)
        self.connection_box.set_connected(port)
        self.is_connected()

    def update(self):
        self.root.update()

    def run_code(self):
        self.set_state("Running")
        self.pic.write_message(self.file_parser.instructions, update_cb=self.update)
        self.set_state("Connected")

    def __init__(self):
        print("Starting GUI")
        self.root = tk.Tk()
        self.root.title("CNC Controller")
        self.pic = None

        self.file_parser = None

        self.menubar = tk.Menu(self.root)
        self.menubar.add_command(label='Import', command=self.import_g_code, state="disabled")

        self.root.config(menu=self.menubar)

        self.file_name = tk.StringVar()
        current_file = tk.Label(self.root, textvariable=self.file_name)
        current_file.grid(sticky='EW')
        self.file_name.set("None")

        self.run_button = tk.Button(text="Run", command=self.run_code, state='disabled')
        self.run_button.grid(row=1, sticky='N')

        self.state_text = tk.StringVar()
        state_label = tk.Label(self.root, textvariable=self.state_text)
        state_label.grid(row=2, columnspan=2,sticky='S')
        self.set_state("Disconnected")

        self.connection_box = ConnectionFrame(self.root, self.connect)
        self.connection_box.grid(row=0, column=1, sticky='NE', rowspan=2)

        self.root.columnconfigure(0, weight=1)
        self.root.rowconfigure(1, weight=1)

        self.root.mainloop()
        print("GUI exited")


MainWindow()




#
#file_parser.parse_file(input_file)


#ic.write_message(file_parser.instructions)

