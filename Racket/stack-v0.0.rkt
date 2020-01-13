#lang racket
(require "src/groupnum.rkt")
(require "src/op-base-stack-v0.0.rkt")
(require "src/check-v0.rkt")

;; stack is a list

(define calc (λ () (calc-stack (lst->RPN (groupnum (read-line))))))

;for each token in the postfix expression:
;  if token is an operator:
;    operand_2 ← pop from the stack
;    operand_1 ← pop from the stack
;    result ← evaluate token with operand_1 and operand_2
;    push result back onto the stack
;  else if token is an operand:
;    push token onto the stack
;result ← pop from the stack

(define (calc-stack stk)
  (define (help stk tmp)
    (if (empty? stk)
        (if (empty? tmp)
            (error "should not happen")
            (if (and (empty? (cdr tmp)) (number? (car tmp)))
                (car tmp)
                (error "result not a number")))
        (if (number? (car stk))
            (help (cdr stk) (cons (car stk) tmp))
            (if (op? (car stk))
                (help (cdr stk) (cons ((op-trans (car stk)) (cadr tmp) (car tmp)) (cddr tmp)))
                (error "neither op nor number")))))
  (help stk empty))

(define (rappend a lst)
  (if (empty? lst)
      a
      (rappend (cons (car lst) a) (cdr lst))))

(define (rightbrac-helper lst stk op-tmp)
  (cond [(empty? op-tmp) (error "missed matched parentheses")]
        [(eq? #\( (car op-tmp))
         (lst->RPN-help (cdr lst) stk (cdr op-tmp))]
        [#true (rightbrac-helper lst (cons (car op-tmp) stk) (cdr op-tmp))]))

(define (op-helper lst stk op-tmp)
  (if (and (not (empty? op-tmp))
           (or (grtr-preced (car op-tmp) (car lst))
               (and (eql-preced (car op-tmp) (car lst)) (leftasso (car op-tmp))))
           (not (eq? (car op-tmp) #\()))
      (op-helper lst (cons (car op-tmp) stk) (cdr op-tmp))
      (lst->RPN-help (cdr lst) stk (cons (car lst) op-tmp))))

;while there are tokens to be read do:
;    read a token.
;    if the token is a number, then:
;        push it to the output queue.
;    if the token is a function then:
;        push it onto the operator stack 
;    if the token is an operator, then:
;        while ((there is a function at the top of the operator stack)
;               or (there is an operator at the top of the operator stack with greater precedence)
;               or (the operator at the top of the operator stack has equal precedence and is left associative))
;              and (the operator at the top of the operator stack is not a left parenthesis):
;            pop operators from the operator stack onto the output queue.
;        push it onto the operator stack.
;    if the token is a left paren (i.e. "("), then:
;        push it onto the operator stack.
;    if the token is a right paren (i.e. ")"), then:
;        while the operator at the top of the operator stack is not a left paren:
;            pop the operator from the operator stack onto the output queue.
;        /* if the stack runs out without finding a left paren, then there are mismatched parentheses. */
;        if there is a left paren at the top of the operator stack, then:
;            pop the operator from the operator stack and discard it
;/* After while loop, if operator stack not null, pop everything to output queue */
;if there are no more tokens to read then:
;    while there are still operator tokens on the stack:
;        /* if the operator token on the top of the stack is a paren, then there are mismatched parentheses. */
;        pop the operator from the operator stack onto the output queue.
;exit.
 
(define (lst->RPN-help lst stk op-tmp)   ; original lst, stack, tmp list for operations
  (if (empty? lst) (rappend op-tmp stk)  ; if all is translated, then reverse stack and append to op-tmp, which is equal to reverse the result of append a reversed op-tmp to the end of stack
      (cond [(number? (car lst))          ; if current element is a number
             (lst->RPN-help (cdr lst) (cons (car lst) stk) op-tmp)]  ; send it to stk
            [(op? (car lst))              ; if current element is an operation
             (op-helper lst stk op-tmp)]     ; call helper
            [(eq? #\( (car lst))          ; if current element is (
             (lst->RPN-help (cdr lst) stk (cons (car lst) op-tmp))]   ; send it to op-tmp
            [(eq? #\) (car lst))          ; if current element is )
             (rightbrac-helper lst stk op-tmp)]  ; call helper
            [#true (error "screwed")])))

(define (lst->RPN lst)
  (lst->RPN-help lst empty empty))

;(define x (list 1 #\+ #\( 2 #\+ 3 #\* 4 #\)))
;(define y (list 1 2 3 4 #\* #\+ #\+))