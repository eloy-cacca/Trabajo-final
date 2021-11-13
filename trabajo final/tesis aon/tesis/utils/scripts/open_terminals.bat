@ECHO OFF

set ARM_CMD=arm-none-eabi-gdb -ex="target extended-remote aonpi2.local:3333" arm-fw.elf
set DEBUG_OPTIONS=--title "Debugger" --tabColor #E45649 --suppressApplicationTitle
set ARM_GDB_OPTIONS=--title "Arm-none-eabi-gdb" --suppressApplicationTitle
set ELF_PATH=%1

wt %DEBUG_OPTIONS% ssh -t pi@aonpi2.local "cd openocd\; sudo openocd -f raspberry.cfg\; bash"  ; %ARM_GDB_OPTIONS% cmd.exe /K "timeout 5 && cd /d %ELF_PATH% && %ARM_CMD%"