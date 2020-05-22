#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "rpn.h"

struct TokenList* inputList;
struct TokenList* opStack;
struct TokenList* outputList;

struct TokenList* popListInp(){
    struct TokenList* ans = inputList;
    if (!inputList) {
        fprintf(stderr, "cannot pop empty inputList\n");
        exit(EXIT_FAILURE); /* indicate failure.*/
    }
    inputList = inputList->next;
    ans->next = NULL;
    return ans;
}

struct TokenList* popListOp(){
    struct TokenList* ans = opStack;
    if (!opStack) {
        fprintf(stderr, "cannot pop empty opStack\n");
        exit(EXIT_FAILURE); /* indicate failure.*/
    }
    opStack = opStack->next;
    ans->next = NULL;
    return ans;
}

struct TokenList* popListOutput(){
    struct TokenList* ans = outputList;
    if (!outputList) {
        fprintf(stderr, "cannot pop empty inputList\n");
        exit(EXIT_FAILURE); /* indicate failure.*/
    }
    outputList = outputList->next;
    ans->next = NULL;
    return ans;
}

void pushListInp(struct TokenList* src){
    src->next = inputList;
    inputList = src;
}

void pushListOp(struct TokenList* src){
    src->next = opStack;
    opStack = src;
}

void pushListOutput(struct TokenList* src){
    src->next = outputList;
    outputList = src;
}

// src is in inputList; tar is in opStack
// 0: we should pop src to opStack;
// 1: we should pop tar to outputList;
int ifPopOp(struct TokenList* src, struct TokenList* tar){
    if (!(src->EXTYPE == OP && tar->EXTYPE == OP)){
        fprintf(stderr, "Wierd error #1\n");
        exit(EXIT_FAILURE); /* indicate failure.*/
    }
    switch (src->op->op)
    {
    case ADD:
    case SUB:
        switch (tar->op->op)
        {
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case POW:
        case LOG:
            return 1;
            break;
        default:
            fprintf(stderr, "invalid Op\n");
        	exit(EXIT_FAILURE); /* indicate failure.*/
            break;
        }
    case MUL:
    case DIV:
        switch (tar->op->op)
        {
        case ADD:
        case SUB:
            return 0;
            break;
        case MUL:
        case DIV:
        case POW:
        case LOG:
            return 1;
            break;
        default:
            fprintf(stderr, "invalid Op\n");
        	exit(EXIT_FAILURE); /* indicate failure.*/
            break;
        }
    case POW:
       switch (tar->op->op)
        {
        case ADD:
        case SUB:
        case MUL:
        case DIV:
            return 0;
            break;
        case POW:
        case LOG:
            return 1;
            break;
        default:
            fprintf(stderr, "invalid Op\n");
        	exit(EXIT_FAILURE); /* indicate failure.*/
            break;
        }
    case LOG:
        switch (tar->op->op)
        {
        case ADD:
        case SUB:
        case MUL:
        case DIV:
        case POW:
        case LOG:
            return 0;
            break;
        default:
            fprintf(stderr, "invalid Op\n");
        	exit(EXIT_FAILURE); /* indicate failure.*/
            break;
        }
    default:
        fprintf(stderr, "invalid Op\n");
        exit(EXIT_FAILURE); /* indicate failure.*/
        break;
    }
}

void shuntYard(struct TokenList* input){
    inputList = input;
    while (inputList){
        // for debug purpose
        // printf("inputlist: ");
        // printEx(inputList);
        // printf("\nopstack: ");
        // printEx(opStack);
        // printf("\noutputlist: ");
        // printEx(outputList);
        // printf("\n-------------------\n");
        switch (inputList->EXTYPE)
        {
        case LP:
            // push it onto the opStack
            pushListOp(popListInp());
            break;
        case RP:
            // while the operator at the top of opStack is not a left parenthesis:
            // pop the operator from the opStack onto the outputList.
            while (opStack->EXTYPE != LP){
                pushListOutput(popListOp());
                // if opStack run out without finding (, there is a mismatch
                if (opStack == NULL){
                    fprintf(stderr, "mismatched parenthesis\n");
                    exit(EXIT_FAILURE);
                }
            }
            if (opStack->EXTYPE == LP){
                popListOp();
                popListInp();
            } else {
                fprintf(stderr, "Wierd error #0\n");
        	    exit(EXIT_FAILURE); /* indicate failure.*/
            }
            break;
        case NUM:
            pushListOutput(popListInp());
            break;
        case OP:
            while (opStack && opStack->EXTYPE == OP && ifPopOp(inputList, opStack)){
                pushListOutput(popListOp());
            }
            pushListOp(popListInp());
            break;
        default:
            fprintf(stderr, "Wierd error #2\n");
            exit(EXIT_FAILURE);
            break;
        }
    }
    while (opStack){
        if (opStack->EXTYPE == LP || opStack->EXTYPE == RP){
            fprintf(stderr, "mismatched parenthesis\n");
            exit(EXIT_FAILURE);
        } else {
            pushListOutput(popListOp());
        }
    }
}

/* int main(){
    char s[30] = "\tlog[1-3)*log4\t";
    struct TokenList *t = tokenize(s);
    // printf("%d\n", t->EXTYPE);
    // printEx(t);
    // printf("\n-----------------------\n");
    shuntYard(t);
    printEx(outputList);
    return 0;
} */