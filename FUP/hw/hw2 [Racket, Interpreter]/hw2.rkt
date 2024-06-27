#lang racket

(require racket/trace)
(provide execute)

;; "Jakub Strnad: FUP jak na hw2": https://discord.com/channels/890688768326139957/938513391457292308/1096827333094146098


;; SVG-primitives
(define (circle x y r style) (format "<circle cx=\"~a\" cy=\"~a\" r=\"~a\" style=~s/>" x y r style))
(define (rect x y width height style) (format "<rect x=\"~a\" y=\"~a\" width=\"~a\" height=\"~a\" style=~s/>" x y width height style))
(define (line x1 y1 x2 y2 style) (format "<line x1=\"~a\" y1=\"~a\" x2=\"~a\" y2=\"~a\" style=~s/>" x1 y1 x2 y2 style))

;; Numerical operations
(define num-op '(+ - * / floor cos sin))
(define (get-num-op op)
  (match op
    ['+ +]
    ['- -]
    ['* *]
    ['/ /]
    ['floor floor]
    ['cos cos]
    ['sin sin]
    [_ error "ERROR: Unknown num operation" ]))

;; Boolean operations
(define bool-op '(= < >))
(define (get-bool-op op)
  (match op
    ['= =]
    ['< <]
    ['> >]
    [_ error "ERROR: Unknown bool operation" ]))

;; SVG operations
(define svg-op '(circle rect line))
(define (get-svg-op op)
  (match op
    ['circle circle]
    ['rect rect]
    ['line line]
    [_ error "ERROR: Unknown svg operation" ]))

;; Environment dictionary (lists of pairs as an association list)
(define (is-in-env? cid env) ;is constant in evn
  (if (eqv? (assv cid env) #f) #f #t))
(define (get-from-env cid env) ;get constant from env
  (cadr (assv cid env)))
(define (is-fun-in-env? cid env) ;is function in env
  (if (assf (lambda (arg) (if (list? arg) (eqv? (car arg) (car cid)) #f )) env ) #t #f) )
(define (get-fun-from-env cid env) ;get function from env
  (assf (lambda (arg) (and (list? arg) (eqv? (car arg) (car cid))) ) env))

;; Zip function, what make pairs of '(argument_name argument_value)
;; (zip '(mean a b c) '(mean 4 23 45)) -> '((a 4) (b 23) (c 45))
(define (zip def to-ex env [start 0] )
  (if (= start 0)
      (zip (cdr def) (cdr to-ex) env 1)
      (map (lambda (el1 el2) (list el1 (prg-match el2 env)) ) def to-ex)))

;; Match program unit
(define (prg-match expr env)
  ;; alias prg-match match with defined env
  (define (match-env expr) (prg-match expr env))
  
  (match expr
    ;; Match defined variable
    [(var cid) #:when (is-in-env? cid env) (prg-match (get-from-env cid env) env)]

    ;; Match numerical operation (num-op)
    [(list func args ...) #:when (member func num-op ) (apply (get-num-op func) (map match-env args))]
    
    ;; Match bool operation (bool-op)
    [(list func a1 a2) #:when (member func bool-op ) ((get-bool-op func) (prg-match a1 env) (prg-match a2 env))]

    ;; Match svg operation (svg-op)
    [(list func args ...) #:when (member func svg-op) (apply (get-svg-op func) (map match-env args))]
    
    ;; Match defined function
    [(var cid) #:when (and (list? cid) (is-fun-in-env? cid env)) 
               (define fun (get-fun-from-env cid env))
               (define new-env (append (zip (car fun) cid env) env))
               (define bodies (list-ref fun 1))
               (define wrapped-bodies (if (not (list? (car bodies))) (list bodies) bodies))

               (map (lambda (body) (prg-match body new-env)) wrapped-bodies )
               ]

    ;; Match 'if' expression
    [(list 'if bool-exp exp-t exp-f) (if (prg-match bool-exp env) (prg-match exp-t env) (prg-match exp-f env) )]

    ;; Match 'when' expression
    [(list 'when bool-exp exprs ..1 ) (when (prg-match bool-exp env) (map match-env exprs))]
    
    ;; Match number
    [_ #:when (number? expr) expr] 

    ;; Match string
    [_ #:when (string? expr) expr] 
    ))

;; Parse program
(define (parse-one item)
  (match item
    [(list 'define cid val) (cdr item)]  ;; variable definition
    [(list 'define id expr ..1) (list id expr)])) ;; function definition
(define (parse-prg input) (map parse-one input))


;; Main function
(define (execute width height prg expr)
  (define parsed-prg (parse-prg prg))
  (define content (prg-match expr parsed-prg))
  
  ;; Merge all svg strings
  (define (merge-strings content)
    (match content
      [_ #:when (string? content) content]
      [_ #:when (void? content) ""]
      [else (apply string-append (map (lambda (c) (merge-strings c)) content ))])
    )
  
  (define merged-content (merge-strings content))
  (define header (format "<svg width=\"~a\" height=\"~a\">" width height))
  (string-append header merged-content "</svg>" ))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; test 1
;;(display (execute 400 400 '() '(line 10 20 30 40 "stroke:black;stroke-width:5")))

;; test 2
;;(display (execute 400 400 '((define STYLE "fill:red")) '(circle 200 200 (floor (/ 200 3)) STYLE)))

;; test 3
(define test3-env
  '((define (start)
      (rect 0 0 100 100 "fill:red")
      (rect 100 0 100 100 "fill:green")
      (rect 200 0 100 100 "fill:blue"))))
;;(display (execute 400 400 test3-env '(start)))

;; test 4
(define test4-env
  '((define STYLE "fill:red;opacity:0.2;stroke:red;stroke-width:3")
    (define START 195)
    (define END 10)
    (define (circles x r)
      (when (> r END)
        (circle x 200 r STYLE)
        (circles (+ x (floor (/ r 2))) (floor (/ r 2)))))))
;;(display (execute 400 400 test4-env '(circles 200 START)))

;; test 5
(define tree-prg
  '((define STYLE1 "stroke:black;stroke-width:2;opacity:0.9")
    (define STYLE2 "stroke:green;stroke-width:3;opacity:0.9")
    (define FACTOR 0.7)
    (define PI 3.14)
    (define (draw x1 y1 x2 y2 len angle)
      (if (> len 30)
          (line x1 y1 x2 y2 STYLE1)
          (line x1 y1 x2 y2 STYLE2))
      (when (> len 20)
        (recur-tree x2 y2 (floor (* len FACTOR)) angle)
        (recur-tree x2 y2 (floor (* len FACTOR)) (+ angle 0.3))
        (recur-tree x2 y2 (floor (* len FACTOR)) (- angle 0.6))))
    (define (recur-tree x1 y1 len angle)
      (draw x1
            y1
            (+ x1 (* len (cos angle)))
            (+ y1 (* len (sin angle)))
            len
            angle))))
;;(display (execute 400 300 tree-prg '(recur-tree 200 300 100 (* PI 1.5))))

;; test 6
(define test-6-env '((define STYLE "fill:pink;opacity:0.5;stroke:black;stroke-width:2")
  (define END 15) (define (recur-circ x y r)
                    (circle x y r STYLE)
                    (when (> r END)
                      (recur-circ (+ x r) y (floor (/ r 2)))
                      (recur-circ (- x r) y (floor (/ r 2)))
                      (recur-circ x (+ y r) (floor (/ r 2)))
                      (recur-circ x (- y r) (floor (/ r 2)))))))
;;(display (execute 400 400 test-6-env '(recur-circ 200 200 100)))