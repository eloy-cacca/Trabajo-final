# ------ Module imports ------ #
import tkinter as tk
from tkinter import ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg, NavigationToolbar2Tk
from matplotlib.figure import Figure
import matplotlib.pyplot as plt
import matplotlib.gridspec as gridspec
import math
import logging as log
import csv

# ------- Local imports -------- #
from utils.settings import Settings

plt.style.use("ggplot")


class PlotPanel(ttk.Frame):
    def __init__(self, master, backend_api):
        super().__init__(master)
        self.data = {}
        self.wg = {}
        self.backend_api = backend_api
        self.harmonic_freq = []
        self.prev_harmonic_freq = []
        self.mode = "rms"
        self.build_plot()

        log.getLogger('matplotlib.font_manager').disabled = True

        #debug only
        """import numpy as np
        self.data["freq"]    = [ [50 + i * 50 for i in range(5)]  for _ in range(4)]
        self.data["rms_avg"] = [ np.random.uniform(2, 8, 5)  for _ in range(4)]
        self.data["dB_avg"] = [ np.random.uniform(2, 8, 5)  for _ in range(4)]
        self.data["twd_avg"] = [ np.random.uniform(2, 80, 5) for _ in range(4)]
        self.data["harmonic_avg"] = [ [np.random.uniform(2, 80, 5) for _ in range(5)] for _ in range(4)]
        self._update_plot()"""

    def build_plot(self):
        """
        First plot
        """
        self.wg["fig"] = Figure(figsize=(5,4))
        self.wg["ax"] = []

        self.wg["fig"].set_tight_layout(True)
        self.wg["fig"].subplots_adjust(left=0.1, right=0.98, top=0.95, bottom=0.1)
        self.wg["canvas"] = FigureCanvasTkAgg(self.wg["fig"], master=self)
        self.wg["canvas"].get_tk_widget().pack(expand=1, fill="both")

        self.set_channels_num(self.backend_api.enabled_channels)

    def update_data(self, trends: dict):
        """
        Updates plot with new data
        """
        self.data = {}
        self.data["freq"]          = [ list(trends.keys())                                                                            for _ in range(Settings.CH_TOTAL_NUM) ]
        self.data["rms_avg"]       = [ [trends[f]["ch"][i].rms.avg                                                  for f in trends]  for i in range(Settings.CH_TOTAL_NUM) ]
        self.data["rms_max"]       = [ [trends[f]["ch"][i].rms.max                                                  for f in trends]  for i in range(Settings.CH_TOTAL_NUM) ]
        self.data["rms_min"]       = [ [trends[f]["ch"][i].rms.min                                                  for f in trends]  for i in range(Settings.CH_TOTAL_NUM) ]
        self.data["twd_avg"]       = [ [trends[f]["ch"][i].twd.avg                                                  for f in trends]  for i in range(Settings.CH_TOTAL_NUM) ]
        self.data["twd_max"]       = [ [trends[f]["ch"][i].twd.max                                                  for f in trends]  for i in range(Settings.CH_TOTAL_NUM) ]
        self.data["twd_min"]       = [ [trends[f]["ch"][i].twd.min                                                  for f in trends]  for i in range(Settings.CH_TOTAL_NUM) ]
        self.data["harmonic_avg"]  = [ [[trends[f]["ch"][i].harmonic[h].avg for h in range(Settings.NDFT_MAX + 1)]  for f in trends]  for i in range(Settings.CH_TOTAL_NUM) ]
        self.data["harmonic_max"]  = [ [[trends[f]["ch"][i].harmonic[h].max for h in range(Settings.NDFT_MAX + 1)]  for f in trends]  for i in range(Settings.CH_TOTAL_NUM) ]
        self.data["harmonic_min"]  = [ [[trends[f]["ch"][i].harmonic[h].min for h in range(Settings.NDFT_MAX + 1)]  for f in trends]  for i in range(Settings.CH_TOTAL_NUM) ]
        self.data["dB_avg"]        = [ [20 * math.log10(trends[f]["ch"][i].rms.avg / trends[f]["amp"])              for f in trends]  for i in range(Settings.CH_TOTAL_NUM) ]
        self.data["dB_max"]        = [ [20 * math.log10(trends[f]["ch"][i].rms.max / trends[f]["amp"])              for f in trends]  for i in range(Settings.CH_TOTAL_NUM) ]
        self.data["dB_min"]        = [ [20 * math.log10(trends[f]["ch"][i].rms.min / trends[f]["amp"])              for f in trends]  for i in range(Settings.CH_TOTAL_NUM) ]
        self._update_plot()

    def set_channels_num(self, enabled_ch: list):
        """
        Updates number of channels present
        """
        self.enabled_ch = enabled_ch
        self._update_plot()

    def set_mode(self, mode: str):
        """
        Sets plot mode, possible values are rms, bode or harmonic.
        """
        self.mode = mode
        self._update_plot()

    def set_harmonic_freq(self, harmonic_freq: list):
        """
        Sets harmonic freq to show in harmonic mode
        """
        if self.data:
            temp = harmonic_freq.copy()
            temp2 = []

            for i, h in enumerate(temp):
                if h in self.data["freq"][0]:
                    temp2.append(h)

            if temp2 != self.harmonic_freq:
                self.harmonic_freq = temp2.copy()
                self._update_plot()

    def save_png(self):
        """
        Saves image in PNG format
        """
        filename = tk.filedialog.asksaveasfilename(
            defaultextension=".png",
            filetypes = [("PNG", ".png")]
        )
        if filename != "" :
            self.wg["fig"].savefig(filename)
            tk.messagebox.showinfo('Info', 'Plot saved!')

    def save_csv(self):
        """
        Saves data in CSV format
        """
        if not self.data:
            tk.messagebox.showwarning("Info", "No data found, run the test first.")
            return

        filename = tk.filedialog.asksaveasfilename(
            defaultextension=".csv",
            filetypes = [("CSV", ".csv")]
        )

        # Make headers
        header = ["Frequencies", "Channel"]
        header_temp = ["Irms", "TWD", "Harmonic DC", "Harmonic 1", "Harmonic 3", "Harmonic 5", "Harmonic 7"]
        aux = ["avg", "max", "min"]
        for h in header_temp:
            for i in aux:
                header.append("{} {}".format(h, i))

        # Map data
        row = []
        for f, freq in enumerate(self.data["freq"][0]):
            for ch in range(Settings.CH_TOTAL_NUM):
                temp = []
                temp.extend([freq, ch + 1])
                temp.extend([
                    self.data["rms_avg"][ch][f],
                    self.data["rms_max"][ch][f],
                    self.data["rms_min"][ch][f],
                    self.data["twd_avg"][ch][f],
                    self.data["twd_max"][ch][f],
                    self.data["twd_min"][ch][f]
                ])
                for h in range(Settings.NDFT_MAX + 1):
                    temp.append(self.data["harmonic_avg"][ch][f][h])
                    temp.append(self.data["harmonic_max"][ch][f][h])
                    temp.append(self.data["harmonic_min"][ch][f][h])
                row.append(temp)

        if filename != "" :
            with open(filename, "w") as f:
                writer = csv.writer(f)
                writer.writerow(header)
                [writer.writerow(r) for r in row]
            tk.messagebox.showinfo('Info', 'Plot saved!')

    def _clean(self):
        """
        Clean current plot
        """
        if self.wg["artists"]:
            for artist in self.wg["artists"]:
                artist.remove()
        self.wg["artists"].clear()

    def _redraw(self):
        """
        This functions exists only because I can't call canvas.draw()
        from another thread as it crashes.
        (see https://github.com/matplotlib/matplotlib/issues/13293)
        """
        self.after(1, self.wg["canvas"].draw)

    def _replot(self):
        """
        Refreshes plot because of issues
        """
        self.wg["fig"].clear()
        self.wg["ax"] = []
        self.wg["ax_twd"] = []
        self.wg["artists"] = []

        enabled_channels = 0
        for state in self.enabled_ch: enabled_channels += state
        cur_channel = 0
        grid = gridspec.GridSpec(2, 2)

        for ch_num, state in enumerate(self.enabled_ch):
            if state:
                if enabled_channels == 1:
                    ax = self.wg["fig"].add_subplot(grid[:, :])
                elif enabled_channels == 2:
                    ax = self.wg["fig"].add_subplot(grid[cur_channel, :])
                elif enabled_channels == 3:
                    if cur_channel < 2:
                        ax = self.wg["fig"].add_subplot(grid[0, cur_channel])
                    else:
                        ax = self.wg["fig"].add_subplot(grid[1, :])
                else:
                    ax = self.wg["fig"].add_subplot(220 + cur_channel + 1)

                ax_twd = ax.twinx()
                ax_twd.grid(False)

                ax.set_title("Channel {}".format(ch_num + 1))
                self.wg["ax"].append(ax)
                self.wg["ax_twd"].append(ax_twd)
                cur_channel += 1

    def _update_plot(self):
        """
        Update plot. Includes everything needed for updating the plot
        """
        if not self.data:
            self._replot()
            self._redraw()
            return

        self._clean()
        self._replot()

        if (self.mode == "rms"):
            i = 0
            for j, ch in enumerate(self.backend_api.enabled_channels):
                if ch:
                    self.wg["artists"].append(self.wg["ax"][i].plot    (self.data["freq"][j], self.data["rms_avg"][j], marker=".", color="tab:blue")[0])
                    self.wg["artists"].append(self.wg["ax_twd"][i].bar (self.data["freq"][j], self.data["twd_avg"][j], color="tab:red", align="center", width=3, alpha=0.6))

                    self.wg["ax"][i]     .set_xlim(self.data["freq"][j][0] - 10, self.data["freq"][j][-1] + 10)
                    self.wg["ax_twd"][i] .set_xlim(self.data["freq"][j][0] - 10, self.data["freq"][j][-1] + 10)
                    self.wg["ax_twd"][i].set_ylim(0, 100)

                    self.wg["ax"][i].set_ylabel("Irms [A]", fontsize=9)
                    self.wg["ax"][i].set_xlabel("Frequency [Hz]", fontsize=9)
                    self.wg["ax_twd"][i].set_ylabel("TWD [%]", fontsize=9)
                    i += 1

        elif (self.mode == "bode"):
            i = 0
            for j, ch in enumerate(self.backend_api.enabled_channels):
                if ch:
                    self.wg["artists"].append(self.wg["ax"][i].plot    (self.data["freq"][j], self.data["dB_avg"][j], marker=".", color="tab:blue")[0])
                    self.wg["artists"].append(self.wg["ax_twd"][i].bar (self.data["freq"][j], self.data["twd_avg"][j], color="tab:red", align="center", width=3, alpha=0.6))

                    self.wg["ax"][i]     .set_xlim(self.data["freq"][j][0] - 10, self.data["freq"][j][-1] + 10)
                    self.wg["ax_twd"][i] .set_xlim(self.data["freq"][j][0] - 10, self.data["freq"][j][-1] + 10)
                    self.wg["ax_twd"][i] .set_ylim(0, 100)

                    self.wg["ax"][i].set_ylabel("Transfer amplitude [dB]", fontsize=9)
                    self.wg["ax"][i].set_xlabel("Frequency [Hz]", fontsize=9)
                    self.wg["ax_twd"][i].set_ylabel("TWD [%]", fontsize=9)
                    i += 1

        elif (self.mode == "harmonic"):
            i = 0
            for j, ch in enumerate(self.backend_api.enabled_channels):
                if ch:
                    for k in range(len(self.harmonic_freq)):
                        if k == 0:
                            alpha = 1
                            self.wg["ax"][i].set_prop_cycle(None)
                        else:
                            alpha = .95
                        self.wg["artists"].append(self.wg["ax"][i].bar(["DC", "1", "3", "5", "7"], self.data["harmonic_avg"][j][k], align="center", width=.8-.2*k, alpha=alpha))

                    self.wg["ax"][i].set_ylabel("Ipk [A]", fontsize=9)
                    self.wg["ax"][i].set_xlabel("Harmonic", fontsize=9)
                    self.wg["ax_twd"][i].set_axis_off()
                    i += 1

        self._redraw()
