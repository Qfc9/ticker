
#include <stdio.h>

#include "avl.h"

void theFunc(double *d)
{
    printf("%lf\n", *d);
}

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

    avl *temps = avl_create();
    // No ABC :(
    
    avl_insert(&temps, 3.14);
    avl_remove(&temps, 3.11, 0.1);
    avl_insert(&temps, 2.71);
    avl_insert(&temps, 1.71);
    avl_insert(&temps, 0.71);
    avl_insert(&temps, -1.71);
    avl_insert(&temps, -2.66);

    printf("%zu\n", avl_height(temps));
    avl_print(temps);

    avl_insert(&temps, 0.0);

    printf("%zu\n", avl_height(temps));
    avl_print(temps);

    avl_remove(&temps, 2.71, 0.1);

    printf("%zu\n", avl_height(temps));
    avl_print(temps);

    printf("\n");

    tree_inorder(temps, theFunc);

    avl_disassemble(temps);
}
