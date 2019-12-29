#lang racket
(require "op-base-v0.rkt")

(provide check)

;; check if it is a legal expression

(define (check lst) (and (check1 lst)
                         (check2 lst)))

;; 1. operation always followed by (/number
(define (check1 lst)
  (let ([tmp (memf op? lst)])
    (cond [(boolean? tmp) #true]
          [(empty? (cdr tmp)) (error "empty after op")]
          [(number? (cadr tmp)) (check1 (cdr tmp))]
          [#true (error "incorrect use of op")])))
; (check1 (groupnum (read-line)))

;; 2. numbers always followed by operation/empty
(define (check2 lst)
  (let ([tmp (memf number? lst)])
    (cond [(boolean? tmp) #true]
          [(or (empty? (cdr tmp)) (op? (cadr tmp))) (check2 (cdr tmp))]
          [#true (error "incorrect use of num")])))
; (check2 (groupnum (read-line)))
