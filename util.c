#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "util.h"

// Converts a double to a size_t
size_t
dollarsToCents (double dollars)
{
  size_t cents = 0;
  cents = dollars * 100;
  return cents;
}

// Returns if the string is a valid ticker
bool
invalidTicker (char *ticker)
{
  for (unsigned int i = 0; strlen (ticker) > i; i++)
    {
      if (isalnum (ticker[i]) == 0)
	{
	  if (ticker[i] != '.' && ticker[i] != '-' && ticker[i] != '/')
	    {
	      return true;
	    }
	}
    }
  return false;
}

// Removes extra chars at the end of stdin
void
getCharClean (void)
{
  int clearChar = getchar ();
  while ((clearChar != '\n') && (clearChar != EOF))
    {
      clearChar = getchar ();
    }
}
