#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "util.h"
#include "tree.h"

void theFunc(double *d)
{
    printf("%lf\n", *d);
}

int main(int argc, char *argv[])
{
    FILE *readFile;

    int tempStorageMax = 64;
    int storageCounter = 0;

    size_t fileSz=0;
    size_t fileCap=4;
    char **fileData = malloc(fileCap * sizeof(*fileData));

    size_t inputSz=0;
    size_t inputCap=4;
    char **inputData = malloc(inputCap * sizeof(*inputData));

    // Checking for valid amount for args
    if(argc != 2)
    {
        fprintf(stderr, "Invalid amount of args\n");
        return 1;
    }

    // Attemping to open the file given
    readFile = fopen(argv[1], "r");
    if(readFile == NULL)
    {
        fprintf(stderr, "Unable to open the file: %s\n", argv[1]);
        return 1;
    }

    char buf = fgetc(readFile);
    while(buf != EOF)
    {
        char *storage = malloc(tempStorageMax);

        // Dynamicly allocating each line in the file as a string
        while(buf != EOF && buf != 10)
        {

            storage[storageCounter] = buf;

            storageCounter++;
            buf = fgetc(readFile);

            // Allocating more memory for the line
            if (storageCounter >= tempStorageMax - 1) {
                tempStorageMax *= 2;
                void *tmp_fileData = realloc(storage, tempStorageMax * sizeof(storage));
                if (tmp_fileData) {
                    storage = tmp_fileData;
                } else {
                     // Freeing mallocs
                    for (unsigned int n = 0; n < fileSz; n++) 
                    {
                        free(fileData[n]);
                    }
                    free(fileData);
                    // Closing opened files
                    fclose(readFile);
                    return 1;
            }
            }
        }

        // Adding eaching line to an array
        storage[storageCounter] = '\0';
        buf = fgetc(readFile);
        storageCounter = 0;
        fileData[fileSz++] = storage;
        // Allocating more memory for the array for lines
        if (fileSz >= fileCap) {
            fileCap *= 2;
            void *tmp_fileData = realloc(fileData, fileCap * sizeof(*fileData));
            if (tmp_fileData) {
                fileData = tmp_fileData;
            } else {
                 // Freeing mallocs
                for (unsigned int n = 0; n < fileSz; n++) 
                {
                    free(fileData[n]);
                }
                free(fileData);
                // Closing opened files
                fclose(readFile);
                return 1;
            }
        }
    }

    storageCounter = 0;

    buf = getc(stdin);
    while(buf != EOF)
    {
        char *storage = malloc(tempStorageMax);
        if(!storage)
        {
            continue;
        }

        // Dynamicly allocating each line in the file as a string
        while(buf != EOF)
        {

            storage[storageCounter] = buf;

            storageCounter++;
            buf = getc(stdin);

            // Allocating more memory for the line
            if (storageCounter >= tempStorageMax - 1) {
                tempStorageMax *= 2;
                void *tmp_fileData = realloc(storage, tempStorageMax * sizeof(storage));
                if (tmp_fileData) {
                    storage = tmp_fileData;
                } else {
                     // Freeing mallocs
                    for (unsigned int n = 0; n < inputSz; n++) 
                    {
                        free(inputData[n]);
                    }
                    free(inputData);
                    return 1;
                }
            }
        }

        // Adding eaching line to an array
        storage[storageCounter] = '\0';
        storageCounter = 0;
        inputData[inputSz++] = storage;

        // Allocating more memory for the array for lines
        if (inputSz >= inputCap) {
            inputCap *= 2;
            void *tmp_fileData = realloc(inputData, inputCap * sizeof(*inputData));
            if (tmp_fileData) {
                inputData = tmp_fileData;
            } else {
                 // Freeing mallocs
                for (unsigned int n = 0; n < inputSz; n++) 
                {
                    free(inputData[n]);
                }
                free(inputData);
                return 1;
            }
        }
    }



    // Closing the read file
    fclose(readFile);

    // Checking if file is null
    if(fileSz == 0)
    {
        fprintf(stderr, "File is null: %s\n", argv[1]);
        for (unsigned int n = 0; n < fileSz; n++) 
        {
            free(fileData[n]);
        }
        free(fileData);
        return 1;
    }

    tree *market = createTree();

    // TODO Check for write $ amount
    // TODO Skip bad lines
    for (unsigned int n = 0; n <fileSz; n++) 
    {
        double value = 0.0;
        char ticker[6];
        char name[64];
        char buf[64];

        int tracker = 0;
        int tempTracker = 0;

        strcpy(name, "\0");
        strcpy(buf, "\0");
        strcpy(ticker, "\0");


        if(sscanf(fileData[n], "%5s %lf%n", ticker, &value, &tracker) != 2)
        {
            continue;
        }

        if(value < 0.00 || value > 1000000.00)
        {
            continue;
        }

        if(invalidTicker(ticker))
        {
            continue;
        }

        while(sscanf(fileData[n] + tracker, "%63s%n", buf, &tempTracker) > 0)
        {
            if((strlen(buf) + strlen(name)) <= 63)
            {
                strcat(name, buf);
                strcat(name, " ");
                tracker += tempTracker;

            }
            else
            {
                strncat(name, buf, 63 - strlen(name));
                name[63] = '\0';
                break;
            }
        }

        tree_insert(&market, ticker, name, dollarsToCents(value));
    }

    

    char temp[6];
    strcpy(temp, "TTT");
    treeUpdate(&market, temp, 10.3);
    //strcpy(temp, "CATT");
    //treeAdd(&market, temp, 34322.2);

    tree_print(market);
    printf("\n");

    strncpy(temp, "NKE", 6);
    treeUpdate(&market, temp, -50.3);
    strncpy(temp, "TTT", 6);
    treeUpdate(&market, temp, 122.3);

    tree_print(market);
    printf("\n");

    // Freeing mallocs
    for (unsigned int n = 0; n < fileSz; n++) 
    {
        free(fileData[n]);
    }
    free(fileData);

    // Freeing mallocs
    for (unsigned int n = 0; n < inputSz; n++) 
    {
        free(inputData[n]);
    }
    free(inputData);

    tree_disassemble(market);
}
