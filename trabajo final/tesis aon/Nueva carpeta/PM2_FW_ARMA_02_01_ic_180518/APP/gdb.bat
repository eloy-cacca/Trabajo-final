@echo off

cls
@echo on
arm-none-eabi-gdb -ex="target remote 192.168.1.176:3333" PM2_FW_ARMA_6_2.elf