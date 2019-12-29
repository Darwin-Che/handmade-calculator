#lang racket
(require "groupnum.rkt")
(require "op-base-v0.rkt")
(require "check-v0.rkt")

(define-struct op-node (op left right))

; helper function

(define (concat-before lst i)
  (if (= i -1)
      empty
      (cons (car lst) (concat-before (cdr lst) (sub1 i)))))

(define (concat-after lst i)
  (if (= i -1)
      lst
      (concat-after (cdr lst) (sub1 i))))

(define (concat-first lst i f) (f lst i))

(define (concat-last lst i f) (reverse (f (reverse lst) i)))

; 1. empty : return (error "empty lst")
; 2. one element : check if it is a number return number
; 3. has +/- : (+/- . (f out1) . (f out2))
; 4. has *// : (*// . (f out1) . (f out2))

(define (+/-? lst) (not (boolean? (memf (λ (x) (or (eq? #\+ x) (eq? #\- x))) lst))))
(define (*//? lst) (not (boolean? (memf (λ (x) (or (eq? #\* x) (eq? #\/ x))) lst))))

(define (cut-lst lst f)
  (define (help acc1 acc2)
    (if (empty? acc1)
        (error "empty acc1")
        (if (f (car acc1))
            (list (car acc1) acc2 (cdr acc1))
            (help (cdr acc1) (cons (car acc1) acc2)))))
  (help lst empty))

(define (list->tree lst)
  (cond [(empty? lst) (error "no expression")]
        [(empty? (cdr lst)) (if (number? (car lst)) (car lst) (error "not a number"))]
        [(+/-? lst)
         (let ([tmp (cut-lst lst (λ (x) (or (eq? #\+ x) (eq? #\- x))))])
           (make-op-node (first tmp) (list->tree (second tmp)) (list->tree (third tmp))))]
        [(*//? lst)
         (let ([tmp (cut-lst lst (λ (x) (or (eq? #\* x) (eq? #\/ x))))])
           (make-op-node (first tmp) (list->tree (second tmp)) (list->tree (third tmp))))]
        [#true (error "we are completely screwed")]))

(define (calc-n node)
  (if (number? node)
      node
      ((op-trans (op-node-op node))
       (calc-n (op-node-left node))
       (calc-n (op-node-right node)))))

(define calc (λ () (calc-n (list->tree (reverse (groupnum (read-line)))))))

;(define x (reverse (groupnum (read-line))))
(calc)

