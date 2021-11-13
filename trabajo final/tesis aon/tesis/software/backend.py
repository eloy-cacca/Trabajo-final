# -------- Module imports ------- #

import logging as log
import math
from tkinter import messagebox
import struct


# ------- Local imports -------- #
import comm
from msg import Msg
from utils.comm_api import CommApi
from utils.comm_flags import CommFlags
from utils.settings import Settings
from utils.trend import *


class Backend():
    """
    Handles communication with the ARM microcontroller.

    It's worth mentioning that the Atmel SAM4S used in this project has the following constraints:
      - little-endian
      - aligned at 16-bit
    """
    def __init__(self, backend_api, comm: comm.Comm):
        self.backend_api = backend_api
        self.comm = comm

    # ARM - Firmware
    def comm_echo_req(self):
        """
        Echo request.
        """
        status = 0

        log.debug("Backend: comm_echo_req() = " + "Sending echo.")
        msg = Msg.from_hex(msg_hex="23", msg_id=CommApi.FW_ARM_ECHO_REQ)

        try:
            msg.send(self.comm)
        except:
            raise
        else:
            if msg.response == msg.msg:
                log.debug("Backend: comm_echo_req() = " + "Echo received.")
            else:
                log.error("Backend: comm_echo_req() = " + "Echo not received.")
                raise IOError("Echo not received.")

    def comm_telemetry_req(self):
        """
        VFP request telemetry to uC.
        """
        log.debug("Backend: comm_telemetry_req() = " + "Requesting telemetry.")
        msg = Msg.from_hex(msg_hex="23", msg_id=CommApi.CONTROL_TELEMETRY_REQ)
        msg.send(self.comm)

        (enable, amp, freq, custom_waveform_b) = struct.unpack("<BxHHBx", msg.response)
        log.debug("Backend: comm_telemetry_req() = " + "Telemetry received. Amp: {}, freq: {}, control enabled: {}, custom LUT set: {}".format(amp, freq, enable, custom_waveform_b))
        self.backend_api.be_telemetry_handler(enable, amp, freq, custom_waveform_b)

    def comm_version_req(self):
        """
        VFP request version to uC.

        Returns:
            fw_version: version number in str
            fw_date: build date in str
        """
        log.debug("Backend: comm_version_req() = " + "Requesting firmware version.")
        msg = Msg.from_hex(msg_hex="23", msg_id=CommApi.FW_ARM_VERSION_REQ)
        msg.send(self.comm)

        # Get version and date separately
        fw_version = str(msg.response[0]) + "." + str(msg.response[1])
        fw_date = msg.response[2:].decode('utf-8')

        # Return tuple
        log.debug("Backend: comm_version_req() = " + "Firmware version received. Version: {}, date: {}".format(fw_version, fw_date))
        return (fw_version, fw_date)

    # ARM - Control
    def comm_control_start_req(self):
        """
        VFP request to start uC Control.
        """
        log.debug("Backend: comm_control_start_req() = " + "Requesting control start.")
        msg = Msg.from_hex(msg_hex="23", msg_id=CommApi.CONTROL_START_REQ)
        msg.send(self.comm)

        # Handle errors
        # for now, it returns echo so...

        if msg.response == msg.msg:
            log.debug("Backend: comm_control_start_req() = " + "Control started.")
            if __name__ != "__main__": self.backend_api.be_set_control_state(True)
        else:
            messagebox.showerror("Backend", "Failed to start control.")
            log.error("Backend: comm_control_start_req() = " + "Failed to start control.")
            if __name__ != "__main__": self.backend_api.be_set_control_state(False)

    def comm_control_stop_req(self):
        """
        VFP request to stop uC Control.
        """
        log.debug("Backend: comm_control_stop_req() = " + "Requesting control stop.")
        msg = Msg.from_hex(msg_hex="23", msg_id=CommApi.CONTROL_STOP_REQ)
        msg.send(self.comm)

        if msg.response == msg.msg:
            log.debug("Backend: comm_control_stop_req() = " + "Control stopped.")
            if __name__ != "__main__": self.backend_api.be_set_control_state(False)
        else:
            messagebox.showerror("Backend", "Failed to stop control.")
            log.error("Backend: comm_control_stop_req() = " + "Failed to stop control.")
            if __name__ != "__main__": self.backend_api.be_set_control_state(True)

    def comm_control_set_signal_params_req(self, amp: int, freq: int):
        """
        VFP request to set signal parameters.

        Parameters:
            amp: amplitude in RMS (if default LUT, else Vp)
            freq: frequency in Hz
        """
        log.debug("Backend: comm_control_set_signal_params_req() = " + "Setting signal params. Amp: {}, freq: {}.".format(amp, freq))
        msg_bytes = bytes()
        msg_bytes += (amp).to_bytes(2, "little", signed=False)
        msg_bytes += (freq).to_bytes(2, "little", signed=False)

        msg = Msg(msg=msg_bytes, msg_id=CommApi.CONTROL_SET_SIGNAL_PARAMS_REQ)
        msg.send(self.comm)

        # Handle errors
        if msg.response == bytes([0]):
            log.debug('backend: comm_control_set_signal_params_req() = ' + "Request received.")
        elif msg.response == bytes([1]):
            log.error('backend: comm_control_set_signal_params_req() = ' + "Wrong number of params.")
        else:
            log.error('backend: comm_control_set_signal_params_req() = ' + "Unregistered error.")

    def comm_set_custom_lut_req(self, lut: list):
        """
        VFP request to set new LUT.
        """
        bytes_per_sample = 4
        #FIXME: no anda

        log.debug("Backend: comm_set_custom_lut_req() = " + "Setting custom LUT.")
        msg = Msg.from_int_list(lut, bytes_per_sample, "little", True, msg_id=CommApi.CONTROL_SET_CUSTOM_LUT_REQ)
        msg.send(self.comm)

    def comm_use_custom_lut_req(self, use_custom: int):
        """
        VFP request to use custom LUT or not.
        """
        if use_custom not in [0, 1]:
            log.error("Backend: comm_set_custom_lut_req() = " + "Wrong use of custom LUT, set to {}.".format(use_custom))
            return
        else:
            log.debug("Backend: comm_set_custom_lut_req() = " + "Set use of custom LUT to {}.".format(use_custom))
            msg = Msg.from_int(msg_int=use_custom, msg_id=CommApi.CONTROL_USE_CUSTOM_LUT_REQ)
            msg.send(self.comm)

    def comm_trend_req(self):
        """
        VFP trend request.
        """
        log.debug("Backend: comm_trend_req() = " + "Sending trend request.")
        msg = Msg.from_hex(msg_hex="00", msg_id=CommApi.CONTROL_TREND_REQ)
        msg.send(self.comm)

        try:
            flags_raw = msg.response
            params_raw = msg._wait(self.comm)[Settings.MSG_HEADER_1ST_BYTE:-2]
            ch_raw = []
            for _ in range(Settings.CH_TOTAL_NUM):
                ch_raw.append(msg._wait(self.comm)[Settings.MSG_HEADER_1ST_BYTE:-2])
        except:
            log.error("Backend: comm_trend_req() = " + "Trend request not received.")
            return


        flags = struct.unpack("<B", flags_raw)
        params = struct.unpack("<BxHHBx", params_raw)
        ch = []
        ch_bytes_format = "<" + 3 * "i" + 3 * "h" + 2 * "x" + (Settings.NDFT_MAX + 1) * 2 * 3 * "i"
        for data in ch_raw:
            ch.append(struct.unpack(ch_bytes_format, data))

        trend = {}
        trend["flags"]             = flags[0]
        trend["enabled"]           = params[0]
        trend["amp"]               = params[1]
        trend["fL"]                = params[2]
        trend["custom_waveform_b"] = params[3]
        trend["ch"]                = []

        for i in range(Settings.CH_TOTAL_NUM):
            harmonics_temp = []
            for j in range(Settings.NDFT_MAX + 1):
                harmonics_temp.append(
                    TrendData(
                        avg = math.hypot(ch[i][6 + j*6] / 2**Settings.HARMONIC_Q, ch[i][9  + j*6] / 2**Settings.HARMONIC_Q),
                        max = math.hypot(ch[i][7 + j*6] / 2**Settings.HARMONIC_Q, ch[i][10 + j*6] / 2**Settings.HARMONIC_Q),
                        min = math.hypot(ch[i][8 + j*6] / 2**Settings.HARMONIC_Q, ch[i][11 + j*6] / 2**Settings.HARMONIC_Q)
                    )
                )
            trend["ch"].append(
                Trend(
                    rms = TrendData(
                        avg = ch[i][0] / 2**Settings.RMS_Q,
                        max = ch[i][1] / 2**Settings.RMS_Q,
                        min = ch[i][2] / 2**Settings.RMS_Q
                    ),
                    twd = TrendData(
                        avg = ch[i][3] / 2**Settings.TWD_Q * 100,
                        max = ch[i][4] / 2**Settings.TWD_Q * 100,
                        min = ch[i][5] / 2**Settings.TWD_Q * 100
                    ),
                    harmonic = harmonics_temp
                )
            )

        # Check if any flag else re run trend_req
        flags_str = CommFlags.check(trend["flags"])
        self.backend_api.be_trend_handler(trend, flags_str)

if __name__ == "__main__":
    from utils.misc import get_serialports

    log.getLogger().setLevel(log.DEBUG)
    comm = comm.Comm()
    backend = Backend(None, comm)

    while True:
        print("1) Elegir puerto")
        print("2) Abrir puerto")
        print("3) Cerrar puerto")
        print("4) Enviar trend request")
        print("5) Setear params")
        print("6) Setear PWM duty")
        print("7) Habilitar control")
        print("8) Deshabilitar control")

        M = int(input("Elegir opción: "))

        if M == 1:
            ports = get_serialports()
            print(ports)
            port_sel = input("Elegir puerto: ")
            port_sel = ports[int(port_sel)]
            comm.selport(port_sel)
            print()

        if M == 2:
            print("Abriendo puerto...\n")
            comm.open()

        if M == 3:
            print("Cerrando puerto...\n")
            comm.close()

        if M == 4:
            print("Enviando trend request...\n")
            backend.comm_trend_req()

        if M == 5:
            amp = int(input("Amp: "))
            freq = int(input("Freq: "))
            backend.comm_control_set_signal_params_req(amp, freq)

        if M == 6:
            duty = int(input("Duty (0-100): "))
            msg = Msg.from_int(msg_int=duty, msg_id=CommApi.CONTROL_SET_PWM_DUTY_REQ)
            msg.send(comm)
            response = msg.msg
            print(f"Respuesta: {response}")

        if M == 7:
            backend.comm_control_start_req()

        if M == 8:
            backend.comm_control_stop_req()

        if M == 0:
            backend.comm_control_stop_req()
            break
