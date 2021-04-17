#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include <stdbool.h> /* to use booleans */

#define AlphabetSize 4 /* 4 letters on the considered alphabet*/

int ReadLine(FILE*, char*); /* to read lines from file (texts and patterns)*/

/* naive algorithm to find all the occurrences of a pattern in a text: 
compare character by character, rescanning the entire pattern after shifting one position 
whenever there is a mismatch or the end of the pattern is reached */
void NaiveAlgorithm(char*, int, char*, int); 

/* implementation of the knuth morris pratt algorithm to find all the occurrences of a pattern in a text*/
void KnuthMorrisPratt(char*, int, char*, int, int*);
/* preprocessing step of the knuth morris pratt algorithm */
void ComputePrefixFunction(char*, int, int*);

/* implementation of the boyer moore algorithm */
void BoyerMoore(char*, int, char*, int);
int BadCharacterRule(int**, char, int);
 /* preprocessing steps of the boyer moore algorithm */
void ZAlgorithm(char*, int, int*);
void PreprocessPattern(char*, int, int**, int*, int*);


/* auxiliary method to get the index of a char of the alphabet (considering the fixed alphabet: {A, C, T, G}) */
int GetIndexFromChar(char);


int main(){

    /* Initialize variables */
    char *text = malloc(2*sizeof(char));
    char *pattern  = malloc(2*sizeof(char));
    int *prefix = malloc(2*sizeof(int));
    bool isReadText = false;
    bool isReadPattern = false;
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
            textSize = ReadLine(stdin, text); /* allocating memory here! */
            /*printf("Read text: ");*/
            /*printf("%s\n", text);*/
        }

        if(isReadPattern){

            patternSize = ReadLine(stdin, pattern); /* allocating memory here! */

            /*printf("Read pattern: ");*/
            /*printf("%s\n", pattern);*/

            if(readChar == 'N')
                NaiveAlgorithm(text, textSize, pattern, patternSize);
            else if (readChar == 'K'){
                /*freeDynArray(&prefix);*/
                ComputePrefixFunction(pattern, patternSize, prefix);
                KnuthMorrisPratt(text, textSize, pattern, patternSize, prefix);
            }
            else if(readChar == 'B'){
                BoyerMoore(text, textSize, pattern, patternSize);
            }
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

int ReadLine(FILE *inputFile, char* text){
    
    int textSize = sizeof(text);
    int characterCounter = 0;
    int readChar = getchar(); /* white space */

    readChar = getchar();
    /*printf("readChar: %c\n", (char) readChar);*/
    if(readChar == EOF)
        return 0;
    while(readChar != '\n'){

        /* read text */
        if(characterCounter+1 == textSize){
            textSize *= 2;
            text = realloc(text, textSize*sizeof(char));
        }
        text[characterCounter] = (char) readChar;
        characterCounter++;
        /*printf("readChar: %c, counter: %d\n", (char) readChar, characterCounter);*/
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
        printf("\n"); /* the pattern never occurs on the text*/
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
            p++;
        }

        if(p == patternSize){
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

    for(i = 0; i < matchCounter; i++){
        printf("%d ", posStore[i]);
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
    prefix = realloc(prefix, patternSize*sizeof(int));

    if(patternSize <= 1){
        return;
    }

    prefix[0] = 0;
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

    int posStoreSize = 2; /* size of the array of occurrences positions */
    int *posStore = malloc(posStoreSize*sizeof(int)); /* to store the occurrences of pattern in text*/ 
    int t; /* position in text */
    int p; /* position in pattern */
    int i; /* iterator of positions list */ 
    int posCounter = 0; /* number of matches */
    int nComparisons = 0; /* number of comparisons */

    if(textSize < patternSize || textSize < 1 || patternSize < 1){
        printf("\n"); /* the pattern never occurs on the text*/
        printf("0 \n"); /* 0 comparisons */
        free(posStore);
        posStore = NULL;
        return;
    }
    
    /*printf("pattern: %s\n", pattern);*/
    /*printf("text: %s\n", text);*/
    p = -1;
    for(t = 0; t < textSize; t++){

        /*printf("\nt: %d ", t);*/
        while(p >= 0 && pattern[p+1] != text[t]){
            nComparisons++;
            p = prefix[p]-1;
        }
        if(p>= 0 && pattern[p+1] == text[t]){
            nComparisons++;
        }

        /*printf("c");*/
        if (pattern[p+1] == text[t]){
            nComparisons++;
            /*printf("d");*/
            p++;
            /*printf("e");*/
        }else{
            nComparisons++;
        }
        /* found a match */
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
        printf("%d ", posStore[i]);
    }
    printf("\n");
    printf("%d \n", nComparisons);

    free(posStore);
    posStore = NULL;
        
}

int FindZValue(char* pattern, int patternSize, int k){
/*returns the length of the longest prefix that matches the substring of pattern starting at k*/
    int i = 0;    
    if(pattern[i] != pattern[k]){
        return 0;
    }
    while(k+i < patternSize && pattern[i] == pattern[k+i]){
        i++;
    }
    return i-1; /* step one back (the current i was the one falsifying the condition)*/
}

void ZAlgorithm(char* pattern, int patternSize, int* zTable){

    
    int* rTable = malloc(patternSize*sizeof(int));
    int* lTable = malloc(patternSize*sizeof(int));
    int i = 0; /* pattern iterator*/
    int z = 0; /* z value*/
    int k = 0; /* position of beta at the zbox's corresponding prefix*/
    int betaSize = 0; /* size of beta - a substring of a zbox and its corresponding prefix*/
    zTable = realloc(zTable, patternSize*sizeof(int));
    zTable[0] = -1; /* undefined */
    rTable[0] = lTable[0] = 0;

    for(i = 1; i < patternSize; i++){
        /* case 1: i is outside a zbox*/
        if(i > rTable[i-1]){
            z = FindZValue(pattern, patternSize, i);
            zTable[i] = z;
            lTable[i] = i;
            rTable[i] = i + z;
        }
        /* case 2: i is inside a zbox*/
        else{
            k = i - lTable[i-1];
            betaSize = rTable[i-1]-i+1;
            /* case 2a*/
            if(zTable[k] < betaSize){
                zTable[i] = zTable[i];
                lTable[i] = lTable[i-1];
                rTable[i] = rTable[i-1];
            }
            /* case 2b*/
            else{
                z = FindZValue(pattern, patternSize, rTable[i-1]);
                zTable[i] = z;
                lTable[i] = i;
                rTable[i] = i + z;
            }
        }
    }

    free(lTable);
    free(rTable);
}

int GetIndexFromChar(char c){
    /*Given a char, returns its index at the bad character rule table*/
    
    int index = -1;
    
    switch (c)
    {
    case (char) 'A':
        index = 0;
        break;
    
    case (char) 'C':
        index = 1;
        break;
    
    case (char) 'T':
        index = 2;
        break;
    
    case (char) 'G':
        index = 3;
        break;
    
    default:
        printf("Character %c is not valid", c);
        break;
    }

    return index;
}

void PreprocessPattern(char* pattern, int patternSize, int **badCharTable, int* LTable, int* lTable){
    
    char* reversedPattern = malloc(patternSize*sizeof(int));
    int i;
    int j;
    int charIndex;
    int* ZTable = malloc(patternSize*sizeof(int));
    int* NTable = malloc(patternSize*sizeof(int));
    int nCharOcc[AlphabetSize]; /* to register the number of occurrences of a character in the pattern */ 

    /* initialize the bad character table */
    for(i = 0; i < AlphabetSize; i++){
        badCharTable[i] = malloc((patternSize+1) * sizeof(badCharTable[0]));
        nCharOcc[i] = 0;
    }

    /* fill the bad character table and save the reversed pattern */
    for(i = patternSize-1; i>= 0; i--){
        charIndex = GetIndexFromChar(pattern[i]);
        badCharTable[charIndex][nCharOcc[charIndex]] = i;
        nCharOcc[charIndex]++; /* update the number of occurrences of char in the pattern*/
        reversedPattern[patternSize-i-1] = pattern[i]; /* save the reversed pattern */
    }
    for(i = 0; i < AlphabetSize; i++){
        badCharTable[i][nCharOcc[i]] = -1; /* list of char occurrences' terminator */
    }

    /* find the Z values of the reversed pattern and fill the n table accordingly */
    ZAlgorithm(reversedPattern, patternSize, ZTable);
    for(i = 0; i < patternSize; i++){
        NTable[i] = ZTable[patternSize-1-i]; /* N_j(P) = Z_{n-1-j}(P^r) */
        LTable[i] = -1; /* initialize lTable*/
    }

    /* fill the lTable*/ 
    for(j = 0; j < patternSize-1; j++){
        i = patternSize - NTable[j];
        LTable[i] = j;
        /*lTable[i];*/ /*TODO: fill the small l table*/
    }

    free(ZTable);
    free(NTable);
    free(reversedPattern);
}

int BadCharacterRule(int** badCharTable, char characterInText, int positionInPattern){

    int charIndex = GetIndexFromChar(characterInText);
    int i = 0;
    int charRightMostPosition = 1;

    /*assuming there is a negative terminator after the last position of the character in the pattern*/
    while(badCharTable[charIndex][i] >= 0 && badCharTable[charIndex][i] >= positionInPattern){
        i++;
    }
    if(badCharTable[charIndex][i] >= 0 && badCharTable[charIndex][i] < positionInPattern){
        charRightMostPosition = badCharTable[charIndex][i-1];
    }

    return charRightMostPosition;
}

void BoyerMoore(char* text, int textSize, char* pattern, int patternSize){

    int **badCharTable = malloc(AlphabetSize * sizeof(*badCharTable)); /* for the bad character rule */
    

    int *LTable = malloc(patternSize*sizeof(int)); /* for the (strong) good suffix rule*/
    int *lTable = malloc(patternSize*sizeof(int)); /* for the good suffix rule */
    /* int i;*/

    /*PreprocessPattern(pattern, patternSize, badCharTable, LTable, lTable);*/
    printf("\n");
    printf("0 \n");




    /*for(i = 0; i < AlphabetSize; i++){*/
        /*free(badCharTable[i]);*/
    /*}*/

    free(badCharTable);
    free(LTable);
    free(lTable);
}
