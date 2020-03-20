# IPC-Communication-CPSC-351

AUTHORS
--------------------------------------
Janelle Estabillo
estabillojanelle@csu.fullerton.edu
CPSC 351-02

Nidhi Shah
nidhi989@csu.fullerton.edu
CPSC 351-02

How to Execute the Program
---------------------------------------

$ make all
$ ./receiver

Open new Terminal
$ ./sender keyfile.txt

EXTRA NOTES
--------------------------------------

On line 28 on msg.h we changed "fprintf(fp, "%d %d", mtype, size);" to "fprintf(fp, "%ld %d", mtype, size);"
On line 

Programming Language Used: C++
Extra Credit: YES
