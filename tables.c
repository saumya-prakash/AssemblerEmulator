#include "tables.h"


struct label* new_label(char name[50], unsigned int a)
{
    struct label* tmp = (struct label*) malloc(sizeof(struct label));

    strcpy(tmp->name, name);
    tmp->a = a;
    tmp->next = NULL;

    return tmp;
}

unsigned hash(const char *name)
{
    unsigned res = 0;
    
    while(*name!='\0')
    {
        res += *name;
        ++name;
    }
    return res%26;
}