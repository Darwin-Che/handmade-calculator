#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "tokenize.h"

struct TokenList* inputList;
struct TokenList* opStack;
struct TokenList* outputList;

struct TokenList* popListInp();
struct TokenList* popListOp();
struct TokenList* popListOutput();

void pushListInp(struct TokenList* src);
void pushListOp(struct TokenList* src);
void pushListOutput(struct TokenList* src);

void shuntYard(struct TokenList* input);
