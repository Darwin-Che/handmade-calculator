#lang racket

(provide groupnum)

;; group together the numbers, chars including 1234567890.
;; using the Gen and cont style of programming

(define op-lst (list #\+ #\- #\* #\\))

(define (op? x) (if (boolean? (member x op-lst)) #false #true))

(define number-lst (list #\0 #\1 #\2 #\3 #\4 #\5 #\6 #\7 #\8 #\9 #\.))

(define (Gen inp state step)
  (define (cont newinp state out)
     (append out (Gen (append newinp (cdr inp)) state step)))
  (step (if (empty? inp) empty (take inp 1)) state cont))

(define (my-step inp state cont)
  (if (empty? inp)
      (if (empty? state) empty
          (list (string->number (list->string (reverse state)))))
      (if (boolean? (member (car inp) number-lst))
          (if (empty? state)
              (cont empty empty (list (car inp)))
              (cont empty empty (list (string->number (list->string (reverse state))) (car inp))))
          (cont empty (cons (car inp) state) empty))))

(define (groupnum lst) (Gen (string->list lst) empty my-step))

;; check if it is a legal expression

;(define (check lst))

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

(define-struct op-node (op left right))

; helper function
; 1. detect has brackets
; 2. get ()'s first appearence's index/ reverse index
; 3. concat lst forward/backward

(define (detect-brac lst) (not (boolean? (memf brac? lst))))

(define (get-first-brac lst)
  (define (help lst i)
    (if (empty? lst)
        (error "empty expression 2-1")
        (if (eq? (car lst) #\()
            i
            (help (cdr lst) (add1 i)))))
  (help lst 0))

(define (get-last-brac lst)
  (define (help lst i)
    (if (empty? lst)
        (error "empty expression 2-2")
        (if (eq? (car lst) #\))
            i
            (help (cdr lst) (add1 i)))))
  (help (reverse lst) 0))

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
; 3. has brac : detect ( and ), then
;         1) if the first is empty:
;              1a) if the second is empty: return (f the expression inside)
;              1b) if the second is not empty: return (the op after second . (f in brac) . (f out brac))
;         2) if the first is not empty:
;              2a) if the second is empty: return (op before first . out brac . inbrac)
;              2b) if the second is not empty:
;                   1) if the first op is +/- : (first op . (f out brac) . (second op . (f in brac) . (f out brac)))
;                   2) else : (second op . (first op . (f out brac) . (f in brac)) . (f out brac))
; 4. has +/- : (+/- . (f out1) . (f out2))
; 5. has *// : (*// . (f out1) . (f out2))


(define (lst->node lst)
  (cond [(empty? lst) (error "empty expression")]
        [(empty? (cdr lst)) (if (number? (car lst)) (car lst) (error "not a number"))]
        [(detect-brac lst)
         (let ([index1 (get-first-brac lst)]
               [index2 (get-last-brac lst)]
               [rlst (reverse lst)])
           (if (zero? index1)
               (if (zero? index2)
                   (lst->node (reverse (cdr (reverse (cdr lst)))))
                   (make-op-node (list-ref rlst (sub1 index2))
                                 (concat-last lst index2 concat-after)
                                 (concat-last lst (- index2 2) concat-before)))
               (if (zero? index2)
                   (make-op-node (list-ref lst (sub1 index1))
                                 (concat-first lst index1 concat-before))))



