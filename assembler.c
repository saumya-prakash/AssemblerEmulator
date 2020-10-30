#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <error.h>

#include "tables.h"


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

    
    char line[100] = {'\0'};

    while(fgets(line, 100, src))
    {
        /* fputs(line, stdout); */

        char* tmp;

        while(sscanf(line, "%s", tmp))
        {
            printf("%s\n", line);
        }

    }
    


    fclose(src);    /* close source file */

    /* error() ; */

    exit(EXIT_SUCCESS);
}