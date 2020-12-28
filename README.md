# IR Project
This Information Retrieval (IR) project implements Boolean model and Phrase (free) model.
This Software is developed using C99 programming language. Developed on Windows and can only be run on windows as the software is using Windows APIs <windows.h>

# Boolean Model
Currently not finished. The only missing implementation is the its query processing.

# Phrase (free) Model
Fully implemented. The phrase (free) done using Positional Index Model.

# Query Processing
Query processing is done using Proximity Algorithm.

# Links
You need to download a c compiler on windows in order to build the software on your machine and run it.
I recommend downloading tdm-gcc: https://jmeubank.github.io/tdm-gcc/download/

# Project structure
1-The src directory holds all .c (source code) files.
2-The header directory holds all .h (header) files.
3-The bin directory holds all .o (object) files.
4-The txt directory holds a dump files used to test the software. If you want to delete them, change the in the main.c file the files I/O operation

# How to run the software
Use the file compile_main.bat for the first time. It will compile the software and run it.
After that use IR.exe as you're main executable file.
