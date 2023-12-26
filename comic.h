// As a Hokie, I will conduct myself with honor and integrity at all times.  
// I will not lie, cheat, or steal, nor will I accept the actions of those who do.
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct Comic
{
    char* date;
    char* code;
    char* publisher;
    char* title;
    char* cost;
};

struct ComicList
{
    struct Comic* data;
	int size;
	int count;
};

void addComic(struct ComicList* list, struct Comic* comic);
void updatePurchased(struct Comic* newComic,char* date, char* code, char* publisher, char* title, char* price);
void initializeComic(struct Comic* comic);
void initializeList(struct ComicList* list);
void load(char* input, FILE* output, struct ComicList* theList);
void add(struct ComicList* list, struct Comic comic);
void expand(struct ComicList* list, int newSize);
void display(struct ComicList* list, FILE* output);
void save(struct ComicList list, char* output);
void clear(struct ComicList* list);
void buy(struct ComicList list, struct ComicList* buy, int position, FILE* output);
bool remover(struct ComicList* list, int position);
void shiftBackward(struct ComicList* list, int index);
void checkout(struct ComicList* buyList, FILE* output);
void find(struct ComicList list, int position, FILE* output);
void scanFile(FILE* input, char* date, char* code, char* publisher, char* title, char* price);
void update(struct Comic* newComic, char* date, char* code, char* publisher, char* title, char* price);
void freeComic(struct Comic* comic);
void freeList(struct ComicList* list);
