#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <getopt.h>

void process_stream(FILE *fpntr); // process_stream FOR FILES
char *fgetline(FILE *fpntr); // GET LINE USING GETC

enum style { ALL_LINES, NO_LINES, NONEMPTY_LINES }; // STYLE OPTIONS
enum style s = NONEMPTY_LINES; // INITIALIZE TO OPTION -t

char string[101] = "\t"; // INITIALIZE STRING OPTION TO TAB
static int inputlinenum = 1; // DIFFERENT LINE NUM COUNTER FOR INPUT(STATIC)



int main(int argc, char *argv[])
{
  int argcnt = 1; // I couldn't remember how to get the files, so I rigged this up
  int c;


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
        printf("ERROR");
      }
      argcnt++; // ANOTHER ARGUMENT BEFORE FILES
      break; // STOP LOOKING
    case 's':
      strcpy(string, optarg); // STRING OPTION CHOSEN, SAVE THE STRING
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
      FILE *fpntr = fopen(argv[argcnt++], "r"); // SET FPNTR TO FILE GIVEN
      process_stream(fpntr); // CALL process_stream ON FPNTR
    }
  }
  else // NO FILE ARGUMENT, READ FROM STDIN
  {
    process_stream(stdin);
  }
  return EXIT_SUCCESS; // WE MADE IT WITHOUT AN ERROR, SUCCESS HOPEFULLY?
}


void process_stream(FILE *fpntr)
{
  static int linenum = 1; // count the number of lines
  char *line; // THE LINE WE GET
  int blank_indent_width = 6 + strlen(string);

  while((line = fgetline(fpntr)) != NULL) // LOOP TILL fgetline RETURNS NULL
  {
      if(s == NONEMPTY_LINES) // -bt
      {
        if(line[0] != '\0') // THE DIFFERENCE FROM ALL(ONLY PRINT ON NONEMPTY)
        {
          printf("%6d%s%s", linenum++, string, line); // FORMAT FOR NONEMPTY
        }
        else
        {
          printf("%*s%s", blank_indent_width, " ", line);
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
        printf("\n"); // ADD A NEWLINE
    }
}

char *fgetline(FILE *fpntr)
{
  int ch; // VARIABLE FOR SINGLE CHAR FOR GETC
  static char buffer[101]; // ARRAY OF CHARS FOR THE LINE
  int i = 0; // KEEP TRACK OF ARRAY INDEX

  while((ch = getc(fpntr)) != EOF && ch != '\n') // WHILE NOT EOF OR NEWLINE
  {
    buffer[i++] = ch; // ADD NEXT CHAR INTO THE BUFFER
  }

  if(feof(fpntr)){
    return NULL; // WE REACHED END OF FILE, RETURN NULL SO LOOP WILL QUIT CALLING
  }

  buffer[i] = '\0'; // WE SHOULD ADD THIS TO THE END OF THE STRING TO NULL TERMINATE STRING
  return buffer; // WE MADE IT HERE WITHOUT RETURNING NULL, SO RETURN THE LINE
}
