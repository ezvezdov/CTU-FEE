#lang racket

;; Exercise 1
;; Implement a function (mult-all-pairs lst1 lst2) taking two lists and returning a list of all possible
;; binary products[sum/division...] between elements from lst1 and elements from lst2
;; Examples:
;; (f-all-pairs cons '(1 2 3) '(a b)) ⇒ ((1 . a) (1 . b) (2 . a) (2 . b) (3 . a) (3 . b)
;; (f-all-pairs * '(1 2 3) '(-2 0)) ⇒ (-2 0 -4 0 -6 0)
;; Comments:
;; (((curry f) number1) number2) => apply function (f[*+-/...] number1 number2)
(define (f-all-pairs f lst1 lst2)
  ( apply append (map
                  (lambda (number1) (map
                                     (lambda (number2)  (((curry f) number1) number2)) lst2 ) ) lst1 )) )


;; Exercise 2

(define (get-coef m) (car m)) ; first component
(define (get-exp m) (cadr m)) ; second component

;; Multiplication of monomials
(define (mult-mon m1 m2) (list (* (get-coef m1) (get-coef m2)) ( + (get-exp m1) (get-exp m2))) )
(define (add-mon m1 m2) (list (+ (get-coef m1) (get-coef m2)) (get-exp m1)) )

(define (add-mon-pol mon pol)
  (define (same-exp? m) (= (get-exp mon) (get-exp m))) ; #t if m has the same exponent as mon
  (define same-mon (filter same-exp? pol))             ; list containing the monomial of the same exponent or empty list
  (define rest (filter (compose not same-exp?) pol))   ; remaining monomials of different exponents
  (if (null? same-mon)                                 
      (cons mon rest)
      (cons (add-mon mon (car same-mon)) rest)))

(define (normalize p)
  (define (non-zero-coef? m) (not (= 0 (get-coef m)))) 
  (sort
   (filter non-zero-coef? p)
   (lambda (p1 p2) (< (get-exp p1) (get-exp p2)))))
 
(define (add-pol p1 p2)
  (normalize (foldl add-mon-pol p1 p2)))
 
(define (mult-pol p1 p2)
  (normalize (foldl add-mon-pol '() (f-all-pairs mult-mon p1 p2))))


;; Task 1
;; Function linear-combination taking a list of vectors, a list of coefficients and
;; returning the corresponding linear combination.
;; The function should be created in the curried form (the list of vectors being the first argument).
;; Examples: 
;; linear combination 2⋅(1,2,3)-1⋅(1,0,1)+3⋅(0,2,0)=(1,10,5)
;; ((linear-combination '((1 2 3) (1 0 1) (0 2 0))) '(2 -1 3))  => (1 10 5)
(define (lin-mult v s)
  (map (lambda (v_number) (* s v_number) ) v) )
(define (sum-two-vectors v1 v2)
  (map (lambda (i) (+ (list-ref v1 i) (list-ref v2 i) ) ) (range 0 (length v1) )))
(define (multiplied_vector v1 v2)
  (map lin-mult v1 v2) )

(define (linear-combination v1)
  (lambda (v2)
    (define multiplied_vector (map lin-mult v1 v2) )
    (foldl sum-two-vectors (car multiplied_vector) (cdr multiplied_vector) )
    ))

;; Task 2
;; 1. matrix-mult compute the matrix multiplication
;; Example:  (matrix-mult '((1 2 3) 
;;                          (-1 0 2))
;;                        '((1 -1) 
;;                          (2 0) 
;;                          (0 3))) => ((5 8) (-1 7)) 
;; 2. matrix-power compute matrix on power
;; Example: (matrix-power 3 '((2 3) (0 -1))) => ((8 9) (0 -1))
(define (matrix-mult m1 m2)
  ( map (linear-combination m2) m1))
(define  (matrix-power p m)
  (foldl matrix-mult m (make-list (- p 1) m) ))



