/*****************************************************************

I hereby declare that I have authored this program independently and
without any plagiarism from any other source.

  Name - Saumya Prakash
  Roll No. - 1801CS68



******************************************************************/


#include <stdlib.h>
#include <string.h>


struct label
{
    char name[50];
    unsigned int a;
    struct label* next;
};




struct label* new_label(const char *, unsigned int a) ;

unsigned hash(const char *) ;

struct label* search(const char *) ;

int insert(const char* name, unsigned int a) ;