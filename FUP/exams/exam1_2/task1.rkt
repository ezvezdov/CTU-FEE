#lang racket
(provide F)

(define (get-next lst N [a 1])
  (cond
    [(empty? lst) 0]
    [(eq? (car lst) N) a]
    [else (get-next (rest lst) N (+ 1 a))]
      )
  )

(define (make-new lst)
  (if (empty? lst) '()
      (append (list (get-next (rest lst) (car lst) )) (make-new (rest lst) ))))


(define F
  (lambda (lst) (cond
    [(empty? lst) lst]
    [(andmap (lambda (x) (eq? (car lst) x) ) lst ) lst]
    [else (F (filter (lambda (x) (not (zero? x))) (make-new lst)) )])
    )
  )