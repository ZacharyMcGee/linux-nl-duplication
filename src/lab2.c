#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

/*
FIXED from lab1 feedback:
1. Global variables
2. The unused int b
3. Fix error handing for getopt
4. Fix error handling for fopen
5. Get rid of scanf(Not allowed) use process_stream on STDIN
6. Make int linenum static so that the line value stays the same
7. For printf, don't assume spacing
8. STDIN should not have own process_stream
9. buffer length should be 102
10. Check if buffer is empty before returning NULL
11. Add newline character to buffer
*/

#define MAX_LINE_LENGTH 100 // CONSTANT FOR MAX LINE LENGTH

// ENUM TYPEDEF FOR STYLES
typedef enum { ALL_LINES, NO_LINES, NONEMPTY_LINES } style; // STYLE OPTIONS

// PROTOTYPES
void process_stream(FILE *fpntr, char *file_pathname, style s, char* string); // process_stream FOR FILES
char *fgetline(FILE *fpntr); // GET LINE USING GETC


int main(int argc, char *argv[])
{
  int argcnt = 1; // I couldn't remember how to get the files, so I rigged this up
  int c; // OPTIONS
  int errorp = 0; // FOR ERROR CHECKING

  FILE *fpntr; // FILE POINTER

  char *string = "\t"; // INITIALIZE STRING OPTION TO TAB
  char *file_pathname; // FOR HOLDING THE FILE PATHNAME

  style s = NONEMPTY_LINES; // INITIALIZE TO OPTION -t

  while((c = getopt (argc, argv, "b:s:")) != -1)
  switch (c){
    case 'b': // IF THE B OPTION
      if(strcmp(optarg, "a") == 0)
      {
        s = ALL_LINES; // OPTION -BA = ALL LINES
      }
      else if(strcmp(optarg, "n") == 0)
      {
        s = NO_LINES; // OPTION -BN = NO LINES
      }
      else if(strcmp(optarg, "t") == 0)
      {
        s = NONEMPTY_LINES; // OPTION -BT = NON EMPTY LINES
      }
      else // IF ANT NOT CHOOSEN THEN ERROR
      {
        fprintf(stderr, "Invalid -b option STYLE: %s\n", optarg);
        fprintf(stderr, "(STYLE must be one of: a, n, t)\n");
        exit(EXIT_FAILURE);
      }
      argcnt++; // ANOTHER ARGUMENT BEFORE FILES
      break; // STOP LOOKING
    case 's':
      string = optarg; // STRING OPTION CHOSEN, SAVE THE STRING
      argcnt++; // ANOTHER ARGUMENT BEFORE FILES
      break;
    default:
      fprintf(stderr, "Usage: %s [-bSTYLE] [-sSTRING] [FILE...]\n", argv[0]);
      fprintf(stderr, "(STYLE must be one of: a, n, t)\n");
      exit(EXIT_FAILURE);
}

  if(argv[argcnt] != NULL) // FIRST CHECK IF THERE IS A FILE ARGUMENT
  {
    while(argv[argcnt] != NULL) // LOOP THROUGH FILES WHILE NOT NULL
    {
      file_pathname = argv[argcnt++];

      if((fpntr = fopen(file_pathname, "r")) != NULL)
      {
        process_stream(fpntr, file_pathname, s, string); // CALL process_stream ON FPNTR
        fclose(fpntr);
      }
      else
      {
        fprintf(stderr, "%s: %s: %s\n", argv[0], file_pathname, strerror(errno));
        errorp = 1;
      }
    }
  }
  else // NO FILE ARGUMENT, READ FROM STDIN
  {
    process_stream(stdin, "stdin", s, string);
  }

  if(errorp){
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS; // WE MADE IT WITHOUT AN ERROR, SUCCESS HOPEFULLY?
}


void process_stream(FILE *fpntr, char *file_pathname, style s, char* string)
{
  static int linenum = 1; // count the number of lines
  char *line; // THE LINE WE GET
  int blank_indent_width = 6 + strlen(string);

  while((line = fgetline(fpntr)) != NULL) // LOOP TILL fgetline RETURNS NULL
  {
      if(s == NONEMPTY_LINES) // -bt
      {
        if(strcmp(line, "\n") == 0) // THE DIFFERENCE FROM ALL(ONLY PRINT ON NONEMPTY)
        {
          printf("%*s%s", blank_indent_width, " ", line);
        }
        else
        {
          printf("%6d%s%s", linenum++, string, line); // FORMAT FOR NONEMPTY
        }
      }
      else if(s == ALL_LINES) // -ba
      {
        printf("%6d%s%s", linenum++, string, line); // FORMAT FOR ALL
      }
      else if(s == NO_LINES) // -bn
      {
        printf("%*s%s", blank_indent_width, string, line); // FORMAT FOR NONE(NO LINENUM)
      }
    }
}

char *fgetline(FILE *fpntr)
{
  int ch; // VARIABLE FOR SINGLE CHAR FOR GETC
  static char buffer[MAX_LINE_LENGTH + 2]; // ARRAY OF CHARS FOR THE LINE
  int i = 0; // KEEP TRACK OF ARRAY INDEX

  while((ch = getc(fpntr)) != EOF && ch != '\n') // WHILE NOT EOF OR NEWLINE
  {
    buffer[i++] = ch; // ADD NEXT CHAR INTO THE BUFFER
  }

  if(ch == EOF && (i == 0 || ferror(fpntr)))
  {
    return NULL; // WE REACHED END OF FILE, RETURN NULL SO LOOP WILL QUIT CALLING
  }

  buffer[i++] = '\n';
  buffer[i] = '\0'; // WE SHOULD ADD THIS TO THE END OF THE STRING TO NULL TERMINATE STRING
  return buffer; // WE MADE IT HERE WITHOUT RETURNING NULL, SO RETURN THE LINE
}
