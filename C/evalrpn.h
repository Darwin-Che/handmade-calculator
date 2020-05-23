#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "rpn.h"

struct NumberList{
    struct Number n;
    struct NumberList *next;
};

// the reverse of result of rpn
struct TokenList *evalin;
// final result
struct NumberList *evalOut;

// print the numberlist for debug
void printNL(struct NumberList* e);
// reverse a tokenlist, use at the start of evaluation
struct TokenList *reverseList(struct TokenList *src);
// precond: INS is a binary operator
// return a Number struct that is the result of the operation
// the Number NUMTYPE is determined with respect to the operator and the NUMTYPE of each argument
struct Number binOp(struct Number n1, struct Number n2, int INS);
// similar to binOp but accpet unary operator
struct Number unaOp(struct Number n1, int INS);
// input: the result finished by rpn
// we compute the result and store it in evalOut
void eval(struct TokenList *input);

