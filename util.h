#ifndef util_H
#define util_H

#include <stdio.h>
#include <stdbool.h>

double centsToDollars(size_t cents);
size_t dollarsToCents(double dollars);
bool invalidTicker(char *ticker);
void getCharClean(void);

#endif