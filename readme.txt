# IPC-Communication-CPSC-351

AUTHORS
--------------------------------------
Janelle Estabillo
estabillojanelle@csu.fullerton.edu
CPSC 351-02

Nidhi Shah
nidhi989@csu.fullerton.edu
CPSC 351-02

-------------------------------------
different folder for two projects.
each folder have makefile which is according to project
------------------------------------------------------
How to Execute the Program- shared memory with message queue IPC.
---------------------------------------
platform: LINUX
commands for shared memory with message queue program:
$ make all
$ ./receiver

Open new Terminal
$ ./sender keyfile.txt

-----------------------------------------
Extra Credit: YES
----------------------------------------
How to Execute the Program for Extra Credit- signal shared memory IPC
---------------------------------------
platform: LINUX
commands for shared memory with signal handling:
$ make all
$ ./recvsig

Open new Terminal
$ ./sendersig keyfilesig.txt

--------------------------------------
SCREEN SHOT OF THE OUTPUT -signal handler
- saved in file : screenshot_output_signalhandlingIPC.png

EXTRA NOTES
--------------------------------------

Programming Language Used: C++

Contribution of Members:
-----------------------------------------
1st week: We spend time in understanding the given code and shared memory and message queue functions.
2nd and third week:- We divide first project into two parts- as sender and receiver.
                   - Nidhi work on sender and Janelle work on receiver.
                   - while we started to make readme file
4th and 5th weeK: - we finished on first project.
                  - we spend time in understanding SIGUSR1 and SIGUSR2 signal handling.
                  - At the end of 5th week, we completed extra credit project.
6th week:         - We work on document and clean our code.
