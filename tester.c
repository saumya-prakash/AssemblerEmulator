#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

int* arr[26] = {NULL};

int main()
{
    for(int i=0; i<26; ++i)
    {
        if(arr[i]==NULL)
            printf("12\n");
    }

    return 0;
}