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

char* ReadLine(FILE*, bool);
void NaiveAlgorithm(char*, char*);
void KnuthMorrisPratt(char*, char*, sDynArray);
sDynArray ComputePrefixFunction(char*);

int main(){

    /* Initialize variables */
    char *text = NULL;
    char *pattern  = NULL;
    bool isReadText = false;
    bool isReadPattern = false;
    sDynArray prefix;    

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
            free(text);
            text = NULL;
            /*printf("text after free: %s size %lu\n", text, sizeof(text));*/
            text = ReadLine(stdin, isReadPattern); /* allocating memory here! */
            /*printf("Read text: ");*/
            /*printf("%s\n", text);*/
        }

        if(isReadPattern){
            free(pattern);
            pattern = NULL;
            pattern = ReadLine(stdin, isReadPattern); /* allocating memory here! */

            /*printf("Read pattern: ");*/
            /*printf("%s\n", pattern);*/

            if(readChar == 'N')
                NaiveAlgorithm(text, pattern);
            else if (readChar == 'K'){
                freeDynArray(&prefix);
                prefix = ComputePrefixFunction(pattern);
                KnuthMorrisPratt(text, pattern, prefix);
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
    freeDynArray(&prefix);

    return 0;
}

char* ReadLine(FILE *inputFile, bool isPattern){
    
    int textSize = 2;
    char *text = malloc(textSize*sizeof(char)); 
    int characterCounter = 0;
    bool isLastChar = false;
    int readChar = getchar(); /* white space */

    readChar = getchar();
    /*printf("readChar: %c\n", (char) readChar);*/
    if(readChar == EOF)
        return NULL;
    while(!isLastChar){
        if(readChar == '\n'){
            if(isPattern)
                readChar = '$';
            else
                readChar = '-'; /* character to end text */
            isLastChar = true;
            /*printf("last char\n");*/
        }
        /* read text */
        if(characterCounter == textSize){
            textSize *= 2;
            text = realloc(text, textSize*sizeof(char));
        }
        text[characterCounter] = (char) readChar;
        characterCounter++;
        if(!isLastChar){
            readChar = getchar();
            /*printf("readChar: %c\n", (char) readChar);*/
        }       
    }

    return text;

}

void NaiveAlgorithm(char* text, char* pattern){

    /*printf("-------------Naive algorithm----------------\n");*/

    /*printf("text: %s\n", text);*/
    /*printf("pattern: %s\n", pattern);*/

    int posStoreSize = 2;
    int *posStore = malloc(posStoreSize*sizeof(int));
    int t;
    int p;
    int matchCounter;
    int i;

    if(sizeof(text) < sizeof(pattern) || sizeof(text) < 1 || sizeof(pattern) < 1){
        /*printf("the size of the text is 0 or smaller than the size of the pattern, or the size of the pattern is 0\n");*/
        free(posStore);
        return;
    }

    matchCounter = 0;
    t = 0;
    p = 0;   
    while(text[t] != '-'){
        /*printf("text[t]: %c\n", text[t]);*/
        /*printf("pattern[p]: %c\n", pattern[p]);*/
        
        while(pattern[p] != '$' && text[t+p] != '-' && pattern[p] == text[t+p]){
            /*printf("match t: %d p: %d\n", t, p);*/
            p++;
        }
        if(pattern[p] == '$'){
            matchCounter++;
            if(posStoreSize == matchCounter){
                posStoreSize *= 2;
                posStore = realloc(posStore, posStoreSize*sizeof(int));
            }
            posStore[matchCounter-1] = t;
            t++;
            p = 0;
        }
        else if(pattern[p] != text[t+p]){
            /*printf("p: %d\n", p);*/
            p = 0;
            t++;
        }
    }

    /*printf("counter: %d\n", matchCounter);*/
    for(i = 0; i < matchCounter; i++){
        printf("%d", posStore[i]);
        if(i < matchCounter-1)
            printf(" ");
        else
            printf("\n");
    }

    free(posStore);
    posStore = NULL;


}

sDynArray ComputePrefixFunction(char* pattern){

    /*printf("computing prefix of %s\n", pattern);*/
    /* int p = 0;*/
    int q = 1;
    int k = -1;
    int patternSize = sizeof(pattern);
    /*printf("patternSize: %d\n", patternSize);*/
    sDynArray prefix;
    initDynArray(&prefix, patternSize);

    if(sizeof(pattern) < 1){
        freeDynArray(&prefix);
        return prefix;
    }

    insertElement(&prefix, 0);  
    while(pattern[q] != '$'){
        while(k >= 0 && pattern[k+1] != pattern[q]){
            k = prefix.array[k]-1;
            /*printf("pattern[k+1]: %c pattern[q]: %d\n", pattern[k+1],pattern[q]);*/
        }
        if (pattern[k+1] == pattern[q])
            k++;
        insertElement(&prefix, k+1);
        /*prefix.array[q] = k+1;*/
        q++;
    }

    return prefix;

}

void KnuthMorrisPratt(char* text, char* pattern, sDynArray prefix){

    /*printf("-------------KMP----------------\n");*/

    int posStoreSize = 2;
    int *posStore = malloc(posStoreSize*sizeof(int));
    int t = 0;
    int p = -1;
    int posCounter = 0;
    int i;
    int nComparisons = 0;

    if(sizeof(text) < sizeof(pattern) || sizeof(text) < 1 || sizeof(pattern) < 1){
        printf("the size of the text is 0 or smaller than the size of the pattern, or the size of the pattern is 0\n");
        return;
    }

    /*for(i = 0; i<prefix.used; i++){
        printf("pos %d: %d ", i, prefix.array[i]);
    }
    printf("\n");*/


    if(sizeof(pattern) < 1){
        free(posStore);
        posStore = NULL;
        return;
    }
    
    /*printf("pattern: %s\n", pattern);*/
    /*printf("text: %s\n", text);*/
    while(text[t] != '-'){
        /*printf("\np: %d, prefix[p+1]: %d\n", p, prefix.array[p+1]);*/
        /*printf("t: %d\n", t);*/
        /*printf("pattern[p+1]: %c\n", pattern[p+1]);*/
        /*printf("text[t]: %c\n", text[t]);*/
        /*printf("pattern[p+1] != text[t]: %d\n", pattern[p+1] != text[t]);*/
        /*printf("--------\n");*/
        /*printf("p >= 0 && pattern[p+1] != text[t]: %d\n", p >= 0 && pattern[p+1] != text[t]);*/
        /*printf("--------\n");*/
        /*printf("c");*/
        while(p >= 0 && pattern[p+1] != text[t]){
             /*printf("a");*/
             nComparisons++;
             p = prefix.array[p+1]-1;
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

        if(pattern[p+1] == '$'){
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
            p = prefix.array[p+1]-1;
            /*printf("m");*/
        }
        /*printf("n");*/
        t++;
    }
   
    for(i = 0; i < posCounter; i++){
        printf("%d", posStore[i]);
        if(i < posCounter-1)
            printf(" ");
        else
            printf("\n");
    }
    printf("%d \n", nComparisons);

    free(posStore);
    posStore = NULL;
    
     
}
