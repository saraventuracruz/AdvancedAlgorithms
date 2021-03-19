#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*to reset the char arrays*/
#include <stdbool.h> /* to use booleans */

char* ReadLine(FILE*, bool);
void NaiveAlgorithm(char*, char*);

int main(){

    /* Initialize variables */
    char *text = malloc(2*sizeof(char));
    char *pattern  = malloc(2*sizeof(char));
    bool isReadText = false;
    bool isReadPattern = false;    

    int readChar = getchar(); /* read input character by character */
    while(readChar != EOF){

        /* printf("readChar: %c\n", (char) readChar);*/
        isReadText = readChar == 'T';
        /*printf("isReadText %d\n", (int) isReadText);*/
        isReadPattern = readChar == 'N'|| readChar == 'K' || readChar == 'B';
        /*printf("isReadPattern %d\n", (int) isReadPattern);*/
        if(readChar == 'X')
            break;

        if(isReadText){
            free(text);
            /*printf("text after free: %s size %lu\n", text, sizeof(text));*/
            text = ReadLine(stdin, isReadPattern); /* allocating memory here! */
            /*printf("Read text: ");*/
            /*printf("%s\n", text);*/
        }

        if(isReadPattern){
            free(pattern);
            pattern = ReadLine(stdin, isReadPattern); /* allocating memory here! */

            /*printf("Read pattern: ");*/
            /*printf("%s\n", pattern);*/

            if(readChar == 'N')
                NaiveAlgorithm(text, pattern);
            /*else if (readChar == 'K')*/
                /*printf("---------------Knuth Morris Pratt-------------\n");*/
            /*else if(readChar == 'B')*/
                /*printf("----------------Boyer Moore-----------------\n");*/
            /*else*/
                /*printf("unexpected char: %c\n", (char)readChar);*/
        }

        readChar = getchar();
    }

    free(text);
    free(pattern);

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
    int t = 0;
    int p = 0;
    int counter = 0;

    if(sizeof(text) < sizeof(pattern) || sizeof(text) < 1 || sizeof(pattern) < 1){
        printf("the size of the text is 0 or smaller than the size of the pattern, or the size of the pattern is 0\n");
        return;
    }


    while(text[t] != '-'){
        /*printf("text[t]: %c\n", text[t]);*/
        /*printf("pattern[p]: %c\n", pattern[p]);*/
        
        while(pattern[p] != '$' && text[t+p] != '-' && pattern[p] == text[t+p]){
            /*printf("match p: %d\n", p);*/
            p++;
        }
        if(text[t+p] == '-')
            break;
        if(pattern[p] == '$'){
            counter++;
            if(posStoreSize == counter){
                posStoreSize *= 2;
                posStore = realloc(posStore, posStoreSize*sizeof(int));
            }
            posStore[counter-1] = t;
            t++;
            p = 0;
        }
        else if(pattern[p] != text[t+p]){
            /*printf("p: %d\n", p);*/
            p = 0;
            t++;
        }
    }

    int i;
    for(i = 0; i < counter; i++){
        printf("%d", posStore[i]);
        if(i < counter-1)
            printf(" ");
        else
            printf("\n");
    }

    free(posStore);


}