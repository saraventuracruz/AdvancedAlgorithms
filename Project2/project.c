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
    bool sentinelQ; /* whether the node is a sentinel node */
    bool rootQ;     /* whether the node is a root (a root points to a next root by the brother link, thus allowing several children) */
    bool terminalQ; /* whether the path-label corresponds to a terminal edege (if so, take 1 to string depth)*/
};
typedef struct point point;
struct point
{
    node* above;  /**< node above */
    node* bellow;  /**< node bellow */
    int sDepth;    /**< String-Depth */ 
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
node* CreateRoot(int, int, node*);
tree* CreateTree(int);
void AddNode(tree*, node*);
node* GetLastNode(tree*);
bool DescendQ(char**, point*, char);
void AddLeaf(tree*, point*, int, int);
void SuffixLink(point*);
void Descend(point*);

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
    setOfStrings = (char**) malloc(numberOfStrings*sizeof(char*));
    stringSizes = (int *) malloc(numberOfStrings*sizeof(int));
    lcsSizes = (int*) malloc((numberOfStrings-1)*sizeof(int));
    /* initialize lcs size counter as 0*/
    for(k = 0; k < numberOfStrings-1; k++){
        lcsSizes[k] = 0;
    }
    /* read and store each string*/
    for(s = 0; s < numberOfStrings; s++){
        readChar = getchar();
        stringSize = 0;
        while(readChar != ' '){
            /* read the string size and convert it to int */
            stringSize = stringSize*10+CharToInt(readChar);
            readChar = getchar();
        }
        stringSize++; /* add \0 to string */
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
    for(i = 0; i < 2*m+1; i++){
      if(suffixTree->setOfNodes[i] != NULL){
        free(suffixTree->setOfNodes[i]);
        suffixTree->setOfNodes[i] = NULL;
      }
    }
    free(suffixTree->setOfNodes);
    suffixTree->setOfNodes = NULL;
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
    
    int m = 0; /* sum of all string sizes */
    int i, j; /* iterators */
    tree* suffixTree = NULL;
    node* root = NULL;
    node* sentinel = NULL;
    node* new_node = NULL;
    node* last_created_node = NULL;
    point* p = malloc(sizeof(point));
    
    /* compute the value of m by adding each string size */
    for(i = 0; i < nStrings; i++){
        m += stringSizes[i];
    }
    /* initialize tree */
    suffixTree = CreateTree(2*m+1);

    /* create a sentinel node (which will occupy the first position of the tree's set of nodes) */
    sentinel = CreateNode();
    AddNode(suffixTree, sentinel);
    sentinel->sentinelQ = true;

    for(i = 0; i < nStrings; i++){
        /* create a root for each string -> which will be linked by the brother link */
        root = CreateRoot(i, 0, sentinel); /* i = current string, 0 = head, the suffix link of a root is always the sentinel node */
        AddNode(suffixTree, root);

        /* make the first root the child of the sentinel (all the others shall be accessed by brothers links) */
        if(i == 0){
            sentinel->child = root;
        }

        /* set the terminator of the current string as a different character from the remaining, 
        so that it is possible to differentiate the string being inserted*/
        setOfStrings[i][stringSizes[i]-1] = '\1';
        
        /* initialize the point */
        p->above = root; /* return to the root */
        p->bellow = NULL;
        p->sDepth = 0;
        for(j = 0; j < stringSizes[i]; j++){

            while(!DescendQ(setOfStrings, p, setOfStrings[i][j])){
                AddLeaf(suffixTree, p, i, j);
                SuffixLink(p);
            }
            Descend(p);
        }
        setOfStrings[i][stringSizes[i]-1] = '\0'; /* restore terminator */
    }
    free(p);
    return suffixTree;  
}

void AddLeaf(tree* sTree, point* p, int i, int j){
    /* TODO: increase the depth of each node related to string i and add a leaf from the root corresponding to the current suffix*/

    node* new_node = NULL;

    if(p->above->child == NULL){
        new_node = CreateNode(i, j, p->sDepth);
        AddNode(sTree, new_node);
        p->above->child  = new_node;
        p->bellow = new_node;
    }

}

bool DescendQ(char** soStrings, point* p, char c){
    /* check whether character c is the next character after point p at the corresponding string */

    int Ti = -1;
    int head = -1;
    /* TODO: this depends on whether we consider the node to be the end or the begining of an edge */
    if(p->bellow != NULL){
        /* check whether there is, in fact, a node bellow the point */
        Ti = p->bellow->Ti;
        head = p->bellow->head;
        return soStrings[Ti][head+p->sDepth] == c;
    }

    return true;
}

void SuffixLink(point* p){
    /* jump to the slink of a node, if p is at a node, or use the skip-count trick, 
    i.e. follow the slink of the node above p and put p at the correct position (using p->sdepth) without the need to read the chars at that edge*/

    node* slink = NULL;

    /* TODO: assess if p is at a node */
    if(true){
        return;
    }
    else{
        slink = p->above->slink;
        p->above = slink;
        p->bellow = slink->child;
        p->sDepth--; /* a suffix link is a link from a substring c-alpha to a substring alpha, thus the string depth decreases by one */
    }

    return;
}
void Descend(point* p){
    /* at this point, we already know that it is possible to read the character c after the point p, 
    thus we might simply increase the string depth of p and of the edge (this depends on whether we are considering the node to be the end or the begining of an edge) */
    p->sDepth++;
    if(p->bellow != NULL){
        /* in the future, there should be no need to check this*/
        p->bellow->sdep++;
    }

}

node* CreateNode(){
    /* create a new node */
    node* new_node = malloc(sizeof(node));
    new_node->Ti = -1;
    new_node->head = -1;
    new_node->sdep = -1;
    new_node->child = NULL;
    new_node->brother = NULL;
    new_node->slink = NULL;
    new_node->sentinelQ = false;
    new_node->rootQ = false;
    new_node->terminalQ = false;

    return new_node;
}

node* CreateRoot(int i, int j, node* sentinel){
    /* create a root node, given the string index and the head of the substring */
    node* root = malloc(sizeof(node));
    root->Ti = i;
    root->head = j;
    root->sdep = 0; /* a root is initialized with string depth 0 – no leaf*/
    root->child = NULL;
    root->brother = NULL;
    root->slink = sentinel;
    root->sentinelQ = false;
    root->rootQ = true;
    root->terminalQ = false;

    return root;
}

tree* CreateTree(int size){
/* creates  a tree as a set of nodes with size *size* (of all possible nodes) initialized as NULL 
    and a counter to keep track of the real number of nodes at the tree*/
    int i = 0;
    tree* retTree = malloc(sizeof(tree));
    retTree->setOfNodes = malloc(size*sizeof(node));
    retTree->nNodes = 0;
    for(i = 0; i < size; i++){
      retTree->setOfNodes[i] = NULL;
    }

    return retTree;
}

void AddNode(tree* tree, node* node){
    /* add the node to the set of nodes (replace a NULL pointer by the pointer to the node) and update the counter */
    tree->setOfNodes[tree->nNodes] = node;
    tree->nNodes++;
}

node* GetLastNode(tree* tree){
    /* retrieves the last added node */
    return tree->setOfNodes[tree->nNodes-1];
}

int CharToInt(char c){
    /* convert a char to an int */

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
