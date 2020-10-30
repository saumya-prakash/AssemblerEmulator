#include <stdio.h>

#include <ctype.h>
#include <error.h>

#include "tables.h"


#define LINE_LENGTH 1000

int main(int argc, char **argv)
{        
    FILE *src = NULL;       /* FILE pointer for source asm file */ 

    if(argc==1)     /* no command-line arguments given */
    {
        printf("Fatal error: No input file(s)\n");
        exit(EXIT_FAILURE);
    }
    
    src = fopen(argv[1], "r");      /* open source file in read mode */

    if(src==NULL)       /* error in opening */
    {
        perror("\0");   /* print error message */
        exit(EXIT_FAILURE);
    }

    
    char line[LINE_LENGTH] = {'\0'};

    while(fgets(line, LINE_LENGTH, src))
    {
        char* ptr = line;

        while(*ptr!='\n' && *ptr!=';')
            ++ptr;

        *ptr = '\0';    /* ignore whaatever comes after ';' */
        
        ptr = line;
        while(*ptr!='\0')   /* printing only non-empty lines */
        {
            if(isprint(*ptr))
            {
                fputs(line, stdout);
                printf("\n");

                break;
            }

            ++ptr;
        }

        /* char tmp1[100], tmp2[100], tmp3[100]; */
        /* sscanf(line, "%s %s %s ", tmp1, tmp2, tmp3); */
        /* printf("%s %s %s\n", tmp1, tmp2, tmp3); */

    }
    


    fclose(src);    /* close source file */

    /* error() ; */

    exit(EXIT_SUCCESS);
}