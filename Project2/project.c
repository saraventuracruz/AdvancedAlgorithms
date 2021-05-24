#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include <stdbool.h> /* to use booleans */

int CharToInt(char);

int main(){

    /* Initialize variables */
    int numberOfStrings = 0; /* number of strings */ 
    char **setOfStrings = NULL; /* to store a set of strings */
    int s = 0; /* string index */
    int stringSize = 0; /* size of a string*/
    int c = 0; /* position of a character in a string */
    char readChar = getchar(); /* read input character by character */
    

    while(readChar != '\n'){
        numberOfStrings = numberOfStrings*10+CharToInt(readChar);
        readChar = getchar();
    }       
    printf("\nnumberOfStrings: %d", numberOfStrings);
    setOfStrings = (char**) malloc(numberOfStrings*sizeof(char*));
    /* read and store each string*/
    for(s = 0; s < numberOfStrings; s++){
        printf("\ns: %d", s);
        readChar = getchar();
        stringSize = 0;
        printf("\nstringSize: %d", stringSize);
        while(readChar != ' '){
            stringSize = stringSize*10+CharToInt(readChar);
            readChar = getchar();
        }
        printf("\nStringSize: %d", stringSize);
        setOfStrings[s] = (char*) malloc(stringSize*sizeof(char));
	if(setOfStrings[s] == NULL){
	  printf("\nerror while allocating memory setOfStrings[%d]", s);
	}
	for(c = 0; c < stringSize; c++){
            setOfStrings[s][c] = getchar();
        }
        printf("\nstring: %s", setOfStrings[s]);
        readChar = getchar(); /* new line character */
    }


    for(s = 0; s < numberOfStrings; s++){
        free(setOfStrings[s]);
        setOfStrings[s] = NULL;
    }
    free(setOfStrings);
    setOfStrings = NULL;
}

int CharToInt(char c){

    int digit = 0;

    switch (c)
    {
    case '0':
        digit = 0;
        break;
    case '1':
        digit = 1;
        break;
    case '2':
        digit = 2;
        break;
    case '3':
        digit = 3;
        break;
    case '4':
        digit = 4;
        break;
    case '5':
        digit = 5;
        break;
    case '6':
        digit = 6;
        break;
    case '7':
        digit = 7;
        break;
    case '8':
        digit = 8;
        break;
    case '9':
        digit = 9;
        break;    
    default:
        break;
    }
    return digit;
}
