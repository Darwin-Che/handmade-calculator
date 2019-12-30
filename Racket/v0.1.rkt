#lang racket
(require "groupnum.rkt")
(require "op-base-v0.rkt")
(require "check-v0.rkt")

(define-struct op-node (op left right))


;; 1. the discriminator of + -
(define (disc+- x) (or (eq? x #\+) (eq? x #\-)))

;; 2. the discriminator of * /
(define (disc*/ x) (or (eq? x #\*) (eq? x #\/)))

;; 3. concat a list according to its last occurrence
(define (concat-last lst f)
  (define (helper acc1 acc2)
    (if (f (car acc1))
        (list (car acc1) (reverse (cdr acc1)) acc2)
        (helper (cdr acc1) (cons (car acc1) acc2))))
  (helper (reverse lst) empty))

