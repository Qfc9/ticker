
#include <stdio.h>

#include "avl.h"


int main(int argc, char *argv[])
{
    FILE *fp;

    // Checking for valid amount for args
    if(argc != 2)
    {
        fprintf(stderr, "Invalid amount of args\n");
        return 1;
    }

    // Attemping to open the file given
    fp = fopen(argv[1], "r");
    if(fp == NULL)
    {
        fprintf(stderr, "Unable to open the file: %s\n", argv[1]);
        return 1;
    }
    fclose(fp);
}
