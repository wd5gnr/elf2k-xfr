Paced Uploaders
===============

These are two programs I wrote a long time ago to solve specific problems uploading
to the COSMAC ELF2K and AmForth. I have since made it configurable so one program should
work for many different use cases.

The idea is to send a character and wait for an echo and/or some special characters.
This plugs into minicom.

Compile
=======
To compile this file:

```
gcc -o elf2k-xfr elf2k-xfr.c 
```

Then copy the executable to your path somewhere 

Install
=======
Minicom uploader has stdin connected to serial input,
stdout connected to serial output
stderr connects back to minicom
exit code 0 for success or non zero for fail
 
To set this up, use Control+AO and configure file transfer 
protcols. Set a name and the program file name 
Set Name=Y, U/D to U, FullScrn to N, IO-Red to Y, and Multi to N

OR you can put this in /etc/minicom/minirc.elf (or whatever you want)

```
# Machine-generated file - use setup menu in minicom to change parameters.
pu pname10          YUNYNamforth
pu pprog10          elf2k-xfr
pu baudrate         1200
pu bits             8
pu parity           N
pu stopbits         1
pu minit            
pu mreset           
pu backspace        BS
pu rtscts           No

 ```

 

 Then start minicom like this:
```
minicom -w -D /dev/ttyUSB1 elf
```

This presumes you have the program on your path
and you are using /dev/ttyUSB1 at 1200 baud, of course.
 

The character pacing is handled by waiting for the echo

The line pacing is handled by waiting for >>> This is the part you might need to change for some other setup

A ? in the prompt output indicates an error


Added a 10ms sleep before each line which makes it work better.

Command line options

* -d delay - Delay before starting new line (default 10000us)
* -c char - Character to wait for (default >)
* -n num - Number of characters to eat after the -c character (default 2)
* -e err - Error character (default ?)

So for elf2k monitor, the defaults are fine. For Basic on the ELF2k use as
program name: elf2k-xfer -c > -n 0 -e :

Note that since minicom doesn't pass this through a shell, you don't need to quote > or : -- it will work fine. What these options do is
to wait for a single > (-c with -n 0) and if the response contains a : consider it an error and abort. This works for the RC/Basic L2.



Public domain by Al Williams
This was repacked as am4up for use with AmForth.
https://sourceforge.net/p/amforth/code/HEAD/tree/releases/5.1/tools/am4up.c

