#include "util.h"

double centsToDollars(size_t cents)
{
    double dollars = 0;
    dollars = cents/100;
    return dollars;
}

size_t dollarsToCents(double dollars)
{
    size_t cents = 0;
    cents = dollars * 100;
    return cents;
}