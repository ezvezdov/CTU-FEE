#lang racket

;; Exercise 1
;; Return reverted list 
(define (my-reverse lst [acc '()] )
  (cond [(empty? lst) acc]
        [else (my-reverse (list-tail lst 1) (append (list (car lst) ) acc) )])
  )


;;Exercise 2
;; Write a function (letter-frequencies str) which takes a string str and returns a histogram of letters
;; occurring in str so that the most frequent characters come first.
;; (letter-frequencies “good”) ⇒ ((#\o . 2) (#\d . 1) (#\g . 1))
(define (group-same lst [ans '()] [cur '()] )
  (cond [(empty? lst) (append ans (list cur) )] ;; if lst is empty, return answer
        [(empty? cur) (group-same (cdr lst) ans (list (car lst)))] ;; if cur is empty, append to it first element from lst
        [(eqv? (car lst) (car cur) ) (group-same (cdr lst) ans (append cur (list (car lst)))  ) ] ;; if elements from lst and curent char are equals, append to cur first element from lst  
        [else (group-same lst (append ans (list cur) ) '()  )] ;; if elements from lst and cur are different, append cur to ans
         ))

(define (join-lengths lst [ans '()])
  (if (empty? lst)
      ans
      (join-lengths (cdr lst) (append ans (list (cons (car (car lst)) (length (car lst))))) )))

;; string-downcase -> string->list -> filter-alphabetic -> sort -> group-same -> join-lengths -> sort
(define (letter-frequencies str)
  (sort ( join-lengths (group-same (sort (filter char-alphabetic? (string->list (string-downcase str))) char<?)) )
        #:key cdr >))

;; Task 1
;; Compute average of list
( define (average-list lst [amount 0] [sum 0])
   (if (empty? lst)
       (/ sum amount)
       (average-list (cdr lst) (max amount (length lst) ) (+ sum (car lst) )  )
   ))

;; Task 2
;; Split list to n-element tuples
;; Examples:
;;     (split-list 2 '(a b 1 2 3 4)) => ((a b) (1 2) (3 4))
;;     '(a b 1 2)) => ((a b 1) (2))
(define (split-list n lst [ans '()] [cur '()])
  (cond [(empty? lst) (append ans (list cur) )] ;; if lst is empty, return answer
        [(empty? cur) (split-list n (cdr lst) ans (list (car lst)))] ;; if cur is empty, append to it first element from lst
        [(not (= (length cur) n)) (split-list n (cdr lst) ans (append cur (list (car lst)))  ) ] ;; if cur length is less then n, append to cur first element from lst  
        [else (split-list n lst (append ans (list cur) ) '()  )] ;; if cur length == n, append cur to ans
         )
  )

;; Task 3
;; Split a given list of numbers lst into n-tuples of consecutive numbers and returns a list of averages of these n-tuples.
;; Example: (n-block-average 2 '(1 3 1 5)) ⇒ (2 3).
(define (n-block-average n lst [splitted_lst 0] [ans '()])
  (cond [(not (list? splitted_lst)) (n-block-average n lst (split-list n lst) )]
        [(empty? splitted_lst) ans ]
        [else (n-block-average n lst (cdr splitted_lst) (append ans  (list ( average-list (car splitted_lst) )))  ) ]))