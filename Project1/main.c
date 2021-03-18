#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*to reset the char arrays*/
#include <stdbool.h> /* to use booleans */

int main(){

    /* Initialize variables */
    int textSize = 2;
    int patternSize = 2;
    char *text = malloc(textSize*sizeof(char));
    char *pattern = malloc(patternSize*sizeof(char));
    int textPosition = 0; /* position of characters in the text */
    int patternCounter = 0; /* the number of characters in the pattern */
    bool isReadText = false;
    bool isReadPattern = false;
    bool isNewLine = false;
    bool isLastChar = false;

    int readChar = getchar(); /* read input character by character */
    while(EOF != readChar){

        printf("readChar: %c\n", (char) readChar);
        isReadText = readChar == 'T';
        printf("isReadText %d\n", (int) isReadText);
        isReadPattern = readChar == 'N'|| readChar == 'K' || readChar == 'B';
        printf("isReadPattern %d\n", (int) isReadPattern);
        if(readChar == 'X')
            break;

        if(isReadText){
            /* reset text */
            readChar = getchar(); /* space */
            if(readChar == EOF) 
                break;
            free(text); /* reset text */
            textPosition = 0;
            textSize = 2; 
            text = malloc(textSize*sizeof(char)); /* another option would be to reuse the previous structure: this should be done carefully!*/
            while(!isLastChar){
                if(readChar == '\n'){
                    readChar = '$';
                    isLastChar = true;
                }
                /* read text */
                if(textPosition == textSize){
                    textSize *= 2;
                    text = realloc(text, textSize*sizeof(char));
                }
                text[textPosition] = (char) readChar;
                textPosition++;
                if(!isLastChar)
                    readChar = getchar();

            }
        }
        readChar = getchar();

    }

    free(text);
    free(pattern);

    return 0;
}