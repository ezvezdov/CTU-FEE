#lang racket
(provide eval state)
(struct state (x y dir) #:transparent)

(define directions '( north east south west) )



(define (make-turn st dir)
  (match st
  [(state x y d)
   (define idx (index-of directions d))
   (define idx_new (modulo (if (eq? dir 'right) (+ idx 1) (- idx 1)) (length directions) ) )
   (state x y (list-ref directions idx_new))
   ]))

(define (make-move st)
  (match st
  [(state x y 'north) (state x (+ y 1) 'north)]
  [(state x y 'east)   (state (+ x 1) y 'east)]
  [(state x y 'south) (state x (- y 1) 'south)]
  [(state x y 'west)   (state (- x 1) y 'west)]
  ))

(define (eval prg st)
  (cond
  [(empty? prg) st]
  [(eq? (car prg) 'move) (eval (rest prg) (make-move st))]
  [else (eval (rest prg) (make-turn st (car prg) ) )]
  ))