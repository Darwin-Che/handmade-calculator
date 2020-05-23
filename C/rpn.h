#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tokenize.h"

// result from tokenize.c
struct TokenList* inputList;
// tmp operator stack
struct TokenList* opStack;
// the result of this program
struct TokenList* outputList;

// the following 6 functions don't malloc or free

// pop one from inputList
struct TokenList* popListInp();
// pop one from opStack
struct TokenList* popListOp();
// pop one from outputList
struct TokenList* popListOutput();

// push one to inputList
void pushListInp(struct TokenList* src);
// push one to opStack
void pushListOp(struct TokenList* src);
// push one to outputStack
void pushListOutput(struct TokenList* src);

// apply shunting-yard algorithm
// dont malloc
// free all parenthesis
void shuntYard();
