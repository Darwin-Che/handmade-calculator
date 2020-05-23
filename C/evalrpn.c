#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "evalrpn.h"

struct TokenList *evalin = NULL;
struct NumberList *evalOut = NULL;

void printNL(struct NumberList* e){
    while (e) {
        printf(",");
        switch (e->n.NUMTYPE)
        {
        case INT:
            printf("%d", e->n.intnum);
            break;
        case DOUBLE:
            printf("%lf", e->n.doublenum);
            break;
        default:
            fprintf(stderr, "print Number have wierd type\n");
            exit(EXIT_FAILURE);
            break;
        }
        e=e->next;
    }
    printf("\n");
}

struct TokenList *reverseList(struct TokenList *src){
    if (src == NULL) return NULL;
    struct TokenList *prev = NULL;
    struct TokenList *cur = src;
    struct TokenList *next = src->next;
    while (next) {
        cur->next = prev;
        prev = cur;
        cur = next;
        next = next->next;
    }
    cur->next = prev;
    return cur; 
}

struct Number binOp(struct Number n1, struct Number n2, int INS){
    struct Number ans;
    switch (INS)
    {
    case ADD:
        if (n1.NUMTYPE == INT && n2.NUMTYPE == INT) {
            ans.NUMTYPE = INT;
            ans.intnum = n1.intnum + n2.intnum;
        } else {
            ans.NUMTYPE = DOUBLE;
            ans.doublenum = (n1.NUMTYPE == INT ? n1.intnum : n1.doublenum) 
                          + (n2.NUMTYPE == INT ? n2.intnum : n2.doublenum);
        }
        break;
    case SUB: 
        if (n1.NUMTYPE == INT && n2.NUMTYPE == INT) {
            ans.NUMTYPE = INT;
            ans.intnum = n1.intnum - n2.intnum;
        } else {
            ans.NUMTYPE = DOUBLE;
            ans.doublenum = (n1.NUMTYPE == INT ? n1.intnum : n1.doublenum) 
                          - (n2.NUMTYPE == INT ? n2.intnum : n2.doublenum);
        }
        break;
    case MUL:
        if (n1.NUMTYPE == INT && n2.NUMTYPE == INT) {
            ans.NUMTYPE = INT;
            ans.intnum = n1.intnum * n2.intnum;
        } else {
            ans.NUMTYPE = DOUBLE;
            ans.doublenum = (n1.NUMTYPE == INT ? n1.intnum : n1.doublenum) 
                          * (n2.NUMTYPE == INT ? n2.intnum : n2.doublenum);
        }
        break;
    case DIV:
        ans.NUMTYPE = DOUBLE;
        ans.doublenum = (n1.NUMTYPE == INT ? n1.intnum : n1.doublenum) 
                      / (n2.NUMTYPE == INT ? n2.intnum : n2.doublenum);
        break;
    case POW:
        if (n1.NUMTYPE == INT && n2.NUMTYPE == INT) {
            ans.NUMTYPE = INT;
            ans.intnum = pow(n1.intnum, n2.intnum);
        } else {
            ans.NUMTYPE = DOUBLE;
            ans.doublenum = pow((n1.NUMTYPE == INT ? n1.intnum : n1.doublenum), 
                                (n2.NUMTYPE == INT ? n2.intnum : n2.doublenum));
        }
        break;
    default:
        fprintf(stderr, "invalid BinOp\n");
        exit(EXIT_FAILURE);
        break;
    }
    return ans;
}

struct Number unaOp(struct Number n1, int INS){
    struct Number ans;
    switch (INS)
    {
    case LOG:
        ans.NUMTYPE = DOUBLE;
        ans.doublenum = log(n1.doublenum);
        break;
    default:
        fprintf(stderr, "invalid UnaryOp\n");
        exit(EXIT_FAILURE);
        break;
    }
    return ans;
}

void eval(struct TokenList *input){
    evalin = reverseList(input);
    struct TokenList *intmp = evalin;
    struct NumberList *outtmp;
    while (intmp) {
        if (intmp->EXTYPE == NUM) {
            outtmp = malloc(sizeof(struct NumberList));
            if ((outtmp->n.NUMTYPE = intmp->n->NUMTYPE) == INT) {
                outtmp->n.intnum = intmp->n->intnum;
            } else {
                outtmp->n.doublenum = intmp->n->doublenum;
            }
            outtmp->next = evalOut;
            evalOut = outtmp;
        } else if (intmp->EXTYPE == OP) {
            switch (intmp->op->op)
            {
            case ADD:
            case SUB:
            case MUL:
            case DIV:
            case POW:
                outtmp = malloc(sizeof(struct NumberList));
                outtmp->n = binOp(evalOut->next->n, evalOut->n, intmp->op->op);
                outtmp->next = evalOut->next->next;
                free(evalOut->next);
                free(evalOut);
                evalOut = outtmp;
                break;
            case LOG:
                outtmp = malloc(sizeof(struct NumberList));
                outtmp->n = unaOp(evalOut->n, intmp->op->op);
                outtmp->next = evalOut->next;
                free(evalOut);
                evalOut = outtmp;
                break;
            default:
                fprintf(stderr, "invalid Op in evalIn\n");
                exit(EXIT_FAILURE);
                break;
            }
        }
        intmp= intmp->next;
    }
    if (evalOut->next) {
        fprintf(stderr, "after computation left with something more than a number\n");
        exit(EXIT_FAILURE);
    }
}

/* void test(){
    char s[30] = "(1+3)*5";
    struct TokenList *t = tokenize(s);
    // printf("%d\n", t->EXTYPE);
    // printTL(t);
    // printf("\n-----------------------\n");
    shuntYard(t);
    printTL(outputList);
    eval(outputList);
    printNL(evalOut);
} */