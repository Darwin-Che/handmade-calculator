#include <stdio.h>
#include <stdlib.h>
#include "evalrpn.h"

int main(){
    char input[150];
    scanf("%149s", input);
    // malloc a TokenList
    inputList = tokenize(input);
    // donot malloc, but inputList and opStack are empty, tranfered to outputList
    // free all parenthesis
    shuntYard();
    // reverse the outputList, name it evalIn
    // donot change the evalIn
    // malloc and free evalOut
    eval(outputList);
    // print the result
    if (evalOut->n.NUMTYPE == INT){
        printf("%d\n", evalOut->n.intnum);
    } else {
        printf("%lf\n", evalOut->n.doublenum);
    }
    // free evalOut
    free(evalOut);
    freeTL(evalin);
    return 0;
}