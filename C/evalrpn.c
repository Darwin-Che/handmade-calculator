#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "evalrpn.h"

// the reverse of result of rpn
// initiated by eval
struct TokenList *evalin = NULL;
// final result
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

// precond: INS is a binary operator
// return a Number struct that is the result of the operation
// the Number NUMTYPE is determined with respect to the operator and the NUMTYPE of each argument
struct Number binOp(struct Number n1, struct Number n2, int INS){
    // the answer
    struct Number ans;
    switch (INS)
    {
    case ADD:
        if (n1.NUMTYPE == INT && n2.NUMTYPE == INT) {
            // if n1 and n2 are integers, result can be stored in integer
            ans.NUMTYPE = INT;
            ans.intnum = n1.intnum + n2.intnum;
        } else {
            // if one of them is double, result should be stored in double
            ans.NUMTYPE = DOUBLE;
            ans.doublenum = (n1.NUMTYPE == INT ? n1.intnum : n1.doublenum) 
                          + (n2.NUMTYPE == INT ? n2.intnum : n2.doublenum);
        }
        break;
    case SUB: 
        // similar to ADD
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
        // similar to ADD
        if (n1.NUMTYPE == INT && n2.NUMTYPE == INT) {
            ans.NUMTYPE = INT;
            ans.intnum = n1.intnum * n2.intnum;
        } else {
            // although possible to store in integer like 3 * 0.3333
            // consider double as not accurate number and int as accurate number
            // we should carry the sign of inaccuracy of the arg to the result
            // so always use double
            ans.NUMTYPE = DOUBLE;
            ans.doublenum = (n1.NUMTYPE == INT ? n1.intnum : n1.doublenum) 
                          * (n2.NUMTYPE == INT ? n2.intnum : n2.doublenum);
        }
        break;
    case DIV:
        // similar to MULT
        // but only store in integer when n1 divides n2
        // cast error when n2 is zero
        if ((n2.NUMTYPE == INT ? n2.intnum : n2.doublenum) == 0){
            fprintf(stderr, "divide by 0\n");
            exit(EXIT_FAILURE);
        } else if (n1.NUMTYPE == INT && n2.NUMTYPE == INT && n1.intnum % n2.intnum == 0) {
            ans.NUMTYPE = INT;
            ans.intnum = n1.intnum / n2.intnum;
        } else {
            ans.NUMTYPE = DOUBLE;
            ans.doublenum = (n1.NUMTYPE == INT ? n1.intnum : n1.doublenum) 
                          / (n2.NUMTYPE == INT ? n2.intnum : n2.doublenum);
        }
        break;
    case POW:
        // similar to ADD
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

// similar to binOp but accpet unary operator
struct Number unaOp(struct Number n1, int INS){
    struct Number ans;
    ans.NUMTYPE = DOUBLE;
    switch (INS)
    {
    case LOG:
        // always use double for result of LOG
        if (n1.NUMTYPE == INT) {
            ans.doublenum = log(n1.intnum);
        } else {
            ans.doublenum = log(n1.doublenum);
        }
        break;
    default:
        fprintf(stderr, "invalid UnaryOp\n");
        exit(EXIT_FAILURE);
        break;
    }
    return ans;
}

void eval(struct TokenList *input){
    // the reverse list function does not malloc, change source directly
    evalin = reverseList(input);
    // for tracking the index of evalin
    // NOTE: we donot change evalin from now on, need to free it later.
    struct TokenList *intmp = evalin;
    // for tmp storage of evalout node
    struct NumberList *outtmp;
    while (intmp) {
        if (intmp->EXTYPE == NUM) {
            // copy the node to evalout, use malloc
            outtmp = malloc(sizeof(struct NumberList));
            if ((outtmp->n.NUMTYPE = intmp->n->NUMTYPE) == INT) {
                outtmp->n.intnum = intmp->n->intnum;
            } else {
                outtmp->n.doublenum = intmp->n->doublenum;
            }
            outtmp->next = evalOut;
            evalOut = outtmp;
        } else if (intmp->EXTYPE == OP) {
            // do operation on the arguments from evalout and replace the arguments with result
            switch (intmp->op->op)
            {
            // these are the binary ops
            case ADD:
            case SUB:
            case MUL:
            case DIV:
            case POW:
                // we should have at least 2 numbers in evalOut
                if (!evalOut || !(evalOut->next)){
                    fprintf(stderr, "not enough operands\n");
                    exit(EXIT_FAILURE);
                }
                outtmp = malloc(sizeof(struct NumberList));
                outtmp->n = binOp(evalOut->next->n, evalOut->n, intmp->op->op);
                outtmp->next = evalOut->next->next;
                // free the args
                free(evalOut->next);
                free(evalOut);
                evalOut = outtmp;
                break;
            // these are the unary ops
            case LOG:
                if (!evalOut){
                    fprintf(stderr, "not enough operands\n");
                    exit(EXIT_FAILURE);
                }
                outtmp = malloc(sizeof(struct NumberList));
                outtmp->n = unaOp(evalOut->n, intmp->op->op);
                outtmp->next = evalOut->next;
                // free args
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
    // we should end up with only one Number
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