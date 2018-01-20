#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "util.h"
#include "tree.h"

// This is my main function
int
main (int argc, char *argv[])
{
  // Initializing Variables
  FILE *readFile;

  int tempStorageMax = 64;
  int storageCounter = 0;

  size_t fileSz = 0;
  size_t fileCap = 4;
  char **fileData = malloc (fileCap * sizeof (*fileData));

  size_t inputSz = 0;
  size_t inputCap = 4;
  char **inputData = malloc (inputCap * sizeof (*inputData));

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

  // Getting all the data in the file given
  char buf = fgetc (readFile);
  while (buf != EOF)
    {
      char *storage = malloc (tempStorageMax);

      // Dynamicly allocating each line in the file as a string
      while (buf != EOF && buf != 10 && buf != 35)
	{

	  storage[storageCounter] = buf;

	  storageCounter++;
	  buf = fgetc (readFile);

	  // Allocating more memory for the line
	  if (storageCounter >= tempStorageMax - 1)
	    {
	      tempStorageMax *= 2;
	      void *tmp_fileData =
		realloc (storage, tempStorageMax * sizeof (storage));
	      if (tmp_fileData)
		{
		  storage = tmp_fileData;
		}
	      else
		{
		  // Freeing mallocs
		  for (unsigned int n = 0; n < fileSz; n++)
		    {
		      free (fileData[n]);
		    }
		  free (fileData);
		  // Closing opened files
		  fclose (readFile);
		  return 1;
		}
	    }
	}

      // Adding eaching line to an array
      storage[storageCounter] = '\0';
      buf = fgetc (readFile);
      storageCounter = 0;
      fileData[fileSz++] = storage;
      // Allocating more memory for the array for lines
      if (fileSz >= fileCap)
	{
	  fileCap *= 2;
	  void *tmp_fileData =
	    realloc (fileData, fileCap * sizeof (*fileData));
	  if (tmp_fileData)
	    {
	      fileData = tmp_fileData;
	    }
	  else
	    {
	      // Freeing mallocs
	      for (unsigned int n = 0; n < fileSz; n++)
		{
		  free (fileData[n]);
		}
	      free (fileData);
	      // Closing opened files
	      fclose (readFile);
	      return 1;
	    }
	}
    }

  // Closing the read file
  fclose (readFile);

  // Checking if file is null
  if (fileSz == 0)
    {
      fprintf (stderr, "File is null: %s\n", argv[1]);
      for (unsigned int n = 0; n < fileSz; n++)
	{
	  free (fileData[n]);
	}
      free (fileData);
      free (inputData);
      return 1;
    }

  // Reseting the counter just in case
  storageCounter = 0;

  // Getting data from stdin
  buf = getc (stdin);
  while (buf != EOF)
    {
      char *storage = malloc (tempStorageMax);
      if (!storage)
	{
	  continue;
	}

      // Dynamicly allocating each line in the file as a string
      while (buf != EOF && buf != 10)
	{

	  storage[storageCounter] = buf;

	  storageCounter++;
	  buf = getc (stdin);

	  // Allocating more memory for the line
	  if (storageCounter >= tempStorageMax - 1)
	    {
	      tempStorageMax *= 2;
	      void *tmp_fileData =
		realloc (storage, tempStorageMax * sizeof (storage));
	      if (tmp_fileData)
		{
		  storage = tmp_fileData;
		}
	      else
		{
		  // Freeing mallocs
		  for (unsigned int n = 0; n < inputSz; n++)
		    {
		      free (inputData[n]);
		    }
		  free (inputData);
		  return 1;
		}
	    }
	}

      // Adding eaching line to an array
      storage[storageCounter] = '\0';
      buf = getc (stdin);
      storageCounter = 0;
      inputData[inputSz++] = storage;

      // Allocating more memory for the array for lines
      if (inputSz >= inputCap)
	{
	  inputCap *= 2;
	  void *tmp_fileData =
	    realloc (inputData, inputCap * sizeof (*inputData));
	  if (tmp_fileData)
	    {
	      inputData = tmp_fileData;
	    }
	  else
	    {
	      // Freeing mallocs
	      for (unsigned int n = 0; n < inputSz; n++)
		{
		  free (inputData[n]);
		}
	      free (inputData);
	      return 1;
	    }
	}
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
  tree_print (market);
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
  tree_disassemble (market);
}
