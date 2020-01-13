#lang racket

(provide op-lst op? op-trans disc+- disc*/ grtr-preced eql-preced leftasso)

(define op-lst (list #\+ #\- #\* #\\))
(define (op? x) (if (boolean? (member x op-lst)) #false #true))
(define (op-trans x)
  (cond [(eq? x #\+) (λ (a b) (+ a b))]
        [(eq? x #\-) (λ (a b) (- b a))]
        [(eq? x #\*) (λ (a b) (* a b))]
        [(eq? x #\/) (λ (a b) (/ b a))]
        [#true (error "some strange op-sign")]))
;; 1. the discriminator of + -
(define (disc+- x) (or (eq? x #\+) (eq? x #\-)))

;; 2. the discriminator of * /
(define (disc*/ x) (or (eq? x #\*) (eq? x #\/)))

(define (grtr-preced a b)
  (cond [(and (disc*/ a) (disc+- b)) #true]
        [#true #false]))

(define (eql-preced a b)
  (cond [(and (disc*/ a) (disc*/ b)) #true]
        [(and (disc+- a) (disc+- b)) #true]
        [#true #false]))

(define (leftasso x) #true)