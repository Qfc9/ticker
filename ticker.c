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
    

    size_t sz=0;
    size_t cap=4;
    char **data = malloc(cap * sizeof(*data));
    int storageMax = 64;
    int counter = 0;

    char buf = fgetc(readFile);
    while(buf != EOF)
    {
        char *storage = malloc(storageMax);

        // Dynamicly allocating each line in the file as a string
        while(buf != EOF && buf != 10)
        {

            storage[counter] = buf;

            counter++;
            buf = fgetc(readFile);

            // Allocating more memory for the line
            if (counter >= storageMax - 1) {
                storageMax *= 2;
                void *tmp_data = realloc(storage, storageMax * sizeof(storage));
                if (tmp_data) {
                    storage = tmp_data;
                } else {
                     // Freeing mallocs
                    for (unsigned int n = 0; n < sz; n++) 
                    {
                        free(data[n]);
                    }
                    free(data);
                    // Closing opened files
                    fclose(readFile);
                    return 1;
            }
            }
        }

        // Adding eaching line to an array
        storage[counter] = '\0';
        buf = fgetc(readFile);
        counter = 0;
        data[sz++] = storage;
        // Allocating more memory for the array for lines
        if (sz >= cap) {
            cap *= 2;
            void *tmp_data = realloc(data, cap * sizeof(*data));
            if (tmp_data) {
                data = tmp_data;
            } else {
                 // Freeing mallocs
                for (unsigned int n = 0; n < sz; n++) 
                {
                    free(data[n]);
                }
                free(data);
                // Closing opened files
                fclose(readFile);
                return 1;
            }
        }
    }


    // Closing the read file
    fclose(readFile);

    // Checking if file is null
    if(sz == 0)
    {
        fprintf(stderr, "File is null: %s\n", argv[1]);
        for (unsigned int n = 0; n < sz; n++) 
        {
            free(data[n]);
        }
        free(data);
        return 1;
    }


    tree *market = createTree();

    // TODO Check for write $ amount
    // TODO Skip bad lines
    for (unsigned int n = 0; n < sz; n++) 
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


        if(sscanf(data[n], "%5s %lf%n", ticker, &value, &tracker) != 2)
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

        while(sscanf(data[n] + tracker, "%63s%n", buf, &tempTracker) > 0)
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

    tree_print(market);
    printf("\n");

    char temp[6];
    strcpy(temp, "AAA");
    treeAdd(&market, temp, 32.2);
    strcpy(temp, "CAT");
    treeAdd(&market, temp, 32.2);

    tree_print(market);
    printf("\n");

    // Freeing mallocs
    for (unsigned int n = 0; n < sz; n++) 
    {
        free(data[n]);
    }
    free(data);

    tree_disassemble(market);
}
