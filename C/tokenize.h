#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

// ( or [ or {
struct Leftparen {
    char p;
}; 

// ) or ] or }
struct Rightparen {
    char p;
}; 

// numbers
enum numtype {INT = 0, DOUBLE};
struct Number {
    enum numtype NUMTYPE;
    union {
        int intnum;
        double doublenum;
    };
};

// operators
// available ones: '+', '-', '*', '/', '^', 'log'
enum optype {ADD = 0, SUB, MUL, DIV, POW, LOG};
struct Operator {
    enum optype op;
};

// a linkedlist of tokens
enum extype {LP = 0, RP, NUM, OP};
struct TokenList {
    enum extype EXTYPE;
    union{
        struct Leftparen* lp;
        struct Rightparen* rp;
        struct Number* n;
        struct Operator* op;
    };
    struct TokenList* next;
};

char globalStr[150];
int strIndex;

// function declaration

// boolean function, no side-effects
int islp(char c);
int isrp(char c);
int isnum(char c);

// read function, change strIndex to pretend consume chars, and malloc structs

// consumes 1 left bracket
struct Leftparen* readlp();

// consumes 1 right bracket
struct Rightparen* readrp();

// consumes 1 number prefer double, but int will work
struct Number* readnum();

// consumes operator
struct Operator* readop();

// print structure of TokenList
void printTL(struct TokenList* l);

// free memory
void freeTL(struct TokenList* l);

// main function
struct TokenList* tokenize(char* str);