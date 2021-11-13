# -------- Module imports ------- #
from tkinter import messagebox
import logging as log
from prettytable import PrettyTable
import dataclasses

# ------- Local imports -------- #
from utils.misc import gen_LUT
from utils.settings import Settings
import app
import backend
import comm
import scheduler


class BackendApi:
    """
    Backend API, connects to:
        Backend: handles communication to uC and
            returns data
        Sched: scheduler that checks state and runs
            jobs accordingly
        Comm: methods starting with "comm" will connect
            directly to comm object

    Also, methods starting with different names refer
    to which parts call it.
    """
    def __init__(self, comm: comm.Comm):
        self.comm = comm
        self._reset_variables()
        self.enabled_channels = [0, 1, 0, 0]    # ch1, ch2, ch3, ch4
        self.app = app.App(self)
        self.backend = backend.Backend(self, comm)
        self.scheduler = scheduler.Scheduler(self.backend, self)
        self.app.mainloop()
        self.first_trend = True

    # App methods
    def app_connection(self):
        """
        Handle the connection button state, opens and
        closes the COM port and handles sched.
        """
        if self.connected_state:
            self.app_end_connection()
        else:
            self.comm.open()
            if self.comm.is_open():
                self.app.comm_bar.connect_btn.config(text="Disconnect")
                self.connected_state = True
                self.scheduler.enable()

    def app_end_connection(self, force: bool = False):
        """
        End connection with uC, if connected
        """
        if self.connected_state:
            if not force:
                if self.control_state:
                    self.backend.comm_control_stop_req()
            self.app.comm_bar.connect_btn.config(text="Connect")

            # Stop tests and generator
            self.app_stop_test1()
            self.app_stop_test2()
            self.app_stop_generator()

            self.scheduler.disable()
            self.connected_state = False
            self.comm.close()

    def app_control_toggle(self):
        """
        Toggle control state.
        """
        if self.connected_state:
            if self.control_state:
                self.scheduler.add_job(self.backend.comm_control_stop_req)
            else:
                self.scheduler.add_job(self.backend.comm_control_start_req)

    def app_selport(self, port):
        """
        Select port in comm.
        """
        self.comm.selport(port)

    def app_start_test1(self, next_freq: int = 0):
        """
        Start test 1.
        """
        if not self.connected_state:
            messagebox.showwarning(title="Test 1", message="Connect instrument before starting test")
            return
        if not self.control_state:
            messagebox.showwarning(title="Test 1", message="Enable control before starting test")
            return
        if (self.test1["enable"]):
            messagebox.showerror(title="Test 1", message="Test 1 is already running")
            return
        if (self.test2["enable"]):
            messagebox.showerror(title="Test 1", message="Test 2 is running")
            return
        if (self.generator_state):
            messagebox.showerror(title="Test 1", message="Generator mode is running")
            return

        # Clear previous results
        self.test1["freq_done"].clear()
        self.test1["trends"].clear()

        # List entry widgets
        ents = [
            self.app.tab_panel.tabs.test1.wg["top"]["left"]["ent_start_freq"],
            self.app.tab_panel.tabs.test1.wg["top"]["left"]["ent_end_freq"],
            self.app.tab_panel.tabs.test1.wg["top"]["left"]["ent_step_freq"],
            self.app.tab_panel.tabs.test1.wg["top"]["left"]["ent_io_rms"]
        ]

        # Enable
        self.app.tab_panel.tabs.test1.enable_widgets(True)
        self.first_trend = True
        self.test1["enable"] = True

        # Get parameters
        params = [param.get() for param in ents]
        for param in params:
            if param == "":
                messagebox.showerror(title="Test 1", message="Please complete all parameters before proceeding.")
                return
        params = [int(param) for param in params]
        start_freq, end_freq, step_freq, amp = params
        self.test1["params"] = {
            "start_freq": start_freq,
            "end_freq":   end_freq,
            "step_freq":  step_freq,
            "amp":        amp
        }

        # Start trend struct
        for f in range(start_freq, end_freq, step_freq):
            self.test1["freq_done"][f] = "waiting"
        self.test1["freq_done"][end_freq] = "waiting"

        # Disable entries
        [ent.config(state="disabled") for ent in ents]

        # Start periodic job - test1 handler
        self.scheduler.add_job(self.backend.comm_use_custom_lut_req, [0])
        self.scheduler.add_job(self.be_test1_handler)

    def app_start_test2(self, next_freq: int = 0):
        """
        Start test 2.
        """
        if not self.connected_state:
            messagebox.showwarning(title="Test 2", message="Connect instrument before starting test")
            return
        if not self.control_state:
            messagebox.showwarning(title="Test 2", message="Enable control before starting test")
            return
        if (self.test1["enable"]):
            messagebox.showerror(title="Test 2", message="Test 1 is running")
            return
        if (self.test2["enable"]):
            messagebox.showerror(title="Test 2", message="Test 2 is already running")
            return
        if (self.generator_state):
            messagebox.showerror(title="Test 2", message="Generator mode is running")
            return

        # Clear previous results
        self.test2["freq_done"].clear()
        self.test2["trends"].clear()

        # List entry widgets
        ents = [
            self.app.tab_panel.tabs.test2.wg["top"]["left"]["ent_start_freq"],
            self.app.tab_panel.tabs.test2.wg["top"]["left"]["ent_end_freq"],
            self.app.tab_panel.tabs.test2.wg["top"]["left"]["ent_step_freq"],
            self.app.tab_panel.tabs.test2.wg["top"]["left"]["ent_io_rms"]
        ]

        # Enable
        self.first_trend = True
        self.app.tab_panel.tabs.test2.enable_widgets(True)
        self.test2["enable"] = True

        # Get parameters
        params = [param.get() for param in ents]
        for param in params:
            if param == "":
                messagebox.showerror(title="Test 2", message="Please complete all parameters before proceeding.")
                return
        params = [int(param) for param in params]
        start_freq, end_freq, step_freq, amp = params
        self.test2["params"] = {
            "start_freq": start_freq,
            "end_freq":   end_freq,
            "step_freq":  step_freq,
            "amp":        amp
        }

        # Start trend struct
        for f in range(start_freq, end_freq, step_freq):
            self.test2["freq_done"][f] = "waiting"
        self.test2["freq_done"][end_freq] = "waiting"

        # Disable entries
        [ent.config(state="disabled") for ent in ents]

        # Start periodic job - test1 handler
        self.scheduler.add_job(self.backend.comm_use_custom_lut_req, [0])
        self.scheduler.add_job(self.be_test2_handler)

    def app_stop_test1(self):
        """
        Stop test 1.
        """
        self.app.tab_panel.tabs.test1.enable_widgets(False)

        # Set progress to 0
        self.app.tab_panel.tabs.test1.progress_update(0)

        # Disable test
        self.test1["enable"] = False

    def app_stop_test2(self):
        """
        Stop test 2.
        """
        self.app.tab_panel.tabs.test2.enable_widgets(False)

        # Set progress to 0
        self.app.tab_panel.tabs.test2.progress_update(0)

        # Disable test
        self.test2["enable"] = False

    def app_start_generator(self):
        """
        Starts generator
        """
        if not self.connected_state:
            messagebox.showwarning(title="Generator", message="Connect instrument before starting test")
            return
        if not self.control_state:
            messagebox.showwarning(title="Generator", message="Enable control before starting test")
            return
        if (self.test1["enable"]):
            messagebox.showerror(title="Generator", message="Test 1 is running")
            return
        if (self.test2["enable"]):
            messagebox.showerror(title="Generator", message="Test 2 is running")
            return
        if self.generator_state:
            messagebox.showerror(title="Generator", message="Generator is already running")
            return



        # List entry widgets
        ents = [
            self.app.tab_panel.tabs.generator.wg["top"]["left"]["ent_io_rms"],
            self.app.tab_panel.tabs.generator.wg["top"]["left"]["ent_freq"],
            self.app.tab_panel.tabs.generator.wg["top"]["left"]["cmb_lut"]
        ]

        # Get parameters
        params = [param.get() for param in ents]
        for param in params:
            if param == "":
                messagebox.showerror(title="Generator", message="Please complete all parameters before proceeding.")
                return
        params = [int(params[0]), int(params[1]), params[2]]

        # Set variable
        self.generator_state = True

        # Disable entries and buttons
        self.app.tab_panel.tabs.generator.enable_widgets(True)

        # Set parameters
        self.scheduler.add_job(self.backend.comm_control_set_signal_params_req, [params[0], params[1]])

        # Set waveform
        waveform = self.app.tab_panel.tabs.generator.wg["top"]["left"]["cmb_lut"].get()
        if waveform not in ["Sine", "Custom"]:
            lut = gen_LUT(waveform, Settings.LUT_SIZE, Settings.LUT_Q)
            self.scheduler.add_job(self.backend.comm_set_custom_lut_req, [lut])
            if waveform == "Sine":
                self.scheduler.add_job(self.backend.comm_use_custom_lut_req, [0])
            else:
                self.scheduler.add_job(self.backend.comm_use_custom_lut_req, [1])
        elif waveform == "Custom":
            lut = self.app.tab_panel.tabs.generator.wg["top"]["left"]["custom_lut"]
            self.scheduler.add_job(self.comm_set_custom_lut_req, [lut])

        # Trend request
        self.scheduler.add_job(self.backend.comm_trend_req, next=True)

    def app_stop_generator(self):
        self.app.tab_panel.tabs.generator.enable_widgets(False)

        # Disable variable
        self.generator_state = False

    def app_set_plot_mode(self, mode):
        self.app.tab_panel.tabs.test1.plot.set_mode(mode)
        self.app.tab_panel.tabs.test2.plot.set_mode(mode)

    def app_set_plot_harmonic_freq(self, harmonic_freq):
        self.app.tab_panel.tabs.test1.plot.set_harmonic_freq(harmonic_freq)
        self.app.tab_panel.tabs.test2.plot.set_harmonic_freq(harmonic_freq)

    def app_set_num_plots(self, enabled_ch):
        self.enabled_channels = enabled_ch
        self.app.tab_panel.tabs.test1.plot.set_channels_num(enabled_ch)
        self.app.tab_panel.tabs.test2.plot.set_channels_num(enabled_ch)

    def app_plot_update(self, tab: str):
        if tab == "Test1":
            self.app.tab_panel.tabs.test1.plot.update_data(self.test1["trends"])
        elif tab == "Test2":
            self.app.tab_panel.tabs.test2.plot.update_data(self.test2["trends"])

    # Backend methods
    def be_set_connection_state(self, enable, attempting_reconnection = False):
        """
        Change connection state, used by scheduler
        """
        if enable:
            self.app.comm_bar.status_lbl.config(text="Connected", background="#06b025")
            if self.control_state:
                self.backend.comm_telemetry_req()
        elif not enable and attempting_reconnection:
            self.app.comm_bar.status_lbl.config(text="Reconnecting",background="#FAFF4D")
        else:
            self.app.comm_bar.status_lbl.config(text="Disconnected",background="#ff4c4c")
            self.app_end_connection(force=True)

    def be_set_control_state(self, control_b):
        """
        Change control state and button.
        """
        if control_b:
            self.app.side_panel.wg["sys_enable"]["btn_control_enable"].configure(text="ON")
            self.control_state = True
        else:
            self.app.side_panel.wg["sys_enable"]["btn_control_enable"].configure(text="OFF")
            self.be_telemetry_handler(0, 0, 0, 0)
            self.control_state = False

    def be_telemetry_handler(self, enable, amp, freq, custom_waveform_b):
        """
        Handles telemetry data received by backend
        """
        if enable:
            self.app.side_panel.wg["telemetry"]["lbl_iopk_display"         ].configure(text=amp)
            self.app.side_panel.wg["telemetry"]["lbl_freq_display"         ].configure(text=freq)
            self.app.side_panel.wg["telemetry"]["lbl_waveform_display"     ].configure(text="Custom" if custom_waveform_b else "Sine")
        else:
            self.app.side_panel.wg["telemetry"]["lbl_iopk_display"         ].configure(text="")
            self.app.side_panel.wg["telemetry"]["lbl_freq_display"         ].configure(text="")
            self.app.side_panel.wg["telemetry"]["lbl_waveform_display"     ].configure(text="")

    def be_test1_handler(self):
        """
        Backend test1 handler.
        """
        if not self.test1["enable"]:
            return

        # Go one by one
        freqs_done = 0
        for freq in self.test1["freq_done"]:
            if self.test1["freq_done"][freq] == "waiting":
                self.scheduler.add_job(self.backend.comm_control_set_signal_params_req, [self.test1["params"]["amp"], freq])
                self.test1["freq_done"][freq] = "working"
                break
            elif self.test1["freq_done"][freq] == "working":
                break
            else:
                freqs_done += 1

        # Update progress bar
        self.app.tab_panel.tabs.test1.progress_update(100 * freqs_done / len(self.test1["freq_done"]))

        # Check if last is done and process
        end_freq = self.test1["params"]["end_freq"]
        if self.test1["freq_done"][end_freq] == "done":
            self.app_plot_update("Test1")
            self.app_stop_test1()
            messagebox.showinfo("Test 1", "Test completed!")
        else:
            self.scheduler.add_job(self.backend.comm_trend_req, next=True)
            self.scheduler.add_job(self.be_test1_handler, next=True)

    def be_test2_handler(self):
        """
        Backend test2 handler.
        """
        if not self.test2["enable"]:
            return

        # Go one by one
        freqs_done = 0
        for freq in self.test2["freq_done"]:
            if self.test2["freq_done"][freq] == "waiting":
                self.scheduler.add_job(self.backend.comm_control_set_signal_params_req, [self.test2["params"]["amp"], freq])
                self.test2["freq_done"][freq] = "working"
                break
            elif self.test2["freq_done"][freq] == "working":
                break
            else:
                freqs_done += 1

        # Update progress bar
        self.app.tab_panel.tabs.test2.progress_update(100 * freqs_done / len(self.test2["freq_done"]))

        # Check if last is done and process
        end_freq = self.test2["params"]["end_freq"]
        if self.test2["freq_done"][end_freq] == "done":
            self.app_plot_update("Test2")
            self.app_stop_test2()
            messagebox.showinfo("Test 2", "Test completed!")
        else:
            self.scheduler.add_job(self.backend.comm_trend_req, next=True)
            self.scheduler.add_job(self.be_test2_handler, next=True)

    def be_trend_handler(self, trend, flags):
        # Log data received
        log_text = "Trend received. Flags: {}, control enabled: {}, amp: {}, freq: {}, custom waveform set: {}.".format(
            trend["flags"], trend["enabled"], trend["amp"], trend["fL"], trend["custom_waveform_b"])

        table = PrettyTable()
        headers_temp = ["Irms", "TWD", "Harmonic DC", "Harmonic 1", "Harmonic 3", "Harmonic 5", "Harmonic 7"]
        aux = ["avg", "max", "min"]
        headers = []
        for h in headers_temp:
            for i in aux:
                headers.append("{} {}".format(h, i))
        table.add_column("Data", headers)
        for i, ch in enumerate(trend["ch"]):
            data = []
            for value in dataclasses.asdict(ch).values():
                if isinstance(value, list):
                    for h in value:
                        data.extend([format(x, ".3f",) for x in h.values()])
                else:
                    data.extend([format(x, ".3f",) for x in value.values()])
            table.add_column("Channel {}".format(i), data)

        log_text += "\n" + table.get_string()
        log.debug('Backend: comm_trend_req() = ' + log_text)

        # Actually handle trend
        if self.generator_state:
            self.app.tab_panel.tabs.generator.write_console(log_text)
            self.scheduler.add_job(self.backend.comm_trend_req, next=True)

        if flags:
            log.warning('Backend: comm_trend_req() = ' + "Trend received with flags " + ", ".join(flags))
        else:
            if ( self.test1["enable"]
                 and trend["fL"] in self.test1["freq_done"]
                 and self.test1["freq_done"][trend["fL"]] != "done"
            ):
                if self.first_trend:
                    self.first_trend = False
                else:
                    self.first_trend = True
                    self.test1["freq_done"][trend["fL"]] = "done"
                    self.test1["trends"][trend["fL"]] = trend

            elif ( self.test2["enable"]
                   and trend["fL"] in self.test2["freq_done"]
                   and self.test2["freq_done"][trend["fL"]] != "done"
            ):
                if self.first_trend:
                    self.first_trend = False
                    log.debug('Backend_api: be_trend_handler() = ' + "Discarding first trend after param change")
                else:
                    self.first_trend = True
                    self.test2["freq_done"][trend["fL"]] = "done"
                    self.test2["trends"][trend["fL"]] = trend

    def _set_null_params(self):
        """
        Sets all params to zero. Used after stopping test or generator.
        """
        self.scheduler.add_job(self.backend.comm_control_set_signal_params_req, [0, 0])

    def _reset_variables(self):
        """
        Sets enabled variables to default
        """
        self.connected_state = False
        self.control_state = False

        self.test1 = {
            "freq_done": {},
            "trends": {},
            "enable": False
        }
        self.test2 = {
            "freq_done": {},
            "trends": {},
            "enable": False
        }
        self.generator_state = False