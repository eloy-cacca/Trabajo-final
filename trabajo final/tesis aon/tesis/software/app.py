# -------- Module imports ------- #
from ctypes import windll
import logging as log
import sys, os
import tkinter as tk
from tkinter import ttk

# ------- Local imports -------- #
from utils import misc
from app_tabs import AppTabs


class CommBar(ttk.Frame):
    """
    Communication bar. Opens ports, check if it's
    connected etc.
    """
    def __init__(self, master, backend_api):
        super().__init__(master)
        self.padx = 4
        self.backend_api = backend_api

        self.widget_init()
        self.widget_bind()
        self.geometry_setup()

    def widget_init(self):
        self.port_lbl    = ttk.Label    (master=self, text="COM port list")
        self.port_cmb    = ttk.Combobox (master=self, values=misc.get_serialports(), state="readonly")
        self.refresh_btn = ttk.Button   (master=self, text="Refresh port list", command=lambda: self.port_cmb.config(values=misc.get_serialports()))
        self.connect_btn = ttk.Button   (master=self, text="Connect", command=self.backend_api.app_connection)
        self.status_lbl  = tk.Label     (master=self, text="Disconnected", background="#ff4c4c", borderwidth=1,relief="solid", width=15, anchor="center")

    def widget_bind(self):
        # Select port
        self.port_cmb.bind("<<ComboboxSelected>>",
            lambda event: self.backend_api.app_selport(self.port_cmb.get()))

    def geometry_setup(self):
        self.port_lbl.pack(side="left", padx=self.padx)
        self.port_cmb.pack(side="left", padx=self.padx)
        self.refresh_btn.pack(side="left", padx=self.padx)
        self.connect_btn.pack(side="left", padx=self.padx)
        self.status_lbl.pack(side="right", padx=self.padx)

class SidePanel(ttk.Frame):
    """
    Side panel. Frame wrapper that organizes all
    side panel widgets.
    """
    def __init__(self, master, backend_api):
        super().__init__(master)
        self.backend_api = backend_api

        # Widget dictionary
        self.wg = {}

        # Panels
        self.fw()
        self.sys_enable()
        # self.mode()
        self.telemetry()
        # self.status()
        self.channels()
        self.plot()

        self.geometry_setup()

    def fw(self):
        self.wg["fw"] = {}
        self.wg["fw"]["master"] = ttk.Frame(self)
        master = self.wg["fw"]["master"]
        wg_dict = self.wg["fw"]

        misc.add_widgets(
            name_list=[
                "lbl_title",
                "lbl_uc_l",
                "lbl_last_build_l"
            ],
            master=master,
            tipo="label",
            columna=0,
            text=[
                "Firmware Version",
                "uC:",
                "Last built:"
            ],
            dict_to_add=wg_dict
        )
        misc.add_widgets(
            name_list=[
                "lbl_uc_r",
                "lbl_last_build_r"
            ],
            master=master,
            tipo="label",
            columna=1,
            fila=1,
            dict_to_add=wg_dict
        )

        wg_dict["lbl_title"].grid(columnspan=2, sticky="W")

        # Settings for background and border widgets
        wg_dict["lbl_uc_r"].configure(background="white", borderwidth=1, relief="solid", width=12, anchor="center")
        wg_dict["lbl_last_build_r"].configure(background="white", borderwidth=1, relief="solid", width=12, anchor="center")

        # Other settings
        master.columnconfigure(0, weight=1)
        master.columnconfigure(1, weight=1)

    def sys_enable(self):
        self.wg["sys_enable"] = {}
        self.wg["sys_enable"]["master"] = ttk.Frame(self)

        self.wg["sys_enable"]["lbl_title"               ] = ttk.Label   ( self.wg["sys_enable"]["master"],    text="System enable"                                      )
        self.wg["sys_enable"]["lbl_control"             ] = ttk.Label   ( self.wg["sys_enable"]["master"],    text="Control enable"                                     )
        self.wg["sys_enable"]["btn_control_enable"      ] = ttk.Button  ( self.wg["sys_enable"]["master"],    text="OFF",   command=self.backend_api.app_control_toggle )

        self.wg["sys_enable"]["lbl_title"               ]   .grid               ( row=0,    column=0,   sticky="W", pady=2  )
        self.wg["sys_enable"]["lbl_control"             ]   .grid               ( row=1,    column=0,               pady=2  )
        self.wg["sys_enable"]["btn_control_enable"      ]   .grid               ( row=2,    column=0,               pady=2  )
        self.wg["sys_enable"]["master"                  ]   .columnconfigure    ( 0,        weight=1                        )

    def mode(self):
        self.wg["mode"] = {}
        self.wg["mode"]["master"] = ttk.Frame(self)
        master = self.wg["mode"]["master"]
        wg_dict = self.wg["mode"]

        misc.add_widgets(
            name_list=["lbl_title"],
            master=master,
            tipo="label",
            text=["Mode selection"],
            dict_to_add=wg_dict
        )
        misc.add_widgets(
            name_list=[
                "rbtn_close_loop",
                "rbtn_open_loop"
            ],
            master=master,
            tipo="radiobutton",
            text=[
                "Close loop",
                "Open loop"
            ],
            fila=1,
            orden="columna",
            sticky="W",
            padx=10,
            dict_to_add=wg_dict
        )

        wg_dict["lbl_title"].grid(columnspan=1, sticky="W")
        master.columnconfigure(0, weight=1)

    def telemetry(self):
        self.wg["telemetry"] = {}
        self.wg["telemetry"]["master"] = ttk.Frame(self)

        self.wg["telemetry"]["lbl_title"                ]   =   ttk.Label   ( self.wg["telemetry"]["master"],     text="Telemetry"        )
        self.wg["telemetry"]["lbl_iopk"                 ]   =   ttk.Label   ( self.wg["telemetry"]["master"],     text="Irms [A]:"         )
        self.wg["telemetry"]["lbl_freq"                 ]   =   ttk.Label   ( self.wg["telemetry"]["master"],     text="Freq [Hz]:"       )
        self.wg["telemetry"]["lbl_waveform"             ]   =   ttk.Label   ( self.wg["telemetry"]["master"],     text="Waveform:"        )
        self.wg["telemetry"]["lbl_iopk_display"         ]   =   ttk.Label   ( self.wg["telemetry"]["master"],                             )
        self.wg["telemetry"]["lbl_freq_display"         ]   =   ttk.Label   ( self.wg["telemetry"]["master"],                             )
        self.wg["telemetry"]["lbl_waveform_display"     ]   =   ttk.Label   ( self.wg["telemetry"]["master"],                             )

        self.wg["telemetry"]["lbl_title"                ]   .grid   ( row=0,  column=0,    columnspan=2,  sticky="w",      padx=2,  pady=2  )
        self.wg["telemetry"]["lbl_iopk"                 ]   .grid   ( row=1,  column=0,                   sticky="e",      padx=2,  pady=2  )
        self.wg["telemetry"]["lbl_freq"                 ]   .grid   ( row=2,  column=0,                   sticky="e",      padx=2,  pady=2  )
        self.wg["telemetry"]["lbl_waveform"             ]   .grid   ( row=3,  column=0,                   sticky="e",      padx=2,  pady=2  )
        self.wg["telemetry"]["lbl_iopk_display"         ]   .grid   ( row=1,  column=1,                   sticky="e",      padx=2,  pady=2  )
        self.wg["telemetry"]["lbl_freq_display"         ]   .grid   ( row=2,  column=1,                   sticky="e",      padx=2,  pady=2  )
        self.wg["telemetry"]["lbl_waveform_display"     ]   .grid   ( row=3,  column=1,                   sticky="e",      padx=2,  pady=2  )

        self.wg["telemetry"]["lbl_iopk_display"         ]   .configure(background="white", borderwidth=1, relief="solid", width=12, anchor="center")
        self.wg["telemetry"]["lbl_freq_display"         ]   .configure(background="white", borderwidth=1, relief="solid", width=12, anchor="center")
        self.wg["telemetry"]["lbl_waveform_display"     ]   .configure(background="white", borderwidth=1, relief="solid", width=12, anchor="center")

    def status(self):
        self.wg["status"] = {}
        self.wg["status"]["master"] = ttk.Frame(self)
        master = self.wg["status"]["master"]
        wg_dict = self.wg["status"]

        misc.add_widgets(
            name_list=["lbl_title"],
            master=master,
            tipo="label",
            text=["Status uC"],
            dict_to_add=wg_dict
        )
        misc.add_widgets(
            name_list=["rbtn_uc_ok"],
            master=master,
            tipo="radiobutton",
            fila=1,
            text=["uC ok"],
            sticky="W",
            padx=10,
            dict_to_add=wg_dict
        )
        wg_dict["lbl_title"].grid(columnspan=2, sticky="W")
        master.columnconfigure(0, weight=1)

    def channels(self):

        def state_changed():
            enabled_ch = [
                self.wg["channels"]["ch1_state"].get(),
                self.wg["channels"]["ch2_state"].get(),
                self.wg["channels"]["ch3_state"].get(),
                self.wg["channels"]["ch4_state"].get()
            ]
            self.backend_api.app_set_num_plots(enabled_ch)

        self.wg["channels"] = {}
        master = ttk.Frame(self)
        self.wg["channels"]["master"] = master

        self.wg["channels"]["ch1_state"] = tk.IntVar()
        self.wg["channels"]["ch2_state"] = tk.IntVar()
        self.wg["channels"]["ch3_state"] = tk.IntVar()
        self.wg["channels"]["ch4_state"] = tk.IntVar()

        self.wg["channels"]["title" ] = ttk.Label       (master, text="Channel selection"                                                                  )
        self.wg["channels"]["ch1"   ] = ttk.Checkbutton (master, text="Channel 1 (shunt)", variable=self.wg["channels"]["ch1_state"], command=state_changed)
        self.wg["channels"]["ch2"   ] = ttk.Checkbutton (master, text="Channel 2"        , variable=self.wg["channels"]["ch2_state"], command=state_changed)
        self.wg["channels"]["ch3"   ] = ttk.Checkbutton (master, text="Channel 3"        , variable=self.wg["channels"]["ch3_state"], command=state_changed)
        self.wg["channels"]["ch4"   ] = ttk.Checkbutton (master, text="Channel 4"        , variable=self.wg["channels"]["ch4_state"], command=state_changed)

        self.wg["channels"]["title" ].grid(row=0, column=0, sticky="w", padx=4,  pady=2)
        self.wg["channels"]["ch1"   ].grid(row=1, column=0, sticky="w", padx=10, pady=2)
        self.wg["channels"]["ch2"   ].grid(row=2, column=0, sticky="w", padx=10, pady=2)
        self.wg["channels"]["ch3"   ].grid(row=3, column=0, sticky="w", padx=10, pady=2)
        self.wg["channels"]["ch4"   ].grid(row=4, column=0, sticky="w", padx=10, pady=2)
        master.columnconfigure(0, weight=1)

        # Default checkboxes
        self.wg["channels"]["ch1_state"].set(self.backend_api.enabled_channels[0])
        self.wg["channels"]["ch2_state"].set(self.backend_api.enabled_channels[1])
        self.wg["channels"]["ch3_state"].set(self.backend_api.enabled_channels[2])
        self.wg["channels"]["ch4_state"].set(self.backend_api.enabled_channels[3])

    def plot(self):

        def validate_harmonic_f(input):
            if input == "":
                return True
            try:
                self.wg["plot"]["harmonic_freq"] = [int(e) for e in input.rstrip(",").split(",")]
            except:
                return False
            else:
                self.backend_api.app_set_plot_harmonic_freq(self.wg["plot"]["harmonic_freq"])
                return True

        vcmd = self.register(validate_harmonic_f)

        def mode_sel():
            mode = self.wg["plot"]["mode"].get()
            self.backend_api.app_set_plot_mode(mode)
            if mode != "harmonic":
                self.wg["plot"]["ent_harmonics_f"].configure(state="disabled")
            else:
                self.wg["plot"]["ent_harmonics_f"].configure(state="normal")

        self.wg["plot"] = {}
        master = ttk.Frame(self)
        self.wg["plot"]["master"] = master

        self.wg["plot"]["mode"] = tk.StringVar()
        self.wg["plot"]["mode"].set("rms")
        self.wg["plot"]["harmonic_freq"] = []

        self.wg["plot"]["lbl_title"       ] = ttk.Label       (master, text="Plot selection   "                                                                       )
        self.wg["plot"]["rbtn_rms"        ] = ttk.Radiobutton (master, text="RMS"              , variable=self.wg["plot"]["mode"], value="rms"      , command=mode_sel)
        self.wg["plot"]["rbtn_bode"       ] = ttk.Radiobutton (master, text="Bode"             , variable=self.wg["plot"]["mode"], value="bode"     , command=mode_sel)
        self.wg["plot"]["rbtn_harmonics"  ] = ttk.Radiobutton (master, text="Harmonics"        , variable=self.wg["plot"]["mode"], value="harmonic",  command=mode_sel)
        self.wg["plot"]["ent_harmonics_f" ] = ttk.Entry       (master, validate="key"          , validatecommand=(vcmd, '%P'),     state="disabled"                   )

        self.wg["plot"]["lbl_title"       ].grid(row=0, column=0, sticky="w" , padx=4,  pady=2)
        self.wg["plot"]["rbtn_rms"        ].grid(row=1, column=0, sticky="w" , padx=10, pady=2)
        self.wg["plot"]["rbtn_bode"       ].grid(row=2, column=0, sticky="w" , padx=10, pady=2)
        self.wg["plot"]["rbtn_harmonics"  ].grid(row=3, column=0, sticky="w" , padx=10, pady=2)
        self.wg["plot"]["ent_harmonics_f" ].grid(row=4, column=0, sticky="we", padx=10, pady=2)
        master.columnconfigure(0, weight=1)

    def geometry_setup(self):
        self.config(borderwidth=1, relief="groove", width=200)

        for panel in self.wg:
            self.wg[panel]["master"].pack(expand=0, fill="both", padx=4)
            if panel != "plot":
                temp = ttk.Separator(master=self, orient=tk.HORIZONTAL)
                temp.pack(expand=0, fill="both", pady=5)

class TabPanel(ttk.Frame):
    """
    Tab panel, ttk.Notebook wrapper that adds
    all tabs from app_tabs.py
    """
    def __init__(self, master, backend_api):
        super().__init__(master)

        self.notebook = ttk.Notebook(self)
        self.config(borderwidth=1, relief="groove")

        self.tabs = AppTabs(self.notebook, backend_api)

        self.notebook.add(self.tabs.test1, text="Test 1")
        self.notebook.add(self.tabs.test2, text="Test 2")
        self.notebook.add(self.tabs.generator, text="Generator")
        self.notebook.add(self.tabs.logs, text="Logs")

        self.notebook.pack(expand=1, fill="both")

class App(tk.Tk):
    """
    GUI class. Handles Tkinter interface.

    The purpose of this class and all others in
    app.py is to organize the widgets and provide
    a structure.

    Attributes:
        self.backend_api: main comms self.backend_api
        app_tabs: tabs class
    """
    def __init__(self, backend_api):
        super().__init__()
        self.backend_api = backend_api
        self.window_init()
        self.widget_init()
        self.geometry_setup()
        # Tkinter "protocols"
        self.tkprotocols()
        windll.shcore.SetProcessDpiAwareness(True)

    def window_init(self):
        self.geometry("1000x750")
        self.title("Medidor de sondas de corriente - LIC")
        self.iconbitmap(os.path.join(sys.path[0], "img", "logo.ico"))

    def widget_init(self):
        self.comm_bar = CommBar(self, self.backend_api)
        self.side_panel = SidePanel(self, self.backend_api)
        self.tab_panel = TabPanel(self, self.backend_api)

    def geometry_setup(self):
        self.comm_bar.pack(expand=0, fill="x", pady=2)
        self.side_panel.pack(expand=0, fill="both", side="left")
        self.side_panel.pack_propagate(0)
        self.tab_panel.pack(expand=1, fill="both", side="right")

    def tkprotocols(self):
        def on_closing():

            # Close connections
            self.backend_api.app_end_connection()

            # Release logging handlers as GUI will be closed
            logger = log.getLogger()
            logger.handlers.clear()

            # Close app window
            self.destroy()

        self.protocol("WM_DELETE_WINDOW", on_closing)