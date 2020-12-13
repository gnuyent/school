#lang scheme
; Program 6
; A prototype for program 6 in Scheme
; CS320
; December 9, 2020
; @author Brandon Nguyen cssc2650
;
; Edit this file to add your documentation and function definitions.
; Leave the rest of this file unchanged.
;
; To run this file, you would start scheme at edoras command line prompt:
; scheme --load p6.scm, where the file is in the current directory
; and then in scheme type the load command (from the '%' prompt):
;(load "p6.scm")
;
; Defined LISTS for use with testing your functions.
; Defined LISTS for use with testing your functions.
(define list0 (list 'j 'k 'l 'm 'n 'o 'j) )
(define list1 (list 'a 'b 'c 'd 'e 'f 'g) )
(define list2 (list 's 't 'u 'v 'w 'x 'y 'z) )
(define list3 (list 'j 'k 'l 'm 'l 'k 'j) )
(define list4 (list 'n 'o 'p 'q 'q 'p 'o 'n) )
(define list5 '((a b) c (d e d) c (a b)) )
(define list6 '((h i) (j k) l (m n)) ) 
(define list7 '(f (a b) c (d e d) (b a) f) )
;
; Here is a typical function definition from Sebesta Ch. 15
(define (adder lis)
  (cond
      ((null lis) 0)
      	(else (+ (car lis) (adder (cdr lis))))
))
; The above five lines are the sort of definition you would need to add to
; this file if asked to define an ADDER function.
; Uncomment and complete the following four definitions. At least have ODDS
; so the program can be tested.

; evens
(define (evens lst)
  (if (list? lst) ; Do we have a list?
      (if (or (null? lst) ; Is this list null?
              (null? (cdr lst))) ; Or does it contain only 1 element?
          '() ; Then return an empty list
          (cons (cadr lst) ; Else, construct a list with list[1] and
                (evens (cddr lst)))) ; recur with list[2:]
      (error "USAGE: (evens {list})") ; NOT A LIST
  )
)

; oddrev
; Gets odds
(define (odds lst); Do we have a list?
  (if (or (null? lst)
      (null? (cdr lst))) ; Is it null or 1 element?
      lst
      (cons (car lst) (odds (cddr lst))) ; Construct a list
  )
)

; HELPER, reverses a list
; adapted from multiple StackOverflow posts
(define (reverse lst)
  (let loop ((lst lst) (memo '())) ; pass already built list
    (if (null? lst) ; check if list is null
        memo
        (loop (cdr lst) (cons (car lst) memo))))) ; pass lst[1:]
      

(define (oddrev lst)
  (if (list? lst) ; Is this a list?
      (reverse (odds lst))
      (error "USAGE: (oddrev {list})"))) ; We don't have a list, print an error message

; middle
; get length of list
(define (length lst)
  (cond
    ((null? lst) 0)
    (else (+ 1 (length (cdr lst))))))

(define (middle lst)
  (if (list? lst) ; Check if this is valid list
      (if (eq? 1 (remainder (length lst) 2)) ; Check if it is even
       (list (list-ref lst (quotient (length lst) 2))) ; If odd, return the middle val
       '()) ; Else, return empty list.
      (error "USAGE: (middle {list})"))) ; We don't have a list, print an error message

; bookends
(define (bookends lst)
  (if (list? lst)
      (if (or (null? lst)
              (null? (cdr lst))) ; if there are 0, 1 elements
          #t
          (if (eq? (car lst) (car (reverse lst)))
              #t
              #f)
          )
      (error "USAGE: (bookends {list})"))) ; We don't have a list, print an error message