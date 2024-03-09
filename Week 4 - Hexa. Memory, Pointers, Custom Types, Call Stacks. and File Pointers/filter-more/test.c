#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int intarr[3];
    intarr[0] = 1;
    intarr[1] = 5;
    intarr[2] = 8;
    intarr[3] = 7;
    int int2[3] = intarr[0:2];
    //intarr = {5,3,4};
    for (int i = 0; i < 3; i++)
    {
        printf("int is %i\n",int2[i]);
    }
}
