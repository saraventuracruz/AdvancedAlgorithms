#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc */
#include <stdbool.h> /* to use booleans */

typedef struct node node;
struct node {
    int Ti;         /**< The value of i in Ti */
    int head;       /**< The path-label start at &(Ti[head]) */
    int sdep;       /**< String-Depth */
    node* child;    /**< Child */
    node* brother;  /**< brother */
    node* slink;    /**< Suffix link */
    node** hook;    /**< What keeps this linked? */   
    bool sentinelQ; /* whether the node is a sentinel node */
    bool terminalQ; /* whether the path-label corresponds to a terminal edege (if so, take 1 to string depth)*/
};
typedef struct point point;
struct point
{
    node* a;  /**< node above */
    node* b;  /**< node bellow */
    int s;    /**< String-Depth */ 
};

typedef struct tree tree;
struct tree
{
    int nNodes; /* to keep track of the number of created nodes (not sure if this is relevant) */
    node** setOfNodes; /* to save the created nodes */
};

/* forward declaration */
int CharToInt(char); /*convert a char to an int*/
tree* BuildSuffixTree(char**, int*, int);
node* CreateNode();
tree* CreateTree(int);
void AddNode(tree*, node*);
node* GetLastNode(tree*);
bool DescendQ(tree*, point*, char);
void AddLeaf(tree*, point*, char);
void Descend(tree*, point*, char);

/* global variables */
int nNodes = 0; 

int main(){

    /* Initialize variables */
    int numberOfStrings = 0; /* number of strings */ 
    char **setOfStrings = NULL; /* to store a set of strings */
    int *stringSizes = NULL; /* to store the length of each string*/
    int m = 0; /* sum of all string lengths */
    int s = 0; /* string index */
    int stringSize = 0; /* size of a string*/
    int c = 0; /* position of a character in a string */
    char readChar = getchar(); /* read input character by character */
    int *lcsSizes = NULL; /* to store the longest common substring sizes for each k = 2,...,numberOfStrings */
    int k = 0; /* number of common substrings */
    int i = 0; /* iterator */
    tree *suffixTree = NULL; /* suffix tree as a list of nodes */

    while(readChar != '\n'){
        /*read the number of strings and convert it to int*/
        numberOfStrings = numberOfStrings*10+CharToInt(readChar);
        readChar = getchar();
    }       
    printf("numberOfStrings: %d\n", numberOfStrings);
    setOfStrings = (char**) malloc(numberOfStrings*sizeof(char*));
    stringSizes = (int *) malloc(numberOfStrings*sizeof(int));
    lcsSizes = (int*) malloc((numberOfStrings-1)*sizeof(int));
    /* initialize lcs size counter as 0*/
    for(k = 0; k < numberOfStrings-1; k++){
        lcsSizes[k] = 0;
    }
    /* read and store each string*/
    for(s = 0; s < numberOfStrings; s++){
      /* printf("s: %d\n", s);*/
        readChar = getchar();
        stringSize = 0;
        printf("stringSize: %d\n", stringSize);
        while(readChar != ' '){
            /* read the string size and convert it to int */
            stringSize = stringSize*10+CharToInt(readChar);
            readChar = getchar();
        }
        printf("StringSize: %d\n", stringSize);
        stringSize++; /* add \0 to string*/
        m += stringSize;
        stringSizes[s] = stringSize;
        setOfStrings[s] = (char*) malloc(stringSize*sizeof(char));
        if(setOfStrings[s] == NULL){
            /*check whether it was possible to allocate memory*/
            printf("error while allocating memory setOfStrings[%d]\n", s);
        }
        /*read each character of the string and store it at the corresponding string entry*/
        for(c = 0; c < stringSize-1; c++){
            setOfStrings[s][c] = getchar();
        }
        setOfStrings[s][stringSize-1] = '\0'; /* add a terminator */
	    /* printf("string: %s\n", setOfStrings[s]);*/
        readChar = getchar(); /* new line character */
    }

    suffixTree = BuildSuffixTree(setOfStrings, stringSizes, numberOfStrings);

    /* print output */
    for(k = 0; k < numberOfStrings-1; k++){
        printf("%d ", lcsSizes[k]);
    }
    printf("\n");

    /* free memory */
    printf("m: %d\n", m);
    for(i = 0; i < 2*m+1; i++){
      if(suffixTree->setOfNodes[i] != NULL){
	    printf("trying to free non existing memory\n");
        free(suffixTree->setOfNodes[i]);
        suffixTree->setOfNodes[i] = NULL;
      }
    }
    free(suffixTree);
    suffixTree = NULL;

    for(s = 0; s < numberOfStrings; s++){
        if(setOfStrings[s]){
            free(setOfStrings[s]);
            setOfStrings[s] = NULL;
        }

    }
    free(setOfStrings);
    setOfStrings = NULL;
    free(stringSizes);
    stringSizes = NULL;
    free(lcsSizes);
    lcsSizes = NULL;
}

tree* BuildSuffixTree(char** setOfStrings, int* stringSizes, int nStrings){
    
    tree* suffixTree = NULL;
    int m = 0; /* sum of all string sizes */
    int i, j; /* iterators */
   
    node* root = NULL;
    node* sentinel = NULL;
    node* new_node = NULL;
    point* p = malloc(sizeof(point));
    
    /* compute the value of m by adding each string size */
    for(i = 0; i < nStrings; i++){
        m += stringSizes[i];
    }
    /* initialize tree */
    suffixTree = CreateTree(2*m+1);
    /*tree = (node**) malloc((2*m+1)*sizeof(node*));*/
    printf("sizeof(node): %lu\n", sizeof(node*));
    printf("sizeof(tree): %lu\n", sizeof(suffixTree));
    printf("initializing tree (m: %d)\n", m);
    for(i = 0; i < 2*m+1; i++){
      suffixTree->setOfNodes[i] = NULL;
    }

    sentinel = CreateNode();
    AddNode(suffixTree, sentinel);

    root = CreateNode();
    AddNode(suffixTree, root);
    root->slink = sentinel;

    sentinel->child = root;
    sentinel->slink = root;
    sentinel->sentinelQ = true;

    for(i = 0; i < nStrings; i++){
        setOfStrings[i][stringSizes[i]-1] = '\1';
        for(j = 0; j < stringSizes[i]; j++){
            p->a = root;
            p->b = root;
            p->s = j;
            /*while(!DescendQ(suffixTree, p, setOfStrings[i][j])){*/
                /*AddLeaf(suffixTree, p, i, j);*/
                /*SuffixLink(suffixTree, p);*/
            /*}*/
            /*Descend(suffixTree, p, setOfStrings[i][j]);*/
        }
        setOfStrings[i][stringSizes[i]-1] = '\0'; /* restore terminator */
    }
    free(p);
    return suffixTree;  
}

node* CreateNode(){

    node* new_node = malloc(sizeof(node));
    new_node->Ti = -1;
    new_node->head = -1;
    new_node->sdep = -1;
    new_node->child = NULL;
    new_node->brother = NULL;
    new_node->slink = NULL;
    new_node->hook = NULL;
    new_node->sentinelQ = false;

    return new_node;
}

tree* CreateTree(int size){

    tree* retTree = malloc(sizeof(tree));
    retTree->setOfNodes = malloc(size*sizeof(node));
    retTree->nNodes = 0;

    return retTree;
}

void AddNode(tree* tree, node* node){
    tree->setOfNodes[tree->nNodes] = node;
    tree->nNodes++;
}

node* GetLastNode(tree* tree){
    return tree->setOfNodes[tree->nNodes-1];
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
