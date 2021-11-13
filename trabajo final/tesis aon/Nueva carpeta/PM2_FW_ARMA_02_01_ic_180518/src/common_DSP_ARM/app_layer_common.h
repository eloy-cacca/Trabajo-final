// $Header
/// @file			app_layer_common.h
/// @brief  		Common communication application layer.
/// @details
/// @author 		Ignacio Carugati
/// @version   		1.0
/// @date      		05 March 2017
/// @details
/// @copyright
///

#ifndef APP_LAYER_COMMON_H_
#define APP_LAYER_COMMON_H_

//*****************************************************************************
// ARM and DSP - Communication ID
//*****************************************************************************

// ARM - Firmware
#define API_FW_ARM_VERSION_REQ                    (0x02)
#define API_FW_ARM_VERSION_RES                    (0x03)
#define API_FW_ARM_ECHO_REQ                       (0x04)
#define API_FW_ARM_ECHO_RES                       (0x05)
#define API_FW_ARM_UNIQUE_ID_REQ                  (0x06)
#define API_FW_ARM_UNIQUE_ID_RES                  (0x07)
#define API_FW_ARM_RESET_ID_REQ                   (0x08)
#define API_FW_ARM_RESET_ID_RES                   (0x09)
#define API_FW_ARM_UNKNOW_MSG_ID_RES              (0x0F)

// DSP - Firmware
#define API_FW_DSP_VERSION_REQ                    (0x10)
#define API_FW_DSP_VERSION_RES                    (0x11)
#define API_FW_DSP_ECHO_REQ                       (0x12)
#define API_FW_DSP_ECHO_RES                       (0x13)
#define API_FW_DSP_SEND_COMM_FLAG_REQ             (0x14)
#define API_FW_DSP_SEND_COMM_FLAG_RES             (0x15)
#define API_FW_DSP_UNKNOW_MSG_ID_RES              (0x1F)

// ARM - Unit configuration
#define API_UNIT_CONFIG_ARM_UPDATE_REQ            (0x20)
#define API_UNIT_CONFIG_ARM_UPDATE_RES            (0x21)
#define API_UNIT_CONFIG_ARM_SEND_REQ              (0x22)
#define API_UNIT_CONFIG_ARM_SEND_RES              (0x23)
#define API_UNIT_CONFIG_ARM_SET_DEFAULT_REQ       (0x24)
#define API_UNIT_CONFIG_ARM_SET_DEFAULT_RES       (0x25)
#define API_UNIT_CALIBRATION_ARM_UPDATE_REQ       (0x26)
#define API_UNIT_CALIBRATION_ARM_UPDATE_RES       (0x27)
#define API_UNIT_CALIBRATION_ARM_SEND_REQ         (0x28)
#define API_UNIT_CALIBRATION_ARM_SEND_RES         (0x29)
#define API_UNIT_CALIBRATION_ARM_SET_DEFAULT_REQ  (0x2A)
#define API_UNIT_CALIBRATION_ARM_SET_DEFAULT_RES  (0x2B)

// DSP - Unit configuration
#define API_UNIT_CONFIG_DSP_UPDATE_REQ            (0x30)
#define API_UNIT_CONFIG_DSP_UPDATE_RES            (0x31)
#define API_UNIT_CONFIG_DSP_SEND_REQ              (0x32)
#define API_UNIT_CONFIG_DSP_SEND_RES              (0x33)
#define API_UNIT_CONFIG_DSP_SET_DEFAULT_REQ       (0x34)
#define API_UNIT_CONFIG_DSP_SET_DEFAULT_RES       (0x35)
#define API_UNIT_CALIBRATION_DSP_UPDATE_REQ       (0x36)
#define API_UNIT_CALIBRATION_DSP_UPDATE_RES       (0x37)
#define API_UNIT_CALIBRATION_DSP_SEND_REQ         (0x38)
#define API_UNIT_CALIBRATION_DSP_SEND_RES         (0x39)
#define API_UNIT_CALIBRATION_DSP_SET_DEFAULT_REQ  (0x3A)
#define API_UNIT_CALIBRATION_DSP_SET_DEFAULT_RES  (0x3B)

// ARM - Real Time Clock
#define API_RTC_ARM_UPDATE_REQ                    (0x40)
#define API_RTC_ARM_UPDATE_RES                    (0x41)
#define API_RTC_ARM_SEND_REQ                      (0x42)
#define API_RTC_ARM_SEND_RES                      (0x43)

// DSP - Real Time Clock
#define API_RTC_DSP_UPDATE_REQ                    (0x50)
#define API_RTC_DSP_UPDATE_RES                    (0x51)
#define API_RTC_DSP_SEND_REQ                      (0x52)
#define API_RTC_DSP_SEND_RES                      (0x53)

// ARM - SD-Card (part 1)
#define API_SDCARD_DIR_OPEN_REQ                   (0x60)
#define API_SDCARD_DIR_OPEN_RES                   (0x61)
#define API_SDCARD_DIR_ITEM_NEXT_REQ              (0x62)
#define API_SDCARD_DIR_ITEM_NEXT_RES              (0x63)
#define API_SDCARD_DIR_ITEM_NAME_REQ              (0x64)
#define API_SDCARD_DIR_ITEM_NAME_RES              (0x65)
#define API_SDCARD_FILE_OPEN_REQ                  (0x66)
#define API_SDCARD_FILE_OPEN_RES                  (0x67)
#define API_SDCARD_FILE_READ_REQ                  (0x68)
#define API_SDCARD_FILE_READ_RES                  (0x69)
#define API_SDCARD_FILE_CLOSE_REQ                 (0x6A)
#define API_SDCARD_FILE_CLOSE_RES                 (0x6B)
#define API_SDCARD_FILE_SIZE_REQ                  (0x6C)
#define API_SDCARD_FILE_SIZE_RES                  (0x6D)
#define API_SDCARD_FILE_AND_DIR_REMOVE_REQ        (0x6E)
#define API_SDCARD_FILE_AND_DIR_REMOVE_RES        (0x6F)

// DSP - Control
#define API_CTRL_DSP_TREND_150CYCLES_REQ          (0x70)
#define API_CTRL_DSP_TREND_150CYCLES_RES          (0x71)
#define API_CTRL_DSP_TREND_5MINUTES_REQ           (0x72)
#define API_CTRL_DSP_TREND_5MINUTES_RES           (0x73)
#define API_CTRL_DSP_TELEMETRIA_REQ               (0x74)
#define API_CTRL_DSP_TELEMETRIA_RES               (0x75)
#define API_CTRL_DSP_EVENT_BUFFER_REQ             (0x78)
#define API_CTRL_DSP_EVENT_BUFFER_RES             (0x79)
#define API_CTRL_DSP_EVENT_DET_REQ                (0x7A)
#define API_CTRL_DSP_EVENT_DET_RES                (0x7B)

// ARM - Bootloader
#define API_BOOT_ARM_START_UPLOAD_REQ             (0x80)
#define API_BOOT_ARM_START_UPLOAD_RES             (0x81)
#define API_BOOT_ARM_UPLOAD_BLOCK_REQ             (0x82)
#define API_BOOT_ARM_UPLOAD_BLOCK_RES             (0x83)
#define API_BOOT_ARM_INSTALL_IMAGE_REQ            (0x84)
#define API_BOOT_ARM_INSTALL_IMAGE_RES            (0x85)

// ARM - SD-Card (part 2)
#define API_SDCARD_GET_MBYTES_REQ                 (0xA0)
#define API_SDCARD_GET_MBYTES_RES                 (0xA1)
///// todo: agregar archivo
///// todo: borrar todas las carpetas de la sd-card.

#endif /* APP_LAYER_COMMON_H_ */
