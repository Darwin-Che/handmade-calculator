#lang racket

(provide groupnum)

;; group together the numbers, chars including 1234567890.
;; using the Gen and cont style of programming

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