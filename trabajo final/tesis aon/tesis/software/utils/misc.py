# -------- Module imports --------- #
import math
import tkinter as tk
from tkinter import ttk
import serial.tools.list_ports


# --------- Misc functions --------- #
def gen_LUT(waveform: str, size: int, q: int) -> list:
    """
    Returns LUT in given Q

    Parameters:
        waveform (str): type of LUT, can be sine, triangular,
                        sawtooth or square
        size (int): number of LUT samples, it always uses the
                    full size as one cycle of the signal
        q (int): Q used for fixed-point arithmetics
    Returns:
        LUT in a list
    """
    LUT = []

    # Sinewave
    if (waveform == "Sine"):
        for i in range(size):
            LUT.append(math.sin(2 * math.pi * i / size))

    # Triangular
    elif (waveform == "Triangular"):
        m = 4 / size
        for i in range(size):
            if (i < size / 2):
                LUT.append(-1 + m * i)
            else:
                LUT.append(1 - m * (i - size/2))

    # Diente de sierra
    elif (waveform == "Sawtooth"):
        m = 2 / size
        for i in range(size):
            LUT.append(- 1 + m * i)

    # Cuadrada
    elif (waveform == "Square"):
        middle = round(size/2)
        for i in range (size):
            if i < middle:
                LUT.append(-1)
            else:
                LUT.append(1)

    # unhandled
    else:
        return(1)

    # Apply Q
    LUT_Q = [math.ceil(x * 2**q) for x in LUT]
    return(LUT_Q)

def get_serialports():
    """Lists serial port names

        :returns:
            A list of the serial ports available on the system
    """
    ports=[comport.device for comport in serial.tools.list_ports.comports()]
    ports.sort()
    return(ports)
    # return(["COM3", "COM4"])    # test

def add_widgets(name_list: list,
                master: str,
                tipo: str,
                columna: int = 0,
                fila: int = 0,
                padx: int = 2,
                pady: int = 2,
                text: str = None,
                btn_commands: list = None,
                cmb_list: list = None,
                cmb_state: str = None,
                ent_validate: bool = False,
                ent_vcmd = None,
                orden: str = "columna",
                sticky: str = None,
                dict_to_add: dict = None,
                width = None
            ) -> dict:
    """
    Genera una columna de widgets dentro de un master.

    Parameters:
        name_list: lista con nombre de los widgets, deben ser todos distintos
        master: estructura donde se van a colocar
        tipo: soporta "button, checkbutton, radiobutton, label, combobox"
        columna (opcional): donde se colocan
        fila (opcional): fila de inicio
        padx (opcional): padding en x
        pady (opcional): padding en y
        text (opcional): si el widget admite texto, acá se pone, si no se agrega lista, se deja sin texto
        btn_commands (opcional): lista de comandos para tipo "button"
        cmb_list (opcional): lista de listas con opciones para tipo "combobox"
        ent_validate (opcional): validate para entrys
        ent_vcmd (opcional): funcion de callback para validate
        orden (opcional): tipo de orden
        sticky (opcional): alineacion de widget
        dict_to_add (opcional): dictionary al cual los widgets se van a agregar
    Returns:
        dictionary: widgets agregados si no hay dict_to_add. Sino, no returns nada.
    """

    # Check if there's a dictionary given to add widgets to
    if dict_to_add:
        return_dict = dict_to_add
    else:
        return_dict = {}

    # Check if there's text or not
    if not text:
        text=[""] * len(name_list)

    # Switch for each type
    for (i,name) in enumerate(name_list):
        if tipo == "label":
                w = ttk.Label(master=master, text=text[i], width=width)
        elif tipo == "button":
            if btn_commands:
                w = ttk.Button(master=master, text=text[i], command=btn_commands[i], width=width)
            else:
                w = ttk.Button(master=master, text=text[i], width=width)
        elif tipo == "checkbutton":
            if btn_commands:
                w = ttk.Checkbutton(master=master, text=text[i], command=btn_commands[i], width=width)
            else:
                w = ttk.Checkbutton(master=master, text=text[i], width=width)
        elif tipo == "radiobutton":
            if btn_commands:
                w = ttk.Radiobutton(master=master, text=text[i], command=btn_commands[i], width=width)
            else:
                w = ttk.Radiobutton(master=master, text=text[i], width=width)
        elif tipo == "combobox":
                w = ttk.Combobox(master=master, values=cmb_list[i], state=cmb_state, width=width)
        elif tipo == "entry":
            if ent_validate:
                w = ttk.Entry(master=master, validate='all', validatecommand=(ent_vcmd, '%P'), width=width)
            else:
                w = ttk.Entry(master=master, width=width)
        else:
            print("add_widgetsError - Error en la selección de tipo")

        if orden == "columna":
            w.grid(row=i+fila, column=columna, padx=padx, pady=pady)
        elif orden == "fila":
            w.grid(row=fila, column=columna+i, padx=padx, pady=pady)
        else:
            print("Seleccionar orden columna o fila")

        if sticky != None:
            w.grid(sticky=sticky)

        return_dict[name] = w

    if not dict_to_add:
        return return_dict

def print_msg(header: str, msg: bytearray): #, bytes_per_line: int):
    """
    Returns message in a nice format.

    Parameters:
        header: name to show before the message, e.g. "TX: "
        msg: message to show
        bytes_per_line (deprecated): number of bytes to show per line
    """
    #tab = ''.ljust(len(header))                     # String of blanks.

    #if (bytes_per_line != 0):
    msg_nice = header
    show_counter = 0                      # counter used to present data for line
    for g in range(len(msg)):
        msg_nice += format(msg[g], '02x') + ' '
        show_counter += 1
        # if (show_counter > bytes_per_line):
            #     msg_nice += "\n"
            #     msg_nice += tab
            #     show_counter = 0
    return(msg_nice)

def validate_number(input):
    """
    Returns true if input is digit or empty
    """
    if str.isdigit(input) or input == "":
        return True
    else:
        return False

if __name__ == "__main__":
    lut = gen_LUT("Square", 128, 20)
    print(lut)