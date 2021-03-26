#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*to reset the char arrays*/
#include <stdbool.h> /* to use booleans */



/* define a dynamic array */
typedef struct {
  int *array;
  size_t used;
  size_t size;
} sDynArray;

void initDynArray(sDynArray *dArr, size_t initialSize) {
  dArr->array = malloc(initialSize * sizeof(int));
  dArr->used = 0;
  dArr->size = initialSize;
}

void insertElement(sDynArray *dArr, int element) {
  if (dArr->used == dArr->size) {
    dArr->size *= 2;
    dArr->array = realloc(dArr->array, dArr->size * sizeof(int));
  }
  dArr->used++;
  dArr->array[dArr->used] = element;
}

void freeDynArray(sDynArray *dArr) {
  free(dArr->array);
  dArr->array = NULL;
  dArr->used = 0;
  dArr->size = 0;
}

int ReadLine(FILE*, bool, char*);
void NaiveAlgorithm(char*, int, char*, int);
void KnuthMorrisPratt(char*, int, char*, int, int*);
void ComputePrefixFunction(char*, int, int*);

int main(){

    /* Initialize variables */
    char *text = malloc(2*sizeof(char));
    char *pattern  = malloc(2*sizeof(char));
    bool isReadText = false;
    bool isReadPattern = false;
    int *prefix = malloc(2*sizeof(int));
    int textSize = 0;
    int patternSize = 0;    

    char readChar = getchar(); /* read input character by character */
    while(readChar != EOF){

        /*printf("readChar: %c\n", (char) readChar);*/
        isReadText = readChar == 'T';
        /*printf("isReadText %d\n", (int) isReadText);*/
        isReadPattern = readChar == 'N'|| readChar == 'K' || readChar == 'B';
        /*printf("isReadPattern %d\n", (int) isReadPattern);*/
        if(readChar == 'X')
            break;

        if(isReadText){

            /*printf("text after free: %s size %lu\n", text, sizeof(text));*/
            textSize = ReadLine(stdin, false, text); /* allocating memory here! */
            /*printf("Read text: ");*/
            /*printf("%s\n", text);*/
        }

        if(isReadPattern){

            patternSize = ReadLine(stdin, isReadPattern, pattern); /* allocating memory here! */

            /*printf("Read pattern: ");*/
            /*printf("%s\n", pattern);*/

            if(readChar == 'N')
                NaiveAlgorithm(text, textSize, pattern, patternSize);
            else if (readChar == 'K'){
                /*freeDynArray(&prefix);*/
                ComputePrefixFunction(pattern, patternSize, prefix);
                KnuthMorrisPratt(text, textSize, pattern, patternSize, prefix);
            }
            /*else if(readChar == 'B')*/
                /*printf("----------------Boyer Moore-----------------\n");*/
            /*else*/
                /*printf("unexpected char: %c\n", (char)readChar);*/
        }

        readChar = getchar();
    }

    free(text);
    free(pattern);
    free(prefix);

    return 0;
}

int ReadLine(FILE *inputFile, bool isPattern, char* text){
    
    int textSize = sizeof(text);
    int characterCounter = 0;
    int readChar = getchar(); /* white space */

    readChar = getchar();
    /*printf("readChar: %c\n", (char) readChar);*/
    if(readChar == EOF)
        return 0;
    while(readChar != '\n'){

        /* read text */
        if(characterCounter == textSize){
            textSize *= 2;
            text = realloc(text, textSize*sizeof(char));
        }
        text[characterCounter] = (char) readChar;
        characterCounter++;
        /*printf("readChar: %c, counter: %d\n", (char) readChar, characterCounter); */
        readChar = getchar();
              
    }

    return characterCounter;

}

void NaiveAlgorithm(char* text, int textSize, char* pattern, int patternSize){

    int posStoreSize = 2;
    int *posStore = malloc(posStoreSize*sizeof(int));
    int t;
    int p;
    int matchCounter;
    int i;

    if(textSize < patternSize || textSize < 1 || patternSize < 1){
        /*printf("the size of the text is 0 or smaller than the size of the pattern, or the size of the pattern is 0\n");*/
        free(posStore);
        posStore = NULL;
        return;
    }

    matchCounter = 0;
    p = 0;   
    for(t = 0; t < textSize; t++){
        /*printf("text[t]: %c\n", text[t]);*/
        /*printf("pattern[p]: %c\n", pattern[p]);*/
        
        while(p < patternSize && t+p < textSize && pattern[p] == text[t+p]){
            /*printf("match t: %d p: %d\n", t, p);*/
            p++;
        }
        if(p == patternSize-1){
            matchCounter++;
            if(posStoreSize == matchCounter){
                posStoreSize *= 2;
                posStore = realloc(posStore, posStoreSize*sizeof(int));
            }
            posStore[matchCounter-1] = t;
            /*t++;*/
            p = 0;
        }
        else if(pattern[p] != text[t+p]){
            /*printf("p: %d\n", p);*/
            p = 0;
            /*t++;*/
        }
    }

    /*printf("counter: %d\n", matchCounter);*/
    for(i = 0; i < matchCounter; i++){
        printf("%d", posStore[i]);
        if(i < matchCounter-1)
            printf(" ");
    }

    printf("\n");

    free(posStore);
    posStore = NULL;


}

void ComputePrefixFunction(char* pattern, int patternSize, int* prefix){

    /*printf("computing prefix of %s\n", pattern);*/
    /* int p = 0;*/
    int q = 1;
    int k = 0;
    prefix = realloc(prefix, patternSize);
    /*printf("patternSize: %d\n", patternSize);*/
    /*sDynArray prefix;*/
    /*initDynArray(&prefix, patternSize);*/

    if(patternSize <= 1){
        /*freeDynArray(&prefix);*/
        return;
    }

    prefix[0] = 0;
    /*insertElement(&prefix, 0);  */
    for(q = 1; q < patternSize; q++){

        k = prefix[q-1];
        while (k > 0 && pattern[q] != pattern[k])
            k = prefix[k-1];
        if (pattern[q] == pattern[k])
            k++;
        prefix[q] = k;

    }

    return;

}

void KnuthMorrisPratt(char* text, int textSize, char* pattern, int patternSize, int* prefix){

    /*printf("-------------KMP----------------\n");*/

    int posStoreSize = 2;
    int *posStore = malloc(posStoreSize*sizeof(int));
    int t;
    int p;
    int posCounter = 0;
    int i;
    int nComparisons = 0;

    if(textSize < patternSize || textSize < 1 || patternSize < 1){
        /*printf("the size of the text is 0 or smaller than the size of the pattern, or the size of the pattern is 0\n");*/
        printf("\n"); /* the pattern never occurs on the text*/
        printf("0\n"); /* 0 comparisons */
        free(posStore);
        posStore = NULL;
        return;
    }


    if(patternSize < 1){
        free(posStore);
        posStore = NULL;
        return;
    }
    
    /*printf("pattern: %s\n", pattern);*/
    /*printf("text: %s\n", text);*/


    p = -1;
    for(t = 0; t < textSize; t++){

        while(p >= 0 && pattern[p+1] != text[t]){
             /*printf("a");*/
             nComparisons++;
             p = prefix[p]-1;
             /*printf("b");*/
         }
         if(p>= 0 && pattern[p+1] == text[t])
            nComparisons++;
        /*printf("c");*/
        if (pattern[p+1] == text[t]){
            /*printf("d");*/
            p++;
            nComparisons++;
            /*printf("e");*/
        }else{
            /*printf("g");*/
            nComparisons++;
        }

        if(p+1 == patternSize){
            /*printf("h");*/
            posCounter++;
            if(posStoreSize == posCounter){
                /*printf("i");*/
                posStoreSize *= 2;
                posStore = realloc(posStore, posStoreSize*sizeof(int));
                /*printf("j");*/
            }
            /*printf("k");*/
            posStore[posCounter-1] = t-p;
            /*printf("l");*/
            p = prefix[p]-1;
            /*printf("m");*/
        }
        /*printf("n");*/
    }
   
    for(i = 0; i < posCounter; i++){
        printf("%d", posStore[i]);
        if(i < posCounter-1)
            printf(" ");
    }
    printf("\n");
    printf("%d \n", nComparisons);

    free(posStore);
    posStore = NULL;
    
     
}
