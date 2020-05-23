#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rpn.h"

struct NumberList{
    struct Number n;
    struct NumberList *next;
};

struct Number binOp(struct Number n1, struct Number n2, int INS);
struct Number unaOp(struct Number n1, int INS);
void freeNL(struct NumberList* src);
struct TokenList *reverseList(struct TokenList *src);
void eval(struct TokenList *src); 
// return one node as the result of the Binary operation of evalIn upon the first two node of evalOut

