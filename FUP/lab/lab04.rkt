#lang racket


;; Exercise 1
;; Example:
;; (2 3) => ((1 2 3) (2 1 3) (2 3 1))
;; (3 2) => ((1 3 2) (3 1 2) (3 2 1))

(define ((interleave el) lst)
  (if (null? lst)
      (list (list el))
      (cons (cons el lst)
            (map (curry cons (car lst))
                 ((interleave el) (cdr lst))))))

(define (permutations lst)
  (if (null? lst )
      '(())
      (map (interleave (car lst))
           (permutations (cdr lst)))))

;; Exercise 2

;; (node-var n) -> (define (node-var n) (car n))
;; (node-left n) -> left

(struct node (var left right) #:transparent)

(define bool-tree
  (node 'x1
        (node 'x2
              (node 'x3 1 0)
              (node 'x3 0 1))
        (node 'x2
              (node 'x3 0 0)
              (node 'x3 1 1))))

(define (evaluate tree vals)
  (match vals
    [(list) tree]
    [(list 0 vs ...) (evaluate (node-left tree) vs)]
    [(list 1 vs ...) (evaluate (node-right tree) vs)]))

(evaluate bool-tree '(1 0 1))

(define (evaluate2 tree vals)
  (define (left-right x)
    (if (zero? x) node-left node-right))
  (apply compose (map left-right (reverse vals)) tree))

(evaluate bool-tree '(1 0 1))

;; Task 1
(define (sub-seq lst) 1)