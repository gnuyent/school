#lang scheme

;; Question 1
'(7 a -12 ????)

;; Question 2
(define z '(a 5 12))
(symbol? (list-ref z 0))

;; Question 3
(define dozen 12)
(number? dozen)

;; Question 4
(list '())
'(())

;; Question 5
(define days-in-semester (+ 30 31 30 15))

;; Question 6
((lambda (x) (+ x x)) 4)

;; Question 7
(define reverse-subtract
  (lambda (x y)
    (- y x)))
(reverse-subtract 7 10)

;; Question 8
(let loop
  ((numbers '(3 -2 1 6 -5))
   (nonneg '())
   (neg '()))
  (cond ((null? numbers)
         (list nonneg neg))
        ((>= (car numbers) 0)
         (loop (cdr numbers)
               (cons (car numbers) nonneg)
               neg))
        (else
         (loop (cdr numbers)
               nonneg
               (cons (car numbers) neg)))))

;; Question 9
(define (tally atm list)
   (cond
     ( (null? list) 0 ) 
     ( (eq? atm (car list)) (+ 1 (tally atm (cdr list))))
     ( else (tally atm (cdr list)))
     ))

(tally 0 '(4 0 3 0 2 0 1))

;; Lexical Bindings: Part A
(let ((x 2) (y 3))
  (* x y))
;; Lexical Bindings: Part B
(let ((x 2) (y 3))
  (let ((foo (lambda (z) (+ x y z)))
    (x 7))
    ;call foo
    (foo 4)))