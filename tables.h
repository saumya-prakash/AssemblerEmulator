#include <stdlib.h>
#include <string.h>


struct label
{
    char name[50];
    unsigned int a;
    struct label* next;
};

struct symTable
{
    struct label* table[26];
};


struct label* new_label(char [50], unsigned int a) ;

unsigned hash(const char *) ;