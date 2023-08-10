// Filter to upload files to ELF2K from minicom
// Al Williams http://www.awce.com
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

// Minicom uploader has stdin connected to serial input,
// stdout connected to serial output
// stderr connects back to minicom
// exit code 0 for success or non zero for fail

// To set this up, use Control+AO and configure file transfer 
// protcols. Set a name and the program file name 
// Set Name=Y, U/D to U, FullScrn to N, IO-Red to Y, and Multi to N


// The character pacing is handled by waiting for the echo
// The line pacing is handled by waiting for >>>
// A ? in the prompt output indicates an error


// New version
// -d usec : delay time after line
// -c char : response char (default >)
// -n #    : # of response characters to eat (default 2, plus the first one)
// -e char : Error character (default ?


unsigned int ldelay=10000;
char rchar='>';
unsigned int nct=2;
char echar='?';

// wait for prompt -- errc is an error character
void elfwait(int errc)
  {
    int c;
    // wait for response
    while ((c=getchar())!=rchar)
      {
	fputc(c,stderr);
	if (c==errc) { fprintf(stderr,"\nError - %c(%c/%x)\n",errc,c,c); exit(2); }
      }
    fputc(rchar,stderr); 
    c=0;
    while (c!=nct)
      {
	fputc(getchar(),stderr); // eat rest of prompt
	c++;
      }
    usleep(ldelay);
  }


int main(int argc, char *argv[])
{
  FILE *f=NULL;
  time_t t0,t1;
  int opt;
  while ((opt=getopt(argc,argv,"d:c:n:e:"))!= -1)
    {
      switch (opt)
	{
	case 'd':
	  ldelay=atoi(optarg);
	  break;
	case 'c':
	  rchar=*optarg;
	  break;
	case 'n':
	  nct=atoi(optarg);
	  break;
	case 'e':
	  echar=*optarg;
	  break;
	default:
	  fprintf(stderr,"Bad argument to %s\n",argv[0]);
	  exit(2);
	}
    }
  fprintf(stderr,"elf2k-xfr by Al Williams... Uploading ");
  if (optind<argc)
    {
      fprintf(stderr,"%s\n",argv[optind]);
      f=fopen(argv[optind],"r");
    }
  if (!f) { fprintf(stderr, "No file\n"); exit(1); }
  putchar('\r'); // putchar('\n');
  elfwait('\0');
  time(&t0);
  while (!feof(f))
    {
      int c=getc(f),c1;
      if (c==EOF) break;
      // newline == CRLF
      if (c=='\n') c='\r';
      if (c!=EOF) { putchar(c); fputc(c,stderr); }  
      //      if (c=='\r') { putchar('\n'); fputc('\n',stderr); }
// read echo
      c1=getchar(); 
      if (c1!=c) { fprintf(stderr,"\nError (%c/%x,%c/%x)\n",c1,c1,c,c); exit(2); }
      if (c=='\r') 
	{
	  elfwait(echar);
	}
    }
  time(&t1);
  fprintf(stderr,"\nTransfer time=%ld seconds\n",t1-t0);

  return 0;
}
  
