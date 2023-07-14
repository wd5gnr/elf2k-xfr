This is a very simple program to handle sending data to old microcontrollers that may have bit-banged serial. It plugs into minicom.

Minicom uploader has stdin connected to serial input,
stdout connected to serial output
stderr connects back to minicom
exit code 0 for success or non zero for fail

To set this up, use Control+AO and configure file transfer 
protcols. Set a name and the program file name 
 Set Name=Y, U/D to U, FullScrn to N, IO-Red to Y, and Multi to N


The character pacing is handled by waiting for the echo

The line pacing is handled by waiting for >>> This is the part you might need to change for some other setup

A ? in the prompt output indicates an error


Added a 500uS sleep before each line which makes it work better.

Build: gcc -o elf2k-xfr elf2k-xfr.c




Public domain by Al Williams
This was repacked as am4up for use with AmForth.
https://sourceforge.net/p/amforth/code/HEAD/tree/releases/5.1/tools/am4up.c

