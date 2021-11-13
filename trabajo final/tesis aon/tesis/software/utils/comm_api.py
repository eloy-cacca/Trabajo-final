# -------- Module imports ------- #
from dataclasses import dataclass


@dataclass
class CommApi:
    # ARM - Firmware
    FW_ARM_VERSION_REQ:                   str = "02"
    FW_ARM_VERSION_RES:                   str = "03"
    FW_ARM_ECHO_REQ:                      str = "04"
    FW_ARM_ECHO_RES:                      str = "05"
    FW_ARM_UNKNOW_MSG_ID_RES:             str = "0F"

    # ARM - Control
    CONTROL_START_REQ:                    str = "10"
    CONTROL_START_RES:                    str = "11"
    CONTROL_STOP_REQ:                     str = "12"
    CONTROL_STOP_RES:                     str = "13"
    CONTROL_SET_SIGNAL_PARAMS_REQ:        str = "14"
    CONTROL_SET_SIGNAL_PARAMS_RES:        str = "15"
    CONTROL_TREND_REQ:                    str = "16"
    CONTROL_TREND_RES:                    str = "17"
    CONTROL_SET_CUSTOM_LUT_REQ:           str = "18"
    CONTROL_SET_CUSTOM_LUT_RES:           str = "19"
    CONTROL_USE_CUSTOM_LUT_REQ:           str = "1A"
    CONTROL_USE_CUSTOM_LUT_RES:           str = "1B"
    CONTROL_TELEMETRY_REQ:                str = "1C"
    CONTROL_TELEMETRY_RES:                str = "1D"
    CONTROL_SET_PWM_DUTY_REQ:             str = "1E"
    CONTROL_SET_PWM_DUTY_RES:             str = "1F"
