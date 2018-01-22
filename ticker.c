#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "getData.h"
#include "util.h"
#include "tree.h"

// This is my main function
int
main (int argc, char *argv[])
{
  // Initializing Variables
  FILE *readFile;

  size_t fileSz = 0;
  size_t fileCap = 4;

  size_t inputSz = 0;
  size_t inputCap = 4;

  char **inputData;
  char **fileData;

  // Checking for valid amount for args
  if (argc != 2)
    {
      fprintf (stderr, "Invalid amount of args\n");
      return 1;
    }

  // Attemping to open the file given
  readFile = fopen (argv[1], "r");
  if (readFile == NULL)
    {
      fprintf (stderr, "Unable to open the file: %s\n", argv[1]);
      return 1;
    }

    // Getting data from a file passed
  getData(readFile, &fileData, &fileSz, &fileCap);

      // Getting data from stdin
  getData(stdin, &inputData, &inputSz, &inputCap);

  // Closing the read file
  fclose (readFile);

    // Checking if everything is good
  if(!fileData || !inputData)
  {
    fprintf (stderr, "Read Error!!");
    return 1;
  }

  // Checking if file is null
  if (fileSz == 0)
    {
      fprintf (stderr, "File is null: %s\n", argv[1]);
      for (unsigned int n = 0; n < fileSz; n++)
	{
	  free (fileData[n]);
	}
      for (unsigned int n = 0; n < inputSz; n++)
    {
      free (inputData[n]);
    }
      free(fileData);
      free (inputData);
      return 1;
    }

  // Creating the market tree
  tree *market = createTree ();

  // Adding all the values from the file into the tree
  for (unsigned int n = 0; n < fileSz; n++)
    {
      processCompany (&market, fileData[n]);
    }

  // Adding all the values from STDIN into the tree
  for (unsigned int n = 0; n < inputSz; n++)
    {
      processCompany (&market, inputData[n]);
    }

  // Printing the market
  printf ("\n");
  treePrint (market);
  printf ("\n");

  // Freeing file storage mallocs
  for (unsigned int n = 0; n < fileSz; n++)
    {
      free (fileData[n]);
    }
  free (fileData);

  // Freeing input mallocs
  for (unsigned int n = 0; n < inputSz; n++)
    {
      free (inputData[n]);
    }
  free (inputData);

  // Freeing the tree
  treeDisassemble (market);
}
