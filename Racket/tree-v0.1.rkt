#lang racket
(require "src/groupnum.rkt")
(require "src/op-base-v0.rkt")
(require "src/check-v0.rkt")

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

(define (list->tree lst) 
  ;; 1. if lst is empty, error, empty expression
  ;; 2. if lst has one element, if it is number, return itself, else, error
  ;; 3. if lst has + -, convert the last occurrence
  ;; 4. if lst has * /, convert the last occurrence
  ;; 5. #true we are screwed
  (cond [(empty? lst) (error "empty expression")]
        [(empty? (cdr lst)) (if (number? (car lst)) (car lst) (error "not a number"))]
        [(not (boolean? (memf disc+- lst)))
         (let ([tmp (concat-last lst disc+-)])
           (make-op-node (first tmp) (list->tree (second tmp)) (list->tree (third tmp))))]
        [(not (boolean? (memf disc*/ lst)))
         (let ([tmp (concat-last lst disc*/)])
           (make-op-node (first tmp) (list->tree (second tmp)) (list->tree (third tmp))))]
        [#true (error "we are screwed")]))

(define (calc-n node)
  (if (number? node)
      node
      ((op-trans (op-node-op node))
       (calc-n (op-node-left node))
       (calc-n (op-node-right node)))))

(define calc (λ () (calc-n (list->tree (groupnum (read-line))))))
