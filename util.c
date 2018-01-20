#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "util.h"

size_t dollarsToCents(double dollars)
{
    size_t cents = 0;
    cents = dollars * 100;
    return cents;
}

bool invalidTicker(char *ticker)
{
    for(unsigned int i = 0; strlen(ticker) > i; i++)
    {
        if(isalnum(ticker[i]) == 0)
        {
            if(ticker[i] != '.' && ticker[i] != '-' && ticker[i] != '/')
            {
                return true;
            }
        }
    }
    return false;
}

void getCharClean(void)
{
    int clearChar = getchar();
    while ((clearChar != '\n') && (clearChar != EOF))
    {
        clearChar = getchar();
    }
}