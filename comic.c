
// As a Hokie, I will conduct myself with honor and integrity at all times.  
// I will not lie, cheat, or steal, nor will I accept the actions of those who do.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "comic.h"

/*This is the load method. It will take 
the name of the file, and parse the data
inside of it. After that, it will store 
this data inside a Comic struct, which
will be placed inside an array.*/
void load(char* input, FILE* output, struct ComicList* theList)
{
    FILE* in = fopen(input, "r");
    
    char date[150] = "\0";
    char code[150] = "\0";
    char publisher[150] = "\0";
    char title[150] = "\0";
    char price[150] = "\0";
    int counter = 0;

    while (!feof(in))
    {
        scanFile(in, date, code, publisher, title, price);
        
        if ((strcmp(date, "DATE") != 0) && (strlen(date) > 2))
        {
            struct Comic newComic;
            initializeComic(&newComic);
            update(&newComic, date, code, publisher, title, price);
            add(theList, newComic);
            counter++;
        }
    }
    fprintf(output, "      Number of comics: %d\n", counter);
    fclose(in);
}

/*This is the add method, where a Comic struct
will be added to the list of comics.*/
void add(struct ComicList* theList, struct Comic theComic)
{
    int trace = (*theList).count;
    if (trace == (*theList).size)
    {
        expand(theList, ((*theList).size) * 2);
        (*theList).size = (*theList).size * 2;
    }
    //struct Comic newComic;
    (*theList).data[trace] = theComic;
    (*theList).count++;
}

/*This is the expand method, where the size
of the list will be doubled if it gets full.*/
void expand(struct ComicList* list, int newSize)
{
    (*list).data = realloc((*list).data, sizeof(struct Comic) * newSize);
}

/*This is the display method, where the information
about all the comic in the list will be printed in
a specific format.*/
void display(struct ComicList* theList, FILE* output)
{
    if ((*theList).count == 0)
    {
        fprintf(output, "   List is currently empty.\n");
    }
    for (int x = 0; x < (*theList).count; x++)
    {
        fprintf(output, "Comic Number: %d\n", x + 1);
        fprintf(output, "      Date: %s\n", (*theList).data[x].date);
        fprintf(output, "      Code: %s\n", (*theList).data[x].code);
        fprintf(output, "      Publisher: %s\n", (*theList).data[x].publisher);
        fprintf(output, "      Title: %s\n", (*theList).data[x].title);
        fprintf(output, "      Cost: %s\n", (*theList).data[x].cost);
    }
}

/*This is the save method, where all the comics
inside the main list will be saved into the
file with the specified name. The new file
will contain the information while maintaining
the same structure of the original file.*/
void save(struct ComicList list, char* output)
{
    FILE* out = fopen(output, "w");
    fprintf(out, "DATE,CODE,PUBLISHER,TITLE,PRICE\n");
    for (int x = 0; x < (list).count; x++)
    {
        fprintf(out, "%s,%s,%s,%s,%s\n", (list).data[x].date, (list).data[x].code, (list).data[x].publisher, (list).data[x].title, (list).data[x].cost);
    }
    fclose(out);
}

/*This is the clear method, where everything
in the list will be removed. This basically works
by calling the remove method on the first position
until the list is empty.*/
void clear(struct ComicList* list)
{
    int trace = (*list).count;
    
    for (int x = trace; x > 0; x--)
    {
        freeComic(&(*list).data[x - 1]);
    }
}

/*This is the buy method, where the specified comic
will be deep copied from the original list into
the purchase lise.*/
void buy(struct ComicList list, struct ComicList* buyList, int position, FILE* output)
{
    if (position < 0 || position >= (list).count)
    {
        fprintf(output, "Unable to buy comic #%d\n", position);
    }
    else
    {
        struct Comic newComic;
        initializeComic(&newComic);
        update(&newComic, (list).data[position].date, (list).data[position].code, 
             (list).data[position].publisher, (list).data[position].title, (list).data[position].cost);
        // strcpy((newComic).date, (list).data[position].date);
        // strcpy((newComic).code, (list).data[position].code);
        // strcpy((newComic).publisher, (list).data[position].publisher);
        // strcpy((newComic).title, (list).data[position].title);
        // strcpy((newComic).cost, (list).data[position].cost);
        
        add(buyList, newComic);
        fprintf(output, "Comic #%d added to purchase list\n", position);
    }
}

void updatePurchased(struct Comic* comic, char* date, char* code, char* publisher, char* title, char* price)
{
    (*comic).date = calloc(strlen(date) + 1, sizeof(char));
    (*comic).code = calloc(strlen(code) + 1, sizeof(char));
    (*comic).publisher = calloc(strlen(publisher) + 1, sizeof(char));
    (*comic).title = calloc(strlen(title) + 1, sizeof(char));
    (*comic).cost = calloc(strlen(price) + 1, sizeof(char));

    strcpy((*comic).date, date);
    strcpy((*comic).code, code);
    strcpy((*comic).publisher, publisher);
    strcpy((*comic).title, title);
    strcpy((*comic).cost, price);
}

/*This is the remove method, where the the comic
at the specified position will be removed from the list.*/
bool remover(struct ComicList* list, int position)
{
    if (position < 0 || position >= (*list).count)
    {
        return false;
    }
    freeComic(&(list)->data[position]);
    for (int x = position; x < (*list).count - 1; x++)
    {
        (*list).data[x] = (*list).data[x + 1];
    }
    (*list).count--;
    return true;
}

/*This is the checkout method, where all the comics
inside the purchase list will be cleared. The total will
be displayed after calculating the tax percentage.*/
void checkout(struct ComicList* buyList, FILE* output)
{
    double subtotal = 0;
    fprintf(output, "Comics in Purchase List\n");
    display(buyList, output);
    for (int x = 0; x < (*buyList).count; x++)
    {
        if (strcmp((*buyList).data[x].cost, "AR") != 0)
        {
            (*buyList).data[x].cost = (*buyList).data[x].cost + 1;
            double price = atof((*buyList).data[x].cost);
            subtotal += price;
        }
    }
    fprintf(output, " Subtotal: $%.2f\n", subtotal);
    double tax = subtotal;
    tax = tax * 0.05;
    fprintf(output, "      Tax: $%.2f\n", tax);
    fprintf(output, "    Total: $%.2f\n", tax + subtotal);
    clear(buyList);
    
}

/*This is the find method, where we'll be looking
through the list and display the comic that is at
the specified position.*/
void find(struct ComicList theList, int position, FILE* output)
{
    if (position < 0 || position >= (theList).count)
    {
        fprintf(output, "There is no comic at index #%d in the list.\n", position);
    }
    else
    {
        fprintf(output, "      Date: %s\n", (theList).data[position].date);
        fprintf(output, "      Code: %s\n", (theList).data[position].code);
        fprintf(output, "      Publisher: %s\n", (theList).data[position].publisher);
        fprintf(output, "      Title: %s\n", (theList).data[position].title);
        fprintf(output, "      Cost: %s\n", (theList).data[position].cost);
    }
}

/*This method initializes the Comic struct.*/
void initializeComic(struct Comic* comic)
{
    (*comic).date = calloc(100, sizeof(char));
    (*comic).code = calloc(100, sizeof(char));
    (*comic).publisher = calloc(100, sizeof(char));
    (*comic).title = calloc(100, sizeof(char));
    (*comic).cost = calloc(100, sizeof(char));
}

/*This method initializes the ComicList struct.*/
void initializeList(struct ComicList* list)
{
    (*list).data = calloc(10, sizeof(struct Comic));
    (*list).size = 10;
    (*list).count = 0;
}

/*This method updates the values of the Comic by reallocating 
memory for each part.*/
void update(struct Comic* newComic, char* date, char* code, char* publisher, char* title, char* price)
{
    (*newComic).date = realloc((*newComic).date, sizeof(char) * (strlen(date) + 1));
    (*newComic).code = realloc((*newComic).code, sizeof(char) * (strlen(code) + 1));
    (*newComic).publisher = realloc((*newComic).publisher, sizeof(char) * (strlen(publisher) + 1));
    (*newComic).title = realloc((*newComic).title, sizeof(char) * (strlen(title) + 1));
    (*newComic).cost = realloc((*newComic).cost, sizeof(char) * (strlen(price) + 1));

    strcpy((*newComic).date, date);
    strcpy((*newComic).code, code);
    strcpy((*newComic).publisher, publisher);
    strcpy((*newComic).title, title);
    strcpy((*newComic).cost, price);
}

/*This method scans the specified file and 
assigns certain values to hold the information.*/
void scanFile(FILE* input, char* date, char* code, char* publisher, char* title, char* price)
{
    date[0] = '\0';
    code[0] = '\0';
    publisher[0] = '\0';
    title[0] = '\0';
    price[0] = '\0';

    fscanf(input, "%[^,]", date);
    fgetc(input);

    fscanf(input, "%[^,]", code);
    fgetc(input);

    fscanf(input, "%[^,]", publisher);
    fgetc(input);

    fscanf(input, "%[^,]", title);
    fgetc(input);

    fscanf(input, "%[^\n]", price);
    fgetc(input);
}

/*This method frees the dynamically
allocated memory for the Comic struct.*/
void freeComic(struct Comic* comic)
{
    (*comic).date[0] = '\0';
    (*comic).code[0] = '\0';
    (*comic).publisher[0] = '\0';
    (*comic).title[0] = '\0';
    (*comic).cost[0] = '\0';
    free((*comic).date);
    free((*comic).code);
    free((*comic).publisher);
    free((*comic).title);
    free((*comic).cost);
}

/*This methid frees the dynamically
allocated memory for the ComicList struct.*/
void freeList(struct ComicList* list)
{
    for (int x = 0; x < (*list).count; x++)
    {
        freeComic(&(list)->data[x]);
    }
    free((*list).data);
    //list->data = NULL;
}











            //freeComic((*list).data[x]);
            //free((*list).data[x].date);
            //free((*list).data[x].code);
            //free((*list).data[x].publisher);
            //free((*list).data[x].title);
            //free((*list).data[x].cost);

            //list->data[x].date = NULL;
            //list->data[x].code = NULL;
            //list->data[x].publisher = NULL;
            //list->data[x].title = NULL;
            //list->data[x].cost = NULL;

        //free((*list).data[position].date);
        //free((*list).data[position].code);
        //free((*list).data[position].publisher);
        //free((*list).data[position].title);
        //free((*list).data[position].cost);

        //list->data[position].date = NULL;
        //list->data[position].code = NULL;
        //list->data[position].publisher = NULL;
        //list->data[position].title = NULL;
        //list->data[position].cost = NULL;
        //freeComic((*list).data[position]);
