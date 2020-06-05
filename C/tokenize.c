#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include "tokenize.h"

// the max length of the expression shall not exceed 149 chars
char globalStr[150];
int strIndex = 0;

int islp(char c){
    return (c == '(') || (c == '[') || (c == '{'); 
}

int isrp(char c){
    return (c == ')') || (c == ']') || (c == '}');  
}

struct Leftparen* readlp(){
    struct Leftparen* lp = malloc(sizeof(struct Leftparen));
    lp->p = globalStr[strIndex];
    ++strIndex;
    return lp;
}

struct Rightparen* readrp(){
    struct Rightparen* rp = malloc(sizeof(struct Rightparen));
    rp->p = globalStr[strIndex];
    ++strIndex;
    return rp;
}

int isnum(char c){
    if (c >= '0' && c <= '9') {
        return 1;
    } else if (c == '.') {
        return 1;
    } else {
        return 0;
    }
}

struct Number* readnum(){
    struct Number* ans = malloc(sizeof(struct Number));
    ans->NUMTYPE = INT;
    int tryint = 0;
    double trydouble = 0;
    int doubledigit = 1;
    // tryint record int part, before '.' show up
    // trydouble record float part, after '.' show up
    while (isnum(globalStr[strIndex])) {
        if (globalStr[strIndex] == '.') {
            // '.' cannot show twice
            if (ans->NUMTYPE == DOUBLE) {
                fprintf(stderr, "More than one . in number\n");
        	    exit(EXIT_FAILURE); /* indicate failure.*/
			    break;
            }
            ans->NUMTYPE = DOUBLE;
        } else if (ans->NUMTYPE == INT){
            tryint = 10*tryint + globalStr[strIndex] - '0';
        } else if (ans->NUMTYPE == DOUBLE){
            trydouble = trydouble + (globalStr[strIndex] - '0') * pow(10, -doubledigit);
            ++doubledigit;
        }
        // goto the next char
        ++strIndex;
    }
    // when the loop ends, globalStr[strIndex] should not be a part of number, i.e. doesnot belong to this token
    if (ans->NUMTYPE == INT) {
        ans->intnum = tryint;
    } else if (ans->NUMTYPE == DOUBLE) {
        ans->doublenum = tryint + trydouble;
    }
    return ans;
}

struct Operator* readop(){
    struct Operator* ans = malloc(sizeof(struct Operator));
    if (globalStr[strIndex] == '+'){
        ans->op = ADD; 
        ++strIndex;
    } else if (globalStr[strIndex] == '-') {
        ans->op = SUB;
        ++strIndex;
    } else if (globalStr[strIndex] == '*') {
        ans->op = MUL;
        ++strIndex;
    } else if (globalStr[strIndex] == '/') {
        ans->op = DIV;
        ++strIndex;
    } else if (globalStr[strIndex] == '^') {
        ans->op = POW;
        ++strIndex;
    } else if (globalStr[strIndex] == 'l') {
        if (globalStr[strIndex+1] == 'o' && globalStr[strIndex+2] == 'g') {
            ans->op = LOG;
            strIndex = strIndex+3;
        } else {
            fprintf(stderr, "not valid operator at char %c\n", globalStr[strIndex]);
            exit(EXIT_FAILURE); /* indicate failure.*/ 
        }
    } else {
        fprintf(stderr, "not valid operator at char %c\n", globalStr[strIndex]);
        exit(EXIT_FAILURE); /* indicate failure.*/
    }
    return ans;
}

// the program does not alters the input
// create a TokenList in heap
struct TokenList* tokenize(char* str){
    // first make the globalstr equal to the input
    strncpy(globalStr, str, 149);
    // the answer to return is start->next, remember to free the first node
    struct TokenList* start = malloc(sizeof(struct TokenList));
    start->next = NULL;
    // some variable to use in the loop
    struct TokenList* prev = start;
    struct TokenList* tmp;
    // printf("%send\n",globalStr);
    while (strIndex <= 149 && globalStr[strIndex]) {
        // printf("%d\n", strIndex);
        // next 2 lines for test purpose
        // printTL(start->next);
        // printf("\n-------------\n");
        // work on tmp as the new node
        // printTL(start->next);
        tmp = malloc(sizeof(struct TokenList)); 
        if (isnum(globalStr[strIndex])) {
            tmp->EXTYPE = NUM;
            tmp->n = readnum();
        } else if (islp(globalStr[strIndex])) {
            tmp->EXTYPE = LP;
            tmp->lp = readlp();
        } else if (isrp(globalStr[strIndex])) {
            tmp->EXTYPE = RP;
            tmp->rp = readrp();
        } else if (isspace(globalStr[strIndex])) {
            // manually increment strIndex
            free(tmp);
            ++strIndex;
            continue; 
        } else {
            tmp->EXTYPE = OP;
            tmp->op = readop();
        }
        // just to make sure of the last node
        tmp->next = NULL;
        // link tmp on the list
        prev->next = tmp;
        prev = tmp;
    }
    // return start->next
    // free start: the first node
    tmp = start->next;
    free(start);
    return tmp;
}

// print the content for debug
void printTL(struct TokenList* e){
    while (e) {
        printf(", ");
        if (e->EXTYPE == LP) {
            printf("(");
        } else if (e->EXTYPE == RP) {
            printf(")");
        } else if (e->EXTYPE == NUM) {
            if (e->n->NUMTYPE == INT) {
                printf("%d", e->n->intnum);
            } else {
                printf("%lf", e->n->doublenum);
            }
        } else if (e->EXTYPE == OP) {
            if (e->op->op == ADD) {
                printf("ADD");
            } else if (e->op->op == SUB) {
                printf("SUB");
            } else if (e->op->op == MUL) {
                printf("MUL");
            } else if (e->op->op == DIV) {
                printf("DIV");
            } else if (e->op->op == POW) {
                printf("POW"); 
            } else if (e->op->op == LOG) {
                printf("LOG");
            }
        }
        e=e->next;
    }
    printf("\n");
}

// free memory of TokenList
void freeTL(struct TokenList* e){
    struct TokenList* cur = e;
    struct TokenList* next; 
    while (cur) {
        next = cur->next;
        switch (cur->EXTYPE)
        {
        case LP:
            free(cur->lp);
            break;
        case RP:
            free(cur->rp);
            break;
        case NUM:
            free(cur->n);
            break;
        case OP:
            free(cur->op);
            break;
        default:
            break;
        }
        free(cur);
        cur = next;
    }
}