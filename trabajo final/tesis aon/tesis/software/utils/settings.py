# -------- Module imports ------- #
from dataclasses import dataclass


@dataclass
class Settings:
    # Message ID
    MSG_PROJECT_ID:             str = '04'
    MSG_DSP_ID:                 str = '33'
    MSG_ARM_ID:                 str = '77'
    MSG_RPI_ID:                 str = 'AA'
    MSG_OTHERS_ID:              str = 'EE'

    # Message format
    MSG_START_BYTE:             str = '3C'
    MSG_LENGTH_HEADER:          int = 8
    MSG_LENGTH_PAYLOAD:         int = 128
    MSG_LENGTH_MAX_SIZE:        int = MSG_LENGTH_HEADER + MSG_LENGTH_PAYLOAD

    # Message header
    MSG_HEADER_START:           int = 0
    MSG_HEADER_PORJ_ID:         int = 1
    MSG_HEADER_SRC_ID:          int = 2
    MSG_HEADER_DES_ID:          int = 3
    MSG_HEADER_MSG_ID:          int = 4
    MSG_HEADER_PAYLOAD_LEN:     int = 5
    MSG_HEADER_1ST_BYTE:        int = 6

    # LUT
    LUT_SIZE:                   int = 128
    LUT_Q:                      int = 20

    # Channels
    CH_TOTAL_NUM:               int = 4
    NDFT_MAX:                   int = 4     # harmonics number without DC

    # Trends fixed point Q
    RMS_Q:                      int = 12
    TWD_Q:                      int = 12
    HARMONIC_Q:                 int = 12