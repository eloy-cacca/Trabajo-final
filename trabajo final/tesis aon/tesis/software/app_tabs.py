"""
File that describes the different tabs used
on the right panel of the GUI.

"""
# ------ Module imports ------ #
import logging as log
import csv
from datetime import datetime

# ------ Tkinter imports ------- #
import tkinter as tk
from tkinter import ttk
import tkinter.scrolledtext

# ------ Plot imports ------- #
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec

plt.style.use("ggplot")

# ------ Local function imports ------- #
from plot_panel import PlotPanel
from utils.settings import Settings
from utils.misc import validate_number
from msg import raw_data_logging_b     #TODO: fix, horrible


class Test1(ttk.Frame):
    """
    Test 1 tab

    Used for general system information (to be modified)
    """
    def __init__(self, master, backend_api):
        super().__init__(master)
        self.wg = {}
        self.backend_api = backend_api
        self.padx = 10
        self.pady = 5
        self.panel_list = []
        self.plot = PlotPanel(self, backend_api)
        self.top_panel()
        self.geometry_setup()

    def top_panel(self):
        self.wg["top"] = {}
        self.wg["top"]["master"] = ttk.Frame(self)

        # Validates if is digit entry
        vcmd = self.register(validate_number)

        # left panel
        self.wg["top"]["left"] = {}
        self.wg["top"]["left"]["master"] = ttk.Frame(self.wg["top"]["master"])

        self.wg["top"]["left"]["lbl_title"          ] = ttk.Label (self.wg["top"]["left"]["master"],     text="Frequency response without THD control test"      )
        self.wg["top"]["left"]["lbl_io_rms"         ] = ttk.Label (self.wg["top"]["left"]["master"],     text="Io [Arms]:"                                        )
        self.wg["top"]["left"]["lbl_start_freq"     ] = ttk.Label (self.wg["top"]["left"]["master"],     text="Start freq [Hz]:"                                 )
        self.wg["top"]["left"]["lbl_end_freq"       ] = ttk.Label (self.wg["top"]["left"]["master"],     text="End freq [Hz]:"                                   )
        self.wg["top"]["left"]["lbl_step_freq"      ] = ttk.Label (self.wg["top"]["left"]["master"],     text="Step freq [Hz]:"                                  )

        self.wg["top"]["left"]["ent_io_rms"         ] = ttk.Entry (self.wg["top"]["left"]["master"],     validate="key", validatecommand=(vcmd, '%P') )
        self.wg["top"]["left"]["ent_start_freq"     ] = ttk.Entry (self.wg["top"]["left"]["master"],     validate="key", validatecommand=(vcmd, '%P') )
        self.wg["top"]["left"]["ent_end_freq"       ] = ttk.Entry (self.wg["top"]["left"]["master"],     validate="key", validatecommand=(vcmd, '%P') )
        self.wg["top"]["left"]["ent_step_freq"      ] = ttk.Entry (self.wg["top"]["left"]["master"],     validate="key", validatecommand=(vcmd, '%P') )

        self.wg["top"]["left"]["lbl_title"          ]   .grid(row=0,    column=0,    sticky="w",    padx=2,    pady=(2, 10),    columnspan=2)
        self.wg["top"]["left"]["lbl_io_rms"         ]   .grid(row=1,    column=0,    sticky="e",    padx=10,   pady=2                       )
        self.wg["top"]["left"]["lbl_start_freq"     ]   .grid(row=2,    column=0,    sticky="e",    padx=10,   pady=2                       )
        self.wg["top"]["left"]["lbl_end_freq"       ]   .grid(row=3,    column=0,    sticky="e",    padx=10,   pady=2                       )
        self.wg["top"]["left"]["lbl_step_freq"      ]   .grid(row=4,    column=0,    sticky="e",    padx=10,   pady=2                       )
        self.wg["top"]["left"]["ent_io_rms"         ]   .grid(row=1,    column=1,    sticky="w",    padx=20,   pady=2                       )
        self.wg["top"]["left"]["ent_start_freq"     ]   .grid(row=2,    column=1,    sticky="w",    padx=20,   pady=2                       )
        self.wg["top"]["left"]["ent_end_freq"       ]   .grid(row=3,    column=1,    sticky="w",    padx=20,   pady=2                       )
        self.wg["top"]["left"]["ent_step_freq"      ]   .grid(row=4,    column=1,    sticky="w",    padx=20,   pady=2                       )

        self.wg["top"]["left"]["master"].pack(side="left", padx=self.padx, pady=self.pady)

        # right panel
        self.wg["top"]["right"] = {}
        self.wg["top"]["right"]["master"] = ttk.Frame(self.wg["top"]["master"])

        self.wg["top"]["right"]["btn_start"         ] = ttk.Button      (self.wg["top"]["right"]["master"],    text="Start",    command=self.backend_api.app_start_test1 )
        self.wg["top"]["right"]["btn_stop"          ] = ttk.Button      (self.wg["top"]["right"]["master"],    text="Stop",     command=self.backend_api.app_stop_test1, state="disabled"  )
        self.wg["top"]["right"]["btn_save_png"      ] = ttk.Button      (self.wg["top"]["right"]["master"],    text="Save PNG", command=self.plot.save_png               )
        self.wg["top"]["right"]["btn_save_csv"      ] = ttk.Button      (self.wg["top"]["right"]["master"],    text="Save CSV", command=self.plot.save_csv               )
        self.wg["top"]["right"]["progress_bar"      ] = ttk.Progressbar (self.wg["top"]["right"]["master"],    orient="horizontal", mode="determinate", length=400       )

        self.wg["top"]["right"]["btn_start"         ]   .grid(row=1,    column=0,    padx=20,   pady=2                       )
        self.wg["top"]["right"]["btn_stop"          ]   .grid(row=1,    column=1,    padx=20,   pady=2                       )
        self.wg["top"]["right"]["btn_save_png"      ]   .grid(row=1,    column=2,    padx=20,   pady=2                       )
        self.wg["top"]["right"]["btn_save_csv"      ]   .grid(row=1,    column=3,    padx=20,   pady=2                       )
        self.wg["top"]["right"]["progress_bar"      ]   .grid(row=2,    column=0,               pady=10,    columnspan=4     )

        self.wg["top"]["right"]["master"].grid_rowconfigure((0,3), weight=1)
        self.wg["top"]["right"]["master"].pack(side="left", padx=self.padx, pady=self.pady, fill="both", expand=1)

    def progress_update(self, value):
        self.wg["top"]["right"]["progress_bar"]["value"] = value
        self.wg["top"]["right"]["progress_bar"].update()

    def geometry_setup(self):
        self.wg["top"]["master"].pack(expand=0, fill="x", pady=5)     # top panel
        ttk.Separator(master=self, orient=tk.HORIZONTAL).pack(expand=0, fill="both")
        self.plot.pack(expand=1, fill="both")  # image

    def enable_widgets(self, enable):
        # List entry widgets
        ents = [
            self.wg["top"]["left"]["ent_io_rms"],
            self.wg["top"]["left"]["ent_start_freq"],
            self.wg["top"]["left"]["ent_end_freq"],
            self.wg["top"]["left"]["ent_step_freq"]
        ]

        # Handle connection
        if enable:
            [ent.config(state="disabled") for ent in ents]
            self.wg["top"]["right"]["btn_start"].config(state="disabled")
            self.wg["top"]["right"]["btn_stop"].config(state="enabled")
        else:
            [ent.config(state="enabled") for ent in ents]
            self.wg["top"]["right"]["btn_start"].config(state="enabled")
            self.wg["top"]["right"]["btn_stop"].config(state="disabled")

class Test2(ttk.Frame):
    """
    Test 2
    """
    def __init__(self, master, backend_api):
        super().__init__(master)
        self.wg = {}
        self.backend_api = backend_api
        self.padx = 10
        self.pady = 5
        self.panel_list = []
        self.plot = PlotPanel(self, backend_api)
        self.top_panel()
        self.geometry_setup()

    def top_panel(self):
        self.wg["top"] = {}
        self.wg["top"]["master"] = ttk.Frame(self)

        # Validates if is digit entry
        vcmd = self.register(validate_number)

        # left panel
        self.wg["top"]["left"] = {}
        self.wg["top"]["left"]["master"] = ttk.Frame(self.wg["top"]["master"])

        self.wg["top"]["left"]["lbl_title"          ] = ttk.Label (self.wg["top"]["left"]["master"],     text="Frequency response with THD control test"         )
        self.wg["top"]["left"]["lbl_io_rms"         ] = ttk.Label (self.wg["top"]["left"]["master"],     text="Io [Arms]:"                                        )
        self.wg["top"]["left"]["lbl_start_freq"     ] = ttk.Label (self.wg["top"]["left"]["master"],     text="Start freq [Hz]:"                                 )
        self.wg["top"]["left"]["lbl_end_freq"       ] = ttk.Label (self.wg["top"]["left"]["master"],     text="End freq [Hz]:"                                   )
        self.wg["top"]["left"]["lbl_step_freq"      ] = ttk.Label (self.wg["top"]["left"]["master"],     text="Step freq [Hz]:"                                  )
        self.wg["top"]["left"]["lbl_max_twd"        ] = ttk.Label (self.wg["top"]["left"]["master"],     text="Max TWD [%]:"                                     )

        self.wg["top"]["left"]["ent_io_rms"         ] = ttk.Entry (self.wg["top"]["left"]["master"],     validate="key", validatecommand=(vcmd, '%P') )
        self.wg["top"]["left"]["ent_start_freq"     ] = ttk.Entry (self.wg["top"]["left"]["master"],     validate="key", validatecommand=(vcmd, '%P') )
        self.wg["top"]["left"]["ent_end_freq"       ] = ttk.Entry (self.wg["top"]["left"]["master"],     validate="key", validatecommand=(vcmd, '%P') )
        self.wg["top"]["left"]["ent_step_freq"      ] = ttk.Entry (self.wg["top"]["left"]["master"],     validate="key", validatecommand=(vcmd, '%P') )
        self.wg["top"]["left"]["ent_max_twd"        ] = ttk.Entry (self.wg["top"]["left"]["master"],     validate="key", validatecommand=(vcmd, '%P') )

        self.wg["top"]["left"]["lbl_title"          ]   .grid(row=0,    column=0,    sticky="w",    padx=2,    pady=(2, 10),    columnspan=2)
        self.wg["top"]["left"]["lbl_io_rms"         ]   .grid(row=1,    column=0,    sticky="e",    padx=10,   pady=2                       )
        self.wg["top"]["left"]["lbl_start_freq"     ]   .grid(row=2,    column=0,    sticky="e",    padx=10,   pady=2                       )
        self.wg["top"]["left"]["lbl_end_freq"       ]   .grid(row=3,    column=0,    sticky="e",    padx=10,   pady=2                       )
        self.wg["top"]["left"]["lbl_step_freq"      ]   .grid(row=4,    column=0,    sticky="e",    padx=10,   pady=2                       )
        self.wg["top"]["left"]["lbl_max_twd"        ]   .grid(row=5,    column=0,    sticky="e",    padx=10,   pady=2                       )
        self.wg["top"]["left"]["ent_io_rms"         ]   .grid(row=1,    column=1,    sticky="w",    padx=20,   pady=2                       )
        self.wg["top"]["left"]["ent_start_freq"     ]   .grid(row=2,    column=1,    sticky="w",    padx=20,   pady=2                       )
        self.wg["top"]["left"]["ent_end_freq"       ]   .grid(row=3,    column=1,    sticky="w",    padx=20,   pady=2                       )
        self.wg["top"]["left"]["ent_step_freq"      ]   .grid(row=4,    column=1,    sticky="w",    padx=20,   pady=2                       )
        self.wg["top"]["left"]["ent_max_twd"        ]   .grid(row=5,    column=1,    sticky="w",    padx=20,   pady=2                       )

        self.wg["top"]["left"]["master"].pack(side="left", padx=self.padx, pady=self.pady)

        # right panel
        self.wg["top"]["right"] = {}
        self.wg["top"]["right"]["master"] = ttk.Frame(self.wg["top"]["master"])

        self.wg["top"]["right"]["btn_start"         ] = ttk.Button      (self.wg["top"]["right"]["master"],    text="Start",    command=self.backend_api.app_start_test2 )
        self.wg["top"]["right"]["btn_stop"          ] = ttk.Button      (self.wg["top"]["right"]["master"],    text="Stop",     command=self.backend_api.app_stop_test2, state="disabled"  )
        self.wg["top"]["right"]["btn_save_png"      ] = ttk.Button      (self.wg["top"]["right"]["master"],    text="Save PNG", command=self.plot.save_png               )
        self.wg["top"]["right"]["btn_save_csv"      ] = ttk.Button      (self.wg["top"]["right"]["master"],    text="Save CSV", command=self.plot.save_csv               )
        self.wg["top"]["right"]["progress_bar"      ] = ttk.Progressbar (self.wg["top"]["right"]["master"],    orient="horizontal", mode="determinate", length=400       )

        self.wg["top"]["right"]["btn_start"         ]   .grid(row=1,    column=0,    padx=20,   pady=2                       )
        self.wg["top"]["right"]["btn_stop"          ]   .grid(row=1,    column=1,    padx=20,   pady=2                       )
        self.wg["top"]["right"]["btn_save_png"      ]   .grid(row=1,    column=2,    padx=20,   pady=2                       )
        self.wg["top"]["right"]["btn_save_csv"      ]   .grid(row=1,    column=3,    padx=20,   pady=2                       )
        self.wg["top"]["right"]["progress_bar"      ]   .grid(row=2,    column=0,               pady=10,    columnspan=4     )

        self.wg["top"]["right"]["master"].grid_rowconfigure((0,3), weight=1)
        self.wg["top"]["right"]["master"].pack(side="left", padx=self.padx, pady=self.pady, fill="both", expand=1)

    def progress_update(self, value):
        self.wg["top"]["right"]["progress_bar"]["value"] = value
        self.wg["top"]["right"]["progress_bar"].update()

    def geometry_setup(self):
        self.wg["top"]["master"].pack(expand=0, fill="x", pady=5)     # top panel
        ttk.Separator(master=self, orient=tk.HORIZONTAL).pack(expand=0, fill="both")
        self.plot.pack(expand=1, fill="both")  # image

    def enable_widgets(self, enable):
        # List entry widgets
        ents = [
            self.wg["top"]["left"]["ent_io_rms"],
            self.wg["top"]["left"]["ent_start_freq"],
            self.wg["top"]["left"]["ent_end_freq"],
            self.wg["top"]["left"]["ent_step_freq"]
        ]

        # Handle connection
        if enable:
            [ent.config(state="disabled") for ent in ents]
            self.wg["top"]["right"]["btn_start"].config(state="disabled")
            self.wg["top"]["right"]["btn_stop"].config(state="enabled")
        else:
            [ent.config(state="enabled") for ent in ents]
            self.wg["top"]["right"]["btn_start"].config(state="enabled")
            self.wg["top"]["right"]["btn_stop"].config(state="disabled")

class Generator(ttk.Frame):
    """
    Generator tab

    Use the device in open loop.
    """

    def __init__(self, master, backend_api):
        super().__init__(master)
        self.wg = {}
        self.backend_api = backend_api
        self.padx = 10
        self.pady = 5
        self.panel_list = []
        self.top_panel()
        self.bottom_panel()
        self.geometry_setup()

    def top_panel(self):
        self.wg["top"] = {}
        self.wg["top"]["master"] = ttk.Frame(self)

        # Validates if is digit entry
        vcmd = self.register(validate_number)

        # left panel
        def cmb_lut_enable(event):
            if self.wg["top"]["left"]["cmb_lut"].get() == "Custom":
                self.wg["top"]["left"]["btn_open_lut"]["state"] = "enabled"
            else:
                self.wg["top"]["left"]["btn_open_lut"]["state"] = "disabled"

        def open_csv_lut():
            file = tk.filedialog.askopenfilename(filetypes=[("CSV", ".csv")])
            with open(file, newline="") as f:
                reader = csv.reader(f)
                lut_list = list(reader)
            if len(lut_list) != 1:
                tk.messagebox.showerror(title="Error", message="Please check that the CSV file is only one row")
            elif (len(lut_list[0]) != Settings.LUT_SIZE):
                tk.messagebox.showerror(title="Error", message="Please check that the LUT has only {} elements.".format(Settings.LUT_SIZE))
            else:
                self.wg["top"]["left"]["custom_lut"] = lut_list[0]

        self.wg["top"]["left"] = {}
        self.wg["top"]["left"]["master"] = ttk.Frame(self.wg["top"]["master"])

        width_left = 20
        self.wg["top"]["left"]["lbl_title"      ] = ttk.Label    ( self.wg["top"]["left"]["master"], text="Generator mode",                                                                               )
        self.wg["top"]["left"]["lbl_io_rms"     ] = ttk.Label    ( self.wg["top"]["left"]["master"], text="Io [Arms]:",                                                                                   )
        self.wg["top"]["left"]["lbl_freq"       ] = ttk.Label    ( self.wg["top"]["left"]["master"], text="Frequency [Hz]:",                                                                              )
        self.wg["top"]["left"]["lbl_lut"        ] = ttk.Label    ( self.wg["top"]["left"]["master"], text="Waveform:",                                                                                    )
        self.wg["top"]["left"]["lbl_lut_sel"    ] = ttk.Label    ( self.wg["top"]["left"]["master"], text="Custom LUT:",                                                                                  )
        self.wg["top"]["left"]["ent_io_rms"     ] = ttk.Entry    ( self.wg["top"]["left"]["master"], validate='all', validatecommand=(vcmd, '%P'),                                    width=width_left    )
        self.wg["top"]["left"]["ent_freq"       ] = ttk.Entry    ( self.wg["top"]["left"]["master"], validate='all', validatecommand=(vcmd, '%P'),                                    width=width_left    )
        self.wg["top"]["left"]["cmb_lut"        ] = ttk.Combobox ( self.wg["top"]["left"]["master"], values=["Sine", "Square", "Triangular", "Sawtooth", "Custom"], state="readonly", width=width_left-3  )
        self.wg["top"]["left"]["btn_open_lut"   ] = ttk.Button   ( self.wg["top"]["left"]["master"], text="Open CSV", command=open_csv_lut,                                           width=width_left    )

        self.wg["top"]["left"]["cmb_lut"].bind("<<ComboboxSelected>>", cmb_lut_enable)
        self.wg["top"]["left"]["btn_open_lut"]["state"] = "disabled"

        self.wg["top"]["left"]["lbl_title"      ]   .grid( row=0,  column=0,    columnspan=2,  sticky="w",     padx=2 , pady=(self.pady, 10) )
        self.wg["top"]["left"]["lbl_io_rms"     ]   .grid( row=1,  column=0,                   sticky="e",     padx=2 , pady=2               )
        self.wg["top"]["left"]["lbl_freq"       ]   .grid( row=2,  column=0,                   sticky="e",     padx=2 , pady=2               )
        self.wg["top"]["left"]["lbl_lut"        ]   .grid( row=3,  column=0,                   sticky="e",     padx=2 , pady=2               )
        self.wg["top"]["left"]["lbl_lut_sel"    ]   .grid( row=4,  column=0,                   sticky="e",     padx=2 , pady=2               )
        self.wg["top"]["left"]["ent_io_rms"     ]   .grid( row=1,  column=1,                                   padx=20, pady=2               )
        self.wg["top"]["left"]["ent_freq"       ]   .grid( row=2,  column=1,                                   padx=20, pady=2               )
        self.wg["top"]["left"]["cmb_lut"        ]   .grid( row=3,  column=1,                                   padx=20, pady=2               )
        self.wg["top"]["left"]["btn_open_lut"   ]   .grid( row=4,  column=1,                                   padx=20, pady=2               )
        self.wg["top"]["left"]["master"         ]   .pack( side="left", padx=20, pady=self.pady                                              )

        # right panel
        self.wg["top"]["right"] = {}
        self.wg["top"]["right"]["master"] = ttk.Frame(self.wg["top"]["master"])
        self.wg["top"]["right"]["btn_start"] = ttk.Button(self.wg["top"]["right"]["master"], text="Start", command=self.backend_api.app_start_generator, width=30)
        self.wg["top"]["right"]["btn_stop"] = ttk.Button(self.wg["top"]["right"]["master"], text="Stop",   command=self.backend_api.app_stop_generator,  width=30, state="disabled")
        self.wg["top"]["right"]["btn_start"] .grid( row=1, column=0, padx=20, pady=5)
        self.wg["top"]["right"]["btn_stop"]  .grid( row=2, column=0, padx=20, pady=5)
        self.wg["top"]["right"]["master"].grid_rowconfigure((0,3), weight=1)
        self.wg["top"]["right"]["master"].pack(side="left", padx=self.padx, pady=self.pady, fill="both", expand=1)

    def bottom_panel(self):
        self.console = tk.scrolledtext.ScrolledText(self, state="disabled")

    def write_console(self, data):
        self._update_console(data)

    def geometry_setup(self):
        self.wg["top"]["master"].pack(expand=0, fill="x")
        self.console.pack(expand=1, fill="both", pady=5)

    def enable_widgets(self, enable):
        # List entry widgets
        ents = [
            self.wg["top"]["left"]["ent_io_rms"],
            self.wg["top"]["left"]["ent_freq"],
            self.wg["top"]["left"]["cmb_lut"]
        ]

        # Handle connection
        if enable:
            [ent.config(state="disabled") for ent in ents]
            self.wg["top"]["right"]["btn_start"].config(state="disabled")
            self.wg["top"]["right"]["btn_stop"].config(state="enabled")
        else:
            [ent.config(state="enabled") for ent in ents]
            self.wg["top"]["right"]["btn_start"].config(state="enabled")
            self.wg["top"]["right"]["btn_stop"].config(state="disabled")

    def _update_console(self, data):
        def update():
            text = datetime.now().strftime('%Y-%m-%d %H:%M:%S')
            text += "\n{}\n\n".format(data)
            self.console.configure(state='normal')

            self.console.insert(tk.END, text)
            self.console.configure(state='disabled')
            self.console.yview(tk.END)
        self.console.after(1, update)

class Logs(ttk.Frame):  #TODO: agregarle formato para mejor visualizacion
    """
    Logs
    """
    def __init__(self, master, backend_api):
        super().__init__(master)
        self.backend_api = backend_api
        self.padx = 4

        self.toolbar()
        self.logframe()
        self.geometry_setup()

        # Set default log value
        self.log_cmb.set("Warning")

    def toolbar(self):

        # Local functions
        def save_file_as():
            filename = tk.filedialog.asksaveasfilename(defaultextension='.txt',
                                                       filetypes = (("Plain Text", "*.txt"),
                                                                    ("Log", "*.log"),
                                                                    ("All Files", "*.*"))
                                                      )
            # Check if a file was selected and save it
            if filename != '':
                f = open(filename, 'w')
                f.write(self.console.get('1.0', 'end'))
                f.close()
                tk.messagebox.showinfo('Info', 'Logs Saved')

        def clear_console():
            self.console.configure(state='normal')
            self.console.delete('1.0', 'end')
            self.console.configure(state='disabled')

        def set_comm_cbtn_state():
            raw_data_logging_b[0] = self.comm_cbtn_state.get()

        self.comm_cbtn_state = tk.IntVar()

        # Widget definition
        self.tools      = tk.Frame      (self)
        self.log_lbl    = ttk.Label     (master=self.tools, text="Log level")
        self.log_cmb    = ttk.Combobox  (master=self.tools, state="readonly", values=["Critical", "Error", "Warning", "Info", "Debug"])
        self.save_btn   = ttk.Button    (master=self.tools, text="Save logs", command=save_file_as)
        self.clear_btn  = ttk.Button    (master=self.tools, text="Clear", command=clear_console)
        self.comm_cbtn  = ttk.Checkbutton(master=self.tools, text="Show raw data", variable=self.comm_cbtn_state, command=set_comm_cbtn_state)

        self.comm_cbtn_state.set(0)

        # Combobox
        def log_cmb_event(event):
            a = self.log_cmb.get()
            if a == "Critical":
                log.getLogger().setLevel(log.CRITICAL)
            elif a == "Error":
                log.getLogger().setLevel(log.ERROR)
            elif a == "Warning":
                log.getLogger().setLevel(log.WARNING)
            elif a == "Info":
                log.getLogger().setLevel(log.INFO)
            elif a == "Debug":
                log.getLogger().setLevel(log.DEBUG)
        self.log_cmb.bind("<<ComboboxSelected>>", log_cmb_event)

    def logframe(self):

        class TextHandler(log.Handler):
            """
            This class allows you to log to a Tkinter Text or ScrolledText widget
            """
            def __init__(self, text):
                log.Handler.__init__(self)
                formatter = log.Formatter('%(asctime)s - %(levelname)s - %(message)s ', datefmt='%Y-%m-%d %H:%M:%S')
                self.setFormatter(formatter)

                # Store text
                self.text = text

            def emit(self, record):
                msg = self.format(record)
                def append():
                    self.text.configure(state='normal')
                    self.text.insert(tk.END, msg + '\n')
                    self.text.configure(state='disabled')
                    # Autoscroll to the bottom
                    self.text.yview(tk.END)
                # This is necessary because we can't modify the Text from other threads
                self.text.after(0, append)

        self.console = tk.scrolledtext.ScrolledText(self, state="disabled")

        # Create textLogger
        text_handler = TextHandler(self.console)

        # Add the handler to logger
        logger = log.getLogger()
        logger.addHandler(text_handler)

    def geometry_setup(self):
        # Widget geometry
        self.log_lbl.pack(side="left", padx=self.padx)
        self.log_cmb.pack(side="left", padx=self.padx)
        self.save_btn.pack(side="left", padx=self.padx)
        self.clear_btn.pack(side="left", padx=self.padx)
        self.comm_cbtn.pack(side="left", padx=self.padx)

        # Frames geometry
        self.tools.pack(fill="x", pady=3)
        self.console.pack(fill="both", expand=1)

        # Focus set
        self.console.focus_set()

class AppTabs():
    def __init__(self, master, backend_api):
        self.test1 = Test1(master, backend_api)
        self.test2 = Test2(master, backend_api)
        self.generator = Generator(master, backend_api)
        self.logs = Logs(master, backend_api)