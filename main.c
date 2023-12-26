// As a Hokie, I will conduct myself with honor and integrity at all times.  
// I will not lie, cheat, or steal, nor will I accept the actions of those who do.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "comic.h"

/*This is the main method where 
a file that contains commands will
be parsed and functions will be
called according to the command.*/
int main (int argc, char* argv[])
{
    //Checking if the number or arguments is acceptable or not.
    if (argc != 3)
    {
        fprintf(stderr, "The number of arguments is incorrect.\n");
        exit(1);
    }

    FILE* input = fopen(argv[1], "r");
    FILE* output = fopen(argv[2], "w");

    char command[10];
    char variable[50];
    int position;
    
    fscanf(input, "%s", command);
    fscanf(input, "%s", variable);
    fprintf(output,"Command: %s %s\n", command, variable);

    //Initializing a new list.
    struct ComicList theList;
    initializeList(&theList);

    //Initializing the purchase list.
    struct ComicList buyList;
    initializeList(&buyList);

    //Calling load on the first file name.
    load(variable, output, &theList);

    while (!feof(input))
    {
        char command[10] = "\0";
        char variable[10] = "\0";
        position = 0;
        fscanf(input, "%s", command);
        if (strcmp(command, "load") == 0)
        {
            fscanf(input, "%s", variable);
            fprintf(output,"Command: load %s\n", variable);
            load(variable, output, &theList);
        }
        else if (strcmp(command, "display") == 0)
        {
            fprintf(output, "Command: %s\n", command);
            display(&theList, output);
        }
        else if (strcmp(command, "save") == 0)
        {
            
            fscanf(input, "%s", variable);
            fprintf(output, "Command: save %s\n", variable);
            save(theList, variable);
        }
        else if (strcmp(command, "clear") == 0)
        {
            fprintf(output, "Command: %s\n", command);
            clear(&theList);
        }
        else if (strcmp(command, "buy") == 0)
        {
            fscanf(input, "%d", &position);
            fprintf(output, "Command: %s %d\n", command, position);
            buy(theList, &buyList, position, output);
        }
        else if (strcmp(command, "checkout") == 0)
        {
            fprintf(output, "Command: %s\n", command);
            checkout(&buyList, output);
        }
        else if (strcmp(command, "find") == 0)
        {
            fscanf(input, "%d", &position);
            fprintf(output, "Command: %s %d\n", command, position);
            find(theList, position, output);
        }
        else if (strcmp(command, "remove") == 0)
        {
            fscanf(input, "%d", &position);
            fprintf(output, "Command: %s %d\n", command, position);
            bool removed = remover(&theList, position);
            if (removed)
            {
                fprintf(output, "Comic at index %d successfully removed\n", position);
            }
            else
            {
                fprintf(output, "Comic at index %d was not removed\n", position);
            }
        }
    }
    freeList(&theList);
    //free(buyList.data);
    //freeList(&buyList);
    fclose(input);
    fclose(output);
}