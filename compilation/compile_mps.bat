@echo off

cd ../src
gcc -Wall -Werror -c -o../bin/mps.o mps.c
cd ../compilation

pause