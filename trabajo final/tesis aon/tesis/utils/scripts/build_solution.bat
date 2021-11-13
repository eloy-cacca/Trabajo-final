@ECHO OFF

set ATMEL_PATH="C:\Program Files (x86)\Atmel\Studio\7.0\AtmelStudio.exe"
set SOLUTION_PATH=%1
set LOG_PATH="%temp%\log.txt"

%ATMEL_PATH% %SOLUTION_PATH% /build debug /out %LOG_PATH%
type %LOG_PATH%
del %LOG_PATH%
