#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include <string.h> /*to reset the char arrays*/
#include <stdbool.h> /* to use booleans */

char* ReadLine(FILE*, bool);

int main(){

    /* Initialize variables */
    /* int textSize = 2; */
    /* int patternSize = 2; */
    char *text = malloc(2*sizeof(char));
    char *pattern  = malloc(2*sizeof(char));
    /* int textPosition = 0;*/ /* position of characters in the text */
    /* int patternCounter = 0;*/ /* the number of characters in the pattern */
    bool isReadText = false;
    bool isReadPattern = false;
    /*    bool isNewLine = false; */
    /* bool isLastChar = false; */
    

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
            
	  /* readChar = getchar();
            // if(readChar == EOF)
	    //     break;
            // free(text);
            // textPosition = 0;
            // textSize = 2; 
            // text = malloc(textSize*sizeof(char));
            // while(!isLastChar){
            //     if(readChar == '\n'){
            //         readChar = '-';
            //         isLastChar = true;
            //     }
            //    
            //     if(textPosition == textSize){
            //         textSize *= 2;
            //         text = realloc(text, textSize*sizeof(char));
            //     }
            //     text[textPosition] = (char) readChar;
            //     textPosition++;
            //     if(!isLastChar)
            //         readChar = getchar();
            //     printf("readChar: %c\n", (char) readChar);

            // }*/

            free(text);
            text = ReadLine(stdin, isReadPattern);
            printf("Read text: ");
            printf("%s\n", text);
        }

        if(isReadPattern){
            free(pattern);
            pattern = ReadLine(stdin, isReadPattern);
            printf("Read pattern: ");
            printf("%s\n", pattern);
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
    printf("readChar: %c\n", (char) readChar);
    if(readChar == EOF)
        return NULL;
    while(!isLastChar){
        if(readChar == '\n'){
            if(isPattern)
                readChar = '$';
            else
                readChar = '-'; /* character to end text */
            isLastChar = true;
            printf("last char\n");
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
            printf("readChar: %c\n", (char) readChar);
        }       
    }

    return text;

}
