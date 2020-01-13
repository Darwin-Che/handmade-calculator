#lang racket
(require "op-base-v0.rkt")
(provide check)

;; check if it is a legal expression

(define (check lst) (and (check1 lst)
                         (check2 lst)
                         (check3 lst)
                         (check4 lst)
                         (check5 lst 0)))

(define (brac? x) (or (eq? x #\() (eq? x #\))))
(define (bracl? x) (eq? x #\())
(define (bracr? x) (eq? x #\)))

;; 1. ( always followed by number/-
(define (check1 lst)
  (let ([tmp (member #\( lst)])
    (cond [(boolean? tmp) #true]
          [(empty? (cdr tmp)) (error "empty after (")]
          [(or (number? (cadr tmp)) (eq? #\- (cadr tmp)) (eq? #\+ (cadr tmp)) (eq? #\( (cadr tmp))) (check1 (cdr tmp))]
          [#true (error "incorrect use of (")])))
; (check1 (groupnum (read-line)))

;; 2. ) always followed by operation/empty
(define (check2 lst)
  (let ([tmp (member #\) lst)])
    (cond [(boolean? tmp) #true]
          [(or (empty? (cdr tmp)) (op? (cadr tmp)) (eq? #\) (cadr tmp))) (check2 (cdr tmp))]
          [#true (error "incorrect use of )")])))
; (check2 (groupnum (read-line)))

;; 3. operation always followed by (/number
(define (check3 lst)
  (let ([tmp (memf op? lst)])
    (cond [(boolean? tmp) #true]
          [(empty? (cdr tmp)) (error "empty after op")]
          [(or (number? (cadr tmp)) (eq? #\( (cadr tmp))) (check3 (cdr tmp))]
          [#true (error "incorrect use of op")])))
; (check3 (groupnum (read-line)))

;; 4. numbers always followed by operation/empty
(define (check4 lst)
  (let ([tmp (memf number? lst)])
    (cond [(boolean? tmp) #true]
          [(or (empty? (cdr tmp)) (op? (cadr tmp)) (eq? #\) (cadr tmp))) (check4 (cdr tmp))]
          [#true (error "incorrect use of num")])))
; (check4 (groupnum (read-line)))

;; 5. check brackets are corresponding, that is number of ( - number of ) >=0 for all, and =0 at end
(define (check5 lst i)
  (let ([tmp (memf brac? lst)])
    (cond [(< i 0) #false]
          [(boolean? tmp) (if (= i 0) #true (error "notequalbracs"))]
          [(bracl? (car tmp)) (check5 (cdr tmp) (add1 i))]
          [(bracr? (car tmp)) (check5 (cdr tmp) (sub1 i))]
          [#true (error "we are screwed")])))
; (check5 (groupnum (read-line)) 0)