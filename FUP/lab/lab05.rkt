#lang racket

; Exercise 1
; Adding two infinite streams together component-wise
(define (stream-add s1 s2)
  (stream-cons (+ (stream-first s1) (stream-first s2) )
               (stream-add (stream-rest s1) (stream-rest s2))))

; Fibonacci stream using stream-add
(define fib-stream
  (stream-cons 0 (stream-cons 1 (stream-add fib-stream (stream-rest fib-stream) ))))

(struct graph (nodes edges))

(define (find-hamiltonian-path g))









; TODO: Task 1
; Multiply streams elements-wise
(define (stream-mul s1 s2)
  (stream-cons (* (stream-first s1) (stream-first s2) )
               (stream-mul (stream-rest s1) (stream-rest s2))))

(define factorial-stream
  (stream-cons) )

