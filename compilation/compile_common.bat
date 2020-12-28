@echo off

cd ../src
gcc -Wall -Werror -c -o ../bin/common.o common.c
cd ../compilation

pause