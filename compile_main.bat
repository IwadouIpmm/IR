@echo off

cd compilation
call compile_string
call compile_common
call compile_mps
cd ../
pause
cd src
gcc -Wall -Werror -o../IR main.c ../bin/string.o ../bin/common.o ../bin/mps.o
cd ../
pause
cls
IR.exe
pause
cls