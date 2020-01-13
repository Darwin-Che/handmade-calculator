#lang racket

(provide op-lst op? op-trans)

(define op-lst (list #\+ #\- #\* #\\))
(define (op? x) (if (boolean? (member x op-lst)) #false #true))
(define (op-trans x)
  (cond [(eq? x #\+) (λ (a b) (+ a b))]
        [(eq? x #\-) (λ (a b) (- b a))]
        [(eq? x #\*) (λ (a b) (* a b))]
        [(eq? x #\/) (λ (a b) (/ b a))]
        [#true (error "some strange op-sign")]))