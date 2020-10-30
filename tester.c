#include <stdio.h>
#include <string.h>
#include <stdlib.h>


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

int main()
{
    printf("%u %u\n", hash("a"), hash("A"));



    return 0;
}