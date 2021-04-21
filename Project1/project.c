#include <stdio.h>  /*printf*/
#include <stdlib.h> /*malloc*/
#include <stdbool.h> /* to use booleans */

#define AlphabetSize 4 /* 4 letters on the considered alphabet*/
#define max(a,b) (a<b) ? b : a /* define the max operation */

int ReadLine(FILE*, char**); /* to read lines from file (texts and patterns)*/

bool EqualCharQ(char, char, int*); /* to compare chars and count comparisons */

/* naive algorithm to find all the occurrences of a pattern in a text: 
compare character by character, rescanning the entire pattern after shifting one position 
whenever there is a mismatch or the end of the pattern is reached */
void NaiveAlgorithm(char*, int, char*, int); 

/* implementation of the knuth morris pratt algorithm to find all the occurrences of a pattern in a text*/
void KnuthMorrisPratt(char*, int, char*, int);
/* preprocessing step of the knuth morris pratt algorithm */
void ComputePrefixFunction(char*, int, int**);

/* implementation of the boyer moore algorithm */
void BoyerMoore(char*, int, char*, int);
int BadCharacterRule(int*, char, int, int);
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
    bool isReadText = false;
    bool isReadPattern = false;
    int textSize = 0;
    int patternSize = 0;  

    char readChar = getchar(); /* read input character by character */
    while(readChar != EOF){

        isReadText = readChar == 'T';
        isReadPattern = readChar == 'N'|| readChar == 'K' || readChar == 'B';

        if(readChar == 'X')
            break;

        if(isReadText)
            textSize = ReadLine(stdin, &text); /* change text with the read values*/

        if(isReadPattern){

            patternSize = ReadLine(stdin, &pattern); /* change pattern with the read values*/

            if(readChar == 'N')
                NaiveAlgorithm(text, textSize, pattern, patternSize);
            else if (readChar == 'K')
                KnuthMorrisPratt(text, textSize, pattern, patternSize);
            else if(readChar == 'B')
               BoyerMoore(text, textSize, pattern, patternSize);
        }
        readChar = getchar();
    }

    free(text);
    text = NULL;
    free(pattern);
    pattern = NULL;
 
    return 0;
}

int ReadLine(FILE *inputFile, char** text){
    /* read a line from the input file and save it in *text* */
    /* return the number of characters of *text* */

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
    /* to count comparisons and checking whether chars p and t match */
    *nComparisons = *nComparisons+1;
    return p == t;

}

void NaiveAlgorithm(char* text, int textSize, char* pattern, int patternSize){
    /* naive approach to string matching: keep going while the characters in both the pattern and text match */
    /* shift the pattern's alignment with the text one position whenever there is a mismatch or an occurrence has been found */

    int t; /* text iterator */
    int p; /* pattern iterator */

    if(textSize < patternSize || textSize < 1 || patternSize < 1){
        printf("\n"); /* the pattern never occurs on the text*/
        return;
    }

    p = 0;   
    for(t = 0; t+p < textSize; t++){
        
        while(p < patternSize && t+p < textSize && pattern[p] == text[t+p]){
            p++;
        }     
        /* whenever p reaches patternSize, an occurrence of the pattern has been found at position t of the text */
        if(p == patternSize){
            printf("%d ", t);
            p = 0;
        }
        else if(pattern[p] != text[t+p]){
            p = 0;
        }
    }
    printf("\n");
}

void ComputePrefixFunction(char* pattern, int patternSize, int** prefix){
    /* compute the values of pi according to the algorithm COMPUTE-PREFIX-FUNCTION of CLRS*/
    /* where pi_i = length of the longest prefix of P that is a proper suffix of P_i */
    /* the algorithm is linear in the size of the pattern */

    int q = 1; /* point to the second position of the pattern: proper suffix*/
    int k = -1; /* point to the position "right before" the pattern */
    *prefix = realloc(*prefix, patternSize*sizeof(int));

    (*prefix)[0] = 0;
    for(q = 1; q < patternSize; q++){
        while (k >= 0 &&  pattern[k+1] != pattern[q])
            k = (*prefix)[k]-1;
        if (pattern[k+1] == pattern[q])
        /* if k might move forward */
            k++;
        /* since the positions begin at 0, the length is given by the position +1*/
        (*prefix)[q] = k+1;
    }
    return;
}

void KnuthMorrisPratt(char* text, int textSize, char* pattern, int patternSize){

    int *prefix = malloc(2*sizeof(int));
    int t; /* position in text */
    int p; /* position in pattern */
    int nComparisons = 0; /* number of comparisons */

    if(textSize < patternSize || textSize < 1 || patternSize < 1){
        printf("\n"); /* the pattern never occurs on the text*/
        printf("0 \n"); /* 0 comparisons */
        return;
    }

    /* preprocess the pattern */
    ComputePrefixFunction(pattern, patternSize, &prefix);

    p = -1;
    for(t = 0; t < textSize; t++){

        while(p >= 0 && pattern[p+1]!= text[t]){
            nComparisons++;
            p = prefix[p]-1;
        }

        nComparisons++;
        if(pattern[p+1] == text[t]){
            p++;
        }

        /* found a match */
        if(p+1 == patternSize){
            /* print occurrence position */
            printf("%d ", t-p);
            /* update p */
            if(t+1 < textSize){
                nComparisons++;
                p = prefix[p]-1;
            }           
        }
    }
    printf("\n");
    /* end of text */
   
    /* print number of comparisons*/
    printf("%d \n", nComparisons);   

    free(prefix);
    prefix = NULL;  
}

void ZAlgorithm(char* pattern, int patternSize, int** zTable){
    /*computes the Z values of each position in the pattern */
    /* where Z_i = length of the longest substring that starts at position i and matches a prefix of P*/
  
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
                /* inside a z-box and smaller than its corresponding substring: no need to update l and r*/
                (*zTable)[i] = (*zTable)[k];
            }
            /* case 2b*/
            else{
                /* inside a z-box and larger than its corresponding substring: find how large and update l and r*/
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
    /* preprocess pattern: fill the badCharacterTable, and both LPrime and lPrime tables */
    /* since we are resorting to the strong suffix rule, the LTable will not be filled */
    /* in order to fill both LPrime and lPrime tables, the z values of the reversed pattern are computed using the Z-Algorithm */
    char* reversedPattern = malloc(patternSize*sizeof(int));
    int* ZTable = malloc(patternSize*sizeof(int)); /* to store the z values of the reversed pattern */
    int* NTable = malloc(patternSize*sizeof(int)); /* position i stores the length of the longest suffix of the substring Pattern[0...i] that is also a suffix of the full string */
    int i;
    int j;
    int charIndex; 

    /* initialize the bad character table */
    for(i = 0; i < AlphabetSize; i++){
        (*badCharTable)[i] = -1; /* -1 if the character does not occur in the pattern */
    }

    /* fill the bad character table and save the reversed pattern */
    for(i = patternSize-1; i>= 0; i--){
        charIndex = GetIndexFromChar(pattern[i]);
        if((*badCharTable)[charIndex] < i){
            (*badCharTable)[charIndex] = i;
        }
        reversedPattern[patternSize-i-1] = pattern[i]; /* save the reversed pattern */
    }

    /* find the Z values of the reversed pattern and fill the N table accordingly */
    ZAlgorithm(reversedPattern, patternSize, &ZTable);
    /* fill the N table and initialize the remaining tables */
    for(i = 0; i < patternSize; i++){
        NTable[i] = ZTable[patternSize-1-i]; /* N_j(P) = Z_{n-1-j}(P^r) */
        (*LTable)[i] = 0; /* initialize big L array */
        (*LPrimeTable)[i] = 0; /* initialize big L prime array */
        (*lPrimeTable)[i] = 0; /* initialize small l prime array */
    }
    NTable[patternSize-1] = patternSize; /* ZTable[0] is undefined, by definition, but NTable[lastPosition] is the size of the pattern*/

    /* fill the LPrime table */
    /* algorithm "Z-based Boyer-Moore" from Gusfield, using theorem 2.2.2*/
    for(j = 0; j < patternSize-1; j++){
        i = patternSize - NTable[j];  
        if( i < patternSize){
        /* since the positions start at 0, we must add 1, */
        /* so that we can use the good suffix rule just as stated in Gusfield */
            (*LPrimeTable)[i] = j+1; 
        }

    }

    /* fill the lPrime table */
    /* following threorem 2.2.4 (Gusfield): l'(i) = largest j<= n-i+1 such that Nj = j*/
    /* since positions start at 0, the condition is Nj = j+1*/
    for(i = 0; i < patternSize; i++){
        if(NTable[i] == i+1){
            /* length, hence i+1*/
            (*lPrimeTable)[patternSize-i-1] = i+1;
        }
    }
    /* update the values that did not match the condition Nj = j+1 precisely at n-i+1*/
    for(i = patternSize-2; i>= 0; i--){
        if((*lPrimeTable)[i] == 0){
            (*lPrimeTable)[i] = (*lPrimeTable)[i+1];
        }
    }

    /* we could also fill the LTable following the same algorithm, but we are implementing the strong good suffix rule, thus we won't need the LTable */
    /* algorithm "Z-based Boyer-Moore" from Gusfield, using theorem 2.2.2*/
    /*(*LTable)[1] = (*LPrimeTable)[1];*/
    /*for(i = 2; i < patternSize;i++){ (*LTable)[i] = max((*LTable)[i-1], (*LPrimeTable)[i]);}*/
    
    free(ZTable);
    ZTable = NULL;
    free(NTable);
    NTable = NULL;
    free(reversedPattern);
    reversedPattern = NULL;
}

int BadCharacterRule(int* badCharTable, char characterInText, int positionInPattern, int patternSize){
    /* returns the shift given by the bad character rule, which is the patternSize if the character does not occur in the pattern */
    /* or the difference between the current position in the pattern and the right-most occurrence of the character in the pattern */
    int charIndex = GetIndexFromChar(characterInText);
    if(badCharTable[charIndex] < 0)
    /* if the character does not occur in the pattern shift the whole pattern*/
        return patternSize;
    else 
    /* otherwise shift as much as the current position - the right most position of the character in the pattern */
    /* this could yield a negative shift, having no meaning in the max of the three possible shifts (badChar, goodSuffix, 1) */
        return positionInPattern - badCharTable[charIndex];
}
int GoodSuffixRule(int* LPrimeTable, int* lPrimeTable, int mismatchPos, int patternSize){

    int shift;
    /* if the mismatch position is the last position in the pattern, apply the special case */
    if(mismatchPos == patternSize-1){
        return 1;
    }
    else{
        /* if the largest position j less than the last one s.t. P[mismatchPos+1...patternSize-1]*/
        /* matches a suffix of P[0...j] and the character preceding that suffix is not equal to P[mismatchPos] is greater than 0  */
        /* than we can shift as much as the size of that suffix, which is given by LPrimeTable[mismatchPos+1] */
        if(LPrimeTable[mismatchPos+1] > 0){
            shift = patternSize - LPrimeTable[mismatchPos+1];
        
        /* otherwise, such suffix does not exist, and we resort to the lPrimeTable */
        /* lPrimeTable[i] gives the length of the largest suffix of P[i...patternSize-1] that is also a prefix of P, when it exists */
        /* thus we can shift as much as the difference between the patternSize and the size of that suffix*/
        }else{
            shift = patternSize - lPrimeTable[mismatchPos+1];
        }
    }
    return shift;
}

void BoyerMoore(char* text, int textSize, char* pattern, int patternSize){

    int *badCharTable = malloc(AlphabetSize*sizeof(int)); /* for the bad character rule */
    int *LPrimeTable = malloc(patternSize*sizeof(int)); /* for the (strong) good suffix rule*/
    int *LTable = malloc(patternSize*sizeof(int)); /* for the good suffix rule */
    int *lPrimeTable = malloc(patternSize*sizeof(int));  /* for the good suffix rule and match skip */
    int k; /* alignment position */
    int h; /* position in text */
    int i; /* position in pattern */
    int nComparisons = 0;
    int shift = 0;

    PreprocessPattern(pattern, patternSize, &badCharTable, &LTable, &LPrimeTable, &lPrimeTable);

    k = patternSize-1;
    while(k < textSize){
        i = patternSize-1;
        h = k;
        while(i >= 0 && 0 < ++nComparisons && pattern[i] == text[h]){
            i--;
            h--;
        }
        /* found an occurrence */
        if( i == -1){
            /* print position */
            printf("%d ", k-patternSize+1);

            /* update k */
            k += patternSize-lPrimeTable[1];
        }else{
            /* mismatch */
            shift = max(BadCharacterRule(badCharTable, text[h], i, patternSize), GoodSuffixRule(LPrimeTable, lPrimeTable, i, patternSize));
            if(shift < 1)
                shift = 1;
            k += shift;
        }
    }
    printf("\n");
    /* print number of comparisons*/
    printf("%d \n", nComparisons);

    free(badCharTable);
    badCharTable = NULL;
    free(LTable);
    LTable = NULL;
    free(LPrimeTable);
    LPrimeTable = NULL;
    free(lPrimeTable);
    lPrimeTable = NULL;
}



