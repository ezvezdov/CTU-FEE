#lang racket

;;; Brainf*ck - interpreter
;;; As the symbols ., have their specific meaning in Racket, we replace them by the symbols *@

;;; Sample Brainf*ck programs
; program reading non-negative numbers on the first two positions and displaying their sum
(define add-prg
  '(@ > @ [- < + >] < *))

; program reading non-negative numbers on the first two positions and displaying their product
(define mul-prg
  '(@ > @ < [- > [- > + > + < <] > [- < + >] < <] > > > *))

(define SIZE 10)

(struct tape_cls (left val right) #:transparent)

(define (fresh-tape size)
  (tape_cls '() 0 (make-list (- size 1) 0)))

(define (change op t)
  (tape_cls (tape_cls-left t)
        (op (tape_cls-val t) 1)
        (tape_cls-right t)))

(define (move dir t)
  (match dir
    ['left (if (null? (tape_cls-left t))
               (error "outside")
               (tape_cls (cdr (tape_cls-left t))
                     (car (tape_cls-left t))
                     (cons (tape_cls-val t) (tape_cls-right t))))]
    ['right (if (null? (tape_cls-right t))
                (error "outside")
                (tape_cls (cons (tape_cls-val t) (tape_cls-left t))
                      (car (tape_cls-right t))
                      (cdr (tape_cls-right t))))]))

; evaluates comma command, i.e., (car input) -> tape[ptr]
(define (eval-comma prg input tape)
  (cond
    ([null? input] (error "Empty input"))
    (else (let ([new-tape (tape_cls (tape_cls-left tape) (car input) (tape_cls-right tape))])
            (eval-prg prg (cdr input) new-tape)))))

; evaluates all the commands beside comma
(define (eval-cmd cmd prg input tape)
  (match cmd
    ['+ (eval-prg prg input (change + tape))]
    ['- (eval-prg prg input (change - tape))]
    ['< (eval-prg prg input (move 'left tape))]
    ['> (eval-prg prg input (move 'right tape))]
    ['* (begin (printf "~a " (tape_cls-val tape))
               (eval-prg prg input tape))]
    [_ (error "Unknown command")]))

(define (eval-cycle cycle prg input tape)
  (if (= (tape_cls-val tape) 0)
      (eval-prg prg input tape)
      (let-values ([(new-input new-tape) (eval-prg cycle input tape)])
        (eval-cycle cycle prg new-input new-tape))))

(define (eval-prg prg input tape)
  (match prg
    [(list) (values input tape)]
    [(list '@ rest ...) (eval-comma rest input tape)]
    [(list (? list? cmd) rest ...) (eval-cycle cmd rest input tape)]
    [(list cmd rest ...) (eval-cmd cmd rest input tape)]))

; executes the given program with the given input
(define (run-prg prg input)
  (let-values ([(remaining-input final-tape) (eval-prg prg input (fresh-tape SIZE))]) ; 
    (printf "done~n")))

(run-prg add-prg '(1 5))

(run-prg mul-prg '(2 5))