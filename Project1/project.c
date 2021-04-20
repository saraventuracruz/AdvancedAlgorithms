#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include <stdbool.h> /* to use booleans */

#define AlphabetSize 4 /* 4 letters on the considered alphabet*/
#define max(a,b) (a<b) ? b : a

int ReadLine(FILE*, char**); /* to read lines from file (texts and patterns)*/

bool EqualCharQ(char, char, int*);

/* naive algorithm to find all the occurrences of a pattern in a text: 
compare character by character, rescanning the entire pattern after shifting one position 
whenever there is a mismatch or the end of the pattern is reached */
void NaiveAlgorithm(char*, int, char*, int); 

/* implementation of the knuth morris pratt algorithm to find all the occurrences of a pattern in a text*/
void KnuthMorrisPratt(char*, int, char*, int, int*);
/* preprocessing step of the knuth morris pratt algorithm */
void ComputePrefixFunction(char*, int, int**);

/* implementation of the boyer moore algorithm */
void BoyerMoore(char*, int, char*, int);
int BadCharacterRule(int*, char, int);
int GoodSuffixRule(int*, int*, int, int);

 /* preprocessing steps of the boyer moore algorithm */
void ZAlgorithm(char*, int, int**);
void PreprocessPattern(char*, int, int**, int**, int**, int**);



/* auxiliary method to get the index of a char of the alphabet (considering the fixed alphabet: {A, C, T, G}) */
int GetIndexFromChar(char);


int main(){

    /* Initialize variables */
    char *text = malloc(2*sizeof(char));
    char *pattern = malloc(2*sizeof(char));
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

            textSize = ReadLine(stdin, &text); /* allocating memory here! */
            /*printf("Read text: ");*/
            /*printf("%s\n", text);*/
        }

        if(isReadPattern){

            patternSize = ReadLine(stdin, &pattern); /* allocating memory here! */

            /*printf("Read pattern: ");*/
            /*printf("%s\n", pattern);*/

            if(readChar == 'N')
                NaiveAlgorithm(text, textSize, pattern, patternSize);
            else if (readChar == 'K'){
                ComputePrefixFunction(pattern, patternSize, &prefix);
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
    text = NULL;
    free(pattern);
    pattern = NULL;
    free(prefix);
    prefix = NULL;
 
    return 0;
}

int ReadLine(FILE *inputFile, char** text){

    int characterCounter = 0;
    int readChar = getchar(); /* white space */
    
    int textSize = 2; /* to keep track of the size of *text*, which is a dynamic array of chars */
    *text = realloc(*text,textSize*sizeof(char)); /* realloc memory to save the read chars */
    
    readChar = getchar();
    if(readChar == EOF)
        return 0;
    while(readChar != '\n'){
        characterCounter++;
        /* read text */
        if(characterCounter== textSize){
            textSize *= 2;
            *text = realloc(*text, textSize*sizeof(char));
        }
        (*text)[characterCounter-1] = (char) readChar;
        readChar = getchar();
              
    }

    return characterCounter;

}

bool EqualCharQ(char p, char t, int* nComparisons){

    *nComparisons = *nComparisons+1;
    return p == t;

}

void NaiveAlgorithm(char* text, int textSize, char* pattern, int patternSize){

    int posStoreSize = 2; /* size of positions storage */
    int *posStore = malloc(posStoreSize*sizeof(int)); /* to store the positions where the pattern occurs in text*/ 
    int t; /* text iterator */
    int p; /* pattern iterator */
    int matchCounter; /* count occurrences of pattern in text*/
    int i; /* position counter iterator */

    if(textSize < patternSize || textSize < 1 || patternSize < 1){
        printf("\n"); /* the pattern never occurs on the text*/
        free(posStore);
        posStore = NULL;
        return;
    }

    matchCounter = 0;
    p = 0;   
    for(t = 0; t+p < textSize; t++){
        
        while(p < patternSize && t+p < textSize && pattern[p] == text[t+p]){
            p++;
        }
        
        /* whenever p reaches the patternSize, an occurrence of the pattern has been found at position t of the text */
        if(p == patternSize){
            matchCounter++;
            if(posStoreSize == matchCounter){
                posStoreSize *= 2;
                posStore = realloc(posStore, posStoreSize*sizeof(int));
            }
            posStore[matchCounter-1] = t;
            p = 0;
        }
        else if(pattern[p] != text[t+p]){
            p = 0;
        }
    }

    for(i = 0; i < matchCounter; i++){
        printf("%d ", posStore[i]);
    }

    printf("\n");

    free(posStore);
    posStore = NULL;
}

void ComputePrefixFunction(char* pattern, int patternSize, int** prefix){

    int q = 1; /* point to the second position of the pattern: propper suffix*/
    int k = -1; /* point to the position "right before" the pattern */
    *prefix = realloc(*prefix, patternSize*sizeof(int));

    (*prefix)[0] = 0;
    for(q = 1; q < patternSize; q++){
        /*k = prefix[q-1];*/
        while (k >= 0 &&  pattern[k+1] != pattern[q])
            k = (*prefix)[k]-1;
        if (pattern[k+1] == pattern[q])
        /* if k might move forward */
            k++;
        /* since in C the positions begin at 0, the length is given by the position +1*/
        (*prefix)[q] = k+1;
    }
    return;
}

void KnuthMorrisPratt(char* text, int textSize, char* pattern, int patternSize, int* prefix){

    int posStoreSize = 2; /* size of the array of the occurrences' positions */
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

    p = -1;
    for(t = 0; t < textSize; t++){

        while(p >= 0 && !EqualCharQ(pattern[p+1], text[t], &nComparisons)){
            p = prefix[p]-1;
        }

        if (EqualCharQ(pattern[p+1],text[t], &nComparisons)){
            p++;
        }
        /* found a match */
        if(p+1 == patternSize){
            posCounter++;
            if(posCounter == posStoreSize){
                /* grow the positions storage*/
                posStoreSize *= 2;
                posStore = realloc(posStore, posStoreSize*sizeof(int));
            }
            /* save position */
            posStore[posCounter-1] = t-p;

            /* update p */
            p = prefix[p]-1;
            
        }
    }
    /* end of text */
   
    /* print positions and number of comparisons*/
    for(i = 0; i < posCounter; i++){
        printf("%d ", posStore[i]);
    }
    printf("\n");
    printf("%d \n", nComparisons);

    free(posStore);
    posStore = NULL;       
}

void ZAlgorithm(char* pattern, int patternSize, int** zTable){
  
    int i = 0; /* pattern iterator*/
    int z = 0; /* z value*/
    int r = 0; /* position of the right limit of a z-box*/
    int l = 0; /* position of the left limit of a z-box*/
    int k = 0; /* position of beta at the z-box's corresponding prefix*/
    int betaSize = 0; /* size of beta - a substring of a z-box and its corresponding prefix*/

    *zTable = realloc(*zTable, patternSize*sizeof(int));
    (*zTable)[0] = -1; /* undefined */
    /* initialize zTable */
    for(i = 1; i < patternSize; i++){
        (*zTable)[i] = 0;
    }

    for(i = 1; i < patternSize; i++){
        /* case 1: i is outside a zbox*/
        if(i > r){
            z = 0; /* size of the current z-box*/
            while(z+i < patternSize && pattern[z] == pattern[z+i]){
                z++;
            }
            (*zTable)[i] = z;
            if(z > 0){
                l = i; /* update the left position of the current z-box*/
                r = i+z-1; /* update the right position of the current z-box*/
            }
        }
        /* case 2: i is inside a zbox*/
        else{
            k = i - l;
            betaSize = r - i +1;
            /* case 2a*/
            if((*zTable)[k] < betaSize){
                /* inside a z-box and smaller than its corresponding substring: no need to update lt and rt*/
                (*zTable)[i] = (*zTable)[k];
            }
            /* case 2b*/
            else{
                /* inside a z-box and larger than its corresponding substring: find how large and update lt and rt*/
                z = r+1;
                while(z < patternSize && pattern[z] == pattern[z-i]){
                    z++;
                }
                (*zTable)[i] = z-i;
                l = i;
                r = z-1;
            }
        }
    }
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

void PreprocessPattern(char* pattern, int patternSize, int** badCharTable, int** LTable, int** LPrimeTable, int **lPrimeTable){
    
    char* reversedPattern = malloc(patternSize*sizeof(int));
    int* ZTable = malloc(patternSize*sizeof(int)); /* to store the z values of the reversed pattern */
    int* NTable = malloc(patternSize*sizeof(int)); /* position i stores the length of the longest suffix of the substring Pattern[0...i] that is alsi a suffix of the full string */
    int nCharOcc[AlphabetSize]; /* to register the number of occurrences of a character in the pattern */
    int i;
    int j;
    int charIndex; 

    /* initialize the bad character table */
    for(i = 0; i < AlphabetSize; i++){
        nCharOcc[i] = 0;
        (*badCharTable)[i] = -1;
    }

    /* fill the bad character table and save the reversed pattern */
    for(i = patternSize-1; i>= 0; i--){
        charIndex = GetIndexFromChar(pattern[i]);
        reversedPattern[patternSize-i-1] = pattern[i]; /* save the reversed pattern */
        if((*badCharTable)[charIndex] < i){
            (*badCharTable)[charIndex] = i;
        }
    }

    /* find the Z values of the reversed pattern and fill the n table accordingly */
    ZAlgorithm(reversedPattern, patternSize, &ZTable);

    for(i = 0; i < patternSize; i++){
        NTable[i] = ZTable[patternSize-1-i]; /* N_j(P) = Z_{n-1-j}(P^r) */
        (*LTable)[i] = 0; /* initialize big L array */
        (*LPrimeTable)[i] = 0; /* initialize big L prime array */
        (*lPrimeTable)[i] = 0; /* initialize small l prime array */
    }
    NTable[patternSize-1] = patternSize;

    /* fill the L2 and l tables*/
    for(j = 0; j < patternSize-1; j++){
        i = patternSize - NTable[j];  
        if( i < patternSize)
            (*LPrimeTable)[i] = j+1;
    }

    for(i = 0; i < patternSize; i++){
        if(NTable[i] == i+1){
            (*lPrimeTable)[patternSize-i-1] = i+1;
        }
    }
    for(i = patternSize-2; i>= 0; i--){
        if((*lPrimeTable)[i] == 0){
            (*lPrimeTable)[i] = (*lPrimeTable)[i+1];
        }
    }


    /* fill the L1Table*/
    (*LTable)[1] = (*LPrimeTable)[1];
    for(i = 2; i < patternSize;i++){
        (*LTable)[i] = max((*LTable)[i-1], (*LPrimeTable)[i]);
    }

    free(ZTable);
    ZTable = NULL;
    free(NTable);
    NTable = NULL;
    free(reversedPattern);
    reversedPattern = NULL;
}

int BadCharacterRule(int* badCharTable, char characterInText, int positionInPattern){
    int charIndex = GetIndexFromChar(characterInText);
    if(badCharTable[charIndex] < 0)
        return 1;
    else 
        return positionInPattern - badCharTable[charIndex];
}

void BoyerMoore(char* text, int textSize, char* pattern, int patternSize){

    int posStoreSize = 2; /* size of the array of the occurrences' positions */
    int *posStore = malloc(posStoreSize*sizeof(int)); /* to store the occurrences of pattern in text*/ 
    int posCounter = 0;

    int *badCharTable = malloc(AlphabetSize*sizeof(int)); /* for the bad character rule */
    int *L1Table = malloc(patternSize*sizeof(int)); /* for the (strong) good suffix rule*/
    int *L2Table = malloc(patternSize*sizeof(int)); /* for the good suffix rule */
    int *lTable = malloc(patternSize*sizeof(int));  /* for the good suffix rule and match skip */
    int k; /* alignment position */
    int h; /* position in text */
    int i; /* position in pattern */
    int nComparisons = 0;
    int shift = 0;

    PreprocessPattern(pattern, patternSize, &badCharTable, &L1Table, &L2Table, &lTable);

    k = patternSize-1;
    while(k < textSize){
        i = patternSize-1;
        h = k;
        while(i >= 0 && EqualCharQ(pattern[i], text[h], &nComparisons)){
            i--;
            h--;
        }
        /* found an occurrence */
        if( i == -1){
            posCounter++;
            if(posCounter == posStoreSize){
                /* grow the positions' storage*/
                posStoreSize *= 2;
                posStore = realloc(posStore, posStoreSize*sizeof(int));
            }

            /* save position */
            posStore[posCounter-1] = k-patternSize+1;
            /* update k */
            k += patternSize-lTable[1];
        }else{
            /* mismatch */
            shift = max(BadCharacterRule(badCharTable, text[h], i), GoodSuffixRule(L2Table, lTable, i, patternSize));
            if(shift < 1)
                shift = 1;
            k += shift; /*, GoodSuffixRule(L2Table, lTable, k)));*/
        }
    }
    
    /* print positions and number of comparisons*/
    for(i = 0; i < posCounter; i++){
        printf("%d ", posStore[i]);
    }
    printf("\n");
    printf("%d \n", nComparisons);

    free(posStore);
    posStore = NULL;    


    free(badCharTable);
    badCharTable = NULL;
    free(L1Table);
    L1Table = NULL;
    free(L2Table);
    L2Table = NULL;
    free(lTable);
    lTable = NULL;
}


int GoodSuffixRule(int* LTable, int* lTable, int mismatchPos, int patternSize){

    int shift = 1;

    if(mismatchPos == patternSize-1){
        return 1;
    }
    else{
        if(LTable[mismatchPos+1] > 0){
            shift = patternSize - LTable[mismatchPos+1];
        }else{
            shift = patternSize - lTable[mismatchPos+1];
        }
    }
    return shift;
}
