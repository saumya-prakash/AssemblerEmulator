#include "tables.h"

struct label* symTable[26]={ NULL };


struct label* new_label(const char *name, unsigned int a)
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


struct label* search(const char *name)
{
    int ind = hash(name); 

    struct label* head=NULL;

    head = symTable[ind];

    while(head!=NULL)
    {
        if(strcmp(head->name, name)==0)
        {
            return head;
        }

        head = head->next;
    }


    return NULL;    /* label node with corresponding node not found in symbol table */

}

int insert(const char* name, unsigned int a)
{
    struct label* tmp = search(name);
    struct label* tmp2 = NULL;
    int ind = 0;

    if(tmp!=NULL)   /* label already included in the symbol table */
    {
        return -1;
    }

    ind = hash(name);

    tmp = new_label(name, a);

    tmp2 = symTable[ind];
    symTable[ind] = tmp;
    tmp->next = tmp2;

    return 1;       /* successfully inserted in the symbol table */
}