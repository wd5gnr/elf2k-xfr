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



// wait for prompt -- errc is an error character
void elfwait(int errc)
  {
    int c;
    // wait for response
    while ((c=getchar())!='>')
      {
	fputc(c,stderr);
	if (c==errc) { fprintf(stderr,"\nError - %c\n",errc); exit(2); }
      }
    fputc('>',stderr);
    fputc(getchar(),stderr); fputc(getchar(),stderr);  // eat next two >>
  }


int main(int argc, char *argv[])
{
  FILE *f=NULL;
  time_t t0,t1;
  fprintf(stderr,"elf2k-xfr by Al Williams... Uploading ");
  if (argc>1) 
    {
      fprintf(stderr,"%s\n",argv[1]);
      f=fopen(argv[1],"r");
    }
  if (!f) { fprintf(stderr, "No file\n"); exit(1); }
  putchar('\r'); putchar('\n');
  elfwait('\0');
  time(&t0);
  while (!feof(f))
    {
      int c=getc(f),c1;
      if (c==EOF) break;
      // newline == CRLF
      if (c=='\n') c='\r';
      if (c!=EOF) { putchar(c); fputc(c,stderr); }  
      if (c=='\r') { putchar('\n'); fputc('\n',stderr); }
// read echo
      c1=getchar(); 
      if (c1!=c) { fprintf(stderr,"\nError (%c,%c)\n",c1,c); exit(2); }
      if (c=='\r') 
	{
	  elfwait('?');
	}
    }
  time(&t1);
  fprintf(stderr,"\nTransfer time=%ld seconds\n",t1-t0);

  return 0;
}
  
