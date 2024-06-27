#lang racket
;; Check if number is even
(define (my-even? x)
  (cond [(= x 0) #t]
        [(= x 1) #f]
        [else (my-even? (- x 2))]))

;; Multiply string
(define (copy-str n str)
  (if (= n 1) str (string-append (copy-str (- n 1) str) str)))

;; Multiply string using tail-recursion
(define (copy-str-tail n str [acc ""])
  (if (= n 0) acc (copy-str-tail (- n 1) str (string-append acc str))))

;; Make char consequence from fst to lst (a-z or z-a for example)
(define (consecutive-chars fst lst)
  (cond 
        [(char=? fst lst) (string fst)]
        [(char<? fst lst) (string-append (string fst) (consecutive-chars (integer->char (+ 1 (char->integer fst)) ) lst))]
        [(char>? fst lst) (string-append (string fst) (consecutive-chars (integer->char (- (char->integer fst) 1) ) lst))]))

;; Task 1
;; Get number of digits of n
(define (num-of-digits n)
  (if (not (= (quotient n 10) 0)) (+ (num-of-digits (quotient n 10)) 1) 1))

;; Task 2
;; Number system conversion
(define (num->str n [radix 10])
  (cond [(= n 0) "0"]
        [(> (remainder n radix) 9) (string-append (if (= (quotient n radix) 0) "" (num->str (quotient n radix) radix)) (string ( integer->char (+ (char->integer #\A) (- (remainder n radix) 10) ))))]
        [else (string-append (if (= (quotient n radix) 0) "" (num->str (quotient n radix) radix)) (string ( integer->char (+ (char->integer #\0) (remainder n radix)))))]))
