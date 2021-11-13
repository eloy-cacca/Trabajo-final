# -------- Module imports ------- #
import logging as log
import serial
from tkinter import messagebox


class Comm:
    baudrate = 115200
    timeout = 3

    def __init__(self):
        self.ser = serial.Serial()
        self.ser.baudrate = self.baudrate
        self.ser.timeout = self.timeout
        self.ser.write_timeout = self.timeout

    def selport(self, port):
        self.ser.port = port
        log.info("Comm: selport() = port " + port + " selected.")

    def open(self):
        try:
            self.ser.open()
            log.info("Comm: open() = port opened correctly")
        except Exception as inst:
            log.error("Comm: open() exception = " + type(inst).__name__ + " - " + str(inst.args[0:]))
            messagebox.showerror("Comm error", "Can't open port.")

    def close(self):
        try:
            self.ser.reset_input_buffer()
            self.ser.reset_output_buffer()
            self.ser.close()
            log.info("Comm: close() = port closed correctly")

        except Exception as inst:
            log.error("Comm: close() exception = " + type(inst).__name__ + " - " + str(inst.args[0:]))
            messagebox.showerror("Comm error", "Can't close port.")
            raise

    def is_open(self):
        return self.ser.is_open

    def read(self) -> bytes:
        """read response"""
        try:
            data = self.ser.read()
        except Exception as inst:
            log.error("Comm: read() exception = " + type(inst).__name__ + " - " + str(inst.args[0:]))
            raise
        else:
            if not data:
                log.error("Comm: read() exception = " + "No data received.")
                raise ValueError("No data received")
            else:
                return data

    def write(self, data):
        """write command"""
        try:
            self.ser.write(data)
        except Exception as inst:
            log.error("Comm: write() exception = " + type(inst).__name__ + " - " + str(inst.args[0:]))
            raise
