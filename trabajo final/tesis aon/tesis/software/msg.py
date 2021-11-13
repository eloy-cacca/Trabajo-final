# -------- Module imports ------- #
import logging as log
from math import floor

# ------- Local imports -------- #
from utils.settings import Settings
from utils.misc import print_msg

raw_data_logging_b = [0]


class Msg:
    """
    Message Class helps create a message packed to be
    understood by the uC. It also talks to the Comm Class
    to handle serial communication.

    Args:
        msg (bytes): mensaje que se quiere enviar
        msg_id (str): tipo de mensaje en hexadecimal
    """
    def __init__(self, msg: bytes, msg_id: str):
        self.msg = msg
        self.msg_id = msg_id
        self.dest_id = Settings.MSG_ARM_ID

    def send(self, comm_manager):
        """
        Sends the message
        """
        if len(self.msg) == 0:
            log.error("Msg: send() exception = " + "Empty message")
            return

        # Pack message
        self._pack()

        # Write message
        try:
            comm_manager.write(self.msg_packed)
            if (raw_data_logging_b[0]): log.debug("Msg: send() = " + print_msg("TX: ", self.msg_packed))
        except Exception as inst:
            log.error("Msg: send() exception = " + type(inst).__name__ + " - " + str(inst.args[0:]))
            raise

        # Receive message
        try:
            self.response_packed = self._wait(comm_manager)
            self.response = self.response_packed[Settings.MSG_HEADER_1ST_BYTE:-2]
        except:
            raise

    def _pack(self):
        """
        Packs message
        """
        # Set header
        header = bytearray.fromhex(Settings.MSG_START_BYTE + Settings.MSG_PROJECT_ID + Settings.MSG_RPI_ID + self.dest_id + self.msg_id)

        # Pack message
        self.msg_packed = header + bytes([len(self.msg)]) + self.msg
        self._crc_add(self.msg_packed)

    def _crc_calculate(self, data_to_calc_crc):
        """
        Add 16-bit checksum to data_to_calc_crc.
        If done for a packet with checksum appended, it should return 0.
        """
        CRCPoly = int("A001", 16)
        result = 0
        for i in range(len(data_to_calc_crc)):
            data = data_to_calc_crc[i]
            for _ in range(8):
                result_bit = result & 1
                result = floor(result/2)
                data_bit = data & 1
                data = floor(data/2)
                if (result_bit != data_bit):
                    result = result ^ CRCPoly
        return(result)

    def _crc_add(self, data_to_add_crc):
        """Adds crc at end of data_to_add_crc"""
        result = self._crc_calculate(data_to_add_crc)
        crc = result.to_bytes(2, byteorder="little")
        data_to_add_crc.extend(crc)

    def _wait(self, comm_manager) -> bytes:
        """
        Reads response, checking every byte and discarding if necessary

        Returns:
            response_packed: respuesta en bytes
        """
        rx_count = 0
        msg = []
        while (True):
            data = comm_manager.read()
            msg.extend(data)
            rx_count += 1

            # Check proper start byte
            if (rx_count > 0) and (msg[Settings.MSG_HEADER_START] != int(Settings.MSG_START_BYTE, 16)):
                log.error("Msg: send() exception = " + "Wrong start byte received. " + print_msg("RX: ", msg))
                raise IOError("Wrong start byte received")

            # Check proper project id
            if (rx_count > 1) and (msg[Settings.MSG_HEADER_PORJ_ID] != int(Settings.MSG_PROJECT_ID, 16)):
                log.error("Msg: send() exception = " + "Wrong project id received. " + print_msg("RX: ", msg))
                raise IOError("Wrong project id received")

            # Check proper source id
            if (rx_count > 2) and (msg[Settings.MSG_HEADER_SRC_ID]) != int(Settings.MSG_ARM_ID, 16):
                log.error("Msg: send() exception = " + "Wrong source id received. " + print_msg("RX: ", msg))
                raise IOError("Wrong source id received")

            # Check proper destination id
            if (rx_count > 3) and ((msg[Settings.MSG_HEADER_DES_ID] != int(Settings.MSG_RPI_ID, 16))):
                log.error("Msg: send() exception = " + "Wrong destination id received. " + print_msg("RX: ", msg))
                raise IOError("Wrong destination id received")

            # if the packet is complete, check CRC and continue
            if (rx_count >= Settings.MSG_LENGTH_HEADER) and ((msg[Settings.MSG_HEADER_PAYLOAD_LEN] + Settings.MSG_LENGTH_HEADER) == rx_count):
                if self._crc_calculate(msg) != 0:
                    log.error("Msg: send() exception = " + "Wrong CRC received. " + print_msg("RX: ", msg))
                    raise ValueError("Wrong CRC received")
                else:
                    break

        if raw_data_logging_b[0]: log.debug("Msg: send() = " + print_msg("RX: ", msg))
        return(bytes(msg))

    @classmethod
    def from_hex(cls, msg_hex: str, msg_id: str):
        """Generates message from hex"""
        msg = bytes.fromhex(msg_hex)
        return cls(msg, msg_id)

    @classmethod
    def from_int(cls, msg_int: int, msg_id: str):
        """Generates message from int"""
        msg = (msg_int).to_bytes(1, "little", signed=False)
        return cls(msg, msg_id)

    @classmethod
    def from_int_list(cls, msg_int_list: int, length: int, byteorder: str,
                      signed: bool, msg_id: str):
        """
        Makes message from list of int

        Parameters:
            msg_int_list: int list
            length: amount of bytes to use per int
            byteorder: "little" o "big" endian
            signed: True or False
        """
        msg = bytes()
        for num in msg_int_list:
            msg += (num).to_bytes(length, byteorder, signed=signed)
        return cls(msg, msg_id)


if __name__ == "__main__":
    from utils import get_serialports
    from utils import print_msg
    from comm import comm

    log.getLogger().setLevel(log.DEBUG)
    raw_data_logging_b[0] = 1

    while True:
        print("1) Elegir puerto")
        print("2) Abrir puerto")
        print("3) Cerrar puerto")
        print("4) Enviar mensaje - manual")
        print("5) Enviar mensaje - AUTO")
        print("6) Recibir datos")
        print("0) Salir")
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
            msg_id = input("Elegir msg_id: ")
            msg_content = input("Mensaje a enviar en hexa: ")
            msg = Msg.from_hex(msg_content, msg_id)
            msg.send(comm)

        if M == 5:
            print("a) Enviar echo")
            print("b) Enviar version_req")
            M4 = input("Elegir opción: ")

            if M4 == "a":
                msg = Msg.from_hex(msg_hex="16", msg_id="04")
                msg.send(comm)
            if M4 == "b":
                msg = Msg.from_hex(msg_hex="16", msg_id="02")
                msg.send(comm)
                print(msg.response)
                resp = msg.response.decode("utf-8")
                print(resp)

        if M == 6:
            import struct
            print("a) float")
            tipo = input("Elegir opción: ")

            if tipo == "a":
                while True:
                    bytes_to_read = comm.ser.inWaiting()
                    rx = comm.ser.read(bytes_to_read)
                    if len(rx) > 0:
                        result = struct.unpack("f"* int((len(rx)/4)), rx)
                        print(result)

        if M == 0:
            break