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

// precond: the inputs are not empty
// src is in inputList; tar is in opStack
// 0: we should pop src to opStack;
// 1: we should pop tar to outputList;
// return 1 if the following is true; 
// ((tar has greater precedence) or (tar has equal precedence and is left associative))
int ifPopOp(struct TokenList* src, struct TokenList* tar){
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
        // note POW is right associative
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

// for reference at https://en.wikipedia.org/wiki/Shunting-yard_algorithm
void shuntYard(){
    while (inputList){
        // for debug purpose
        // printf("inputlist: ");
        // printTL(inputList);
        // printf("\nopstack: ");
        // printTL(opStack);
        // printf("\noutputlist: ");
        // printTL(outputList);
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
            // popout the '(' in opStack and the ')' in inputList
            struct TokenList* tmp;
            tmp = popListOp();
            free(tmp->lp);
            free(tmp);
            tmp = popListInp();
            free(tmp->rp);
            free(tmp);
            break;
        case NUM:
            // transfer it to output
            pushListOutput(popListInp());
            break;
        case OP:
            // complicate logic, reference https://en.wikipedia.org/wiki/Shunting-yard_algorithm
            while (opStack && opStack->EXTYPE == OP && ifPopOp(inputList, opStack)){
                pushListOutput(popListOp());
            }
            pushListOp(popListInp());
            break;
        default:
            fprintf(stderr, "invalid token type\n");
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