#lang racket
(require 2htdp/image)
(provide img->mat
ascii-art)

(define chars " .,:;ox%#@")

;; Convert image to matrix
(define (img->mat img)
  (define img_list (grayscale img) )
  (split-list img_list (image-width img)))

;; Make ASCII art of image
(define (ascii-art width height chars)
  (lambda (img)
    (define image-matrix (img->mat img)) ; convert image into matrix
    (define cutted-matrix (cut-matrix image-matrix width height)) ; cut off not complete rows and cols
    (define width-div (map (lambda (row) (split-list row width)) cutted-matrix)) ; divide rows into blocks of "width"
    (define transposed (transpose-matrix width-div)) ; transpose matrix
    (define joined-transposed (join-blocks transposed) ) ; Join n blocks of size m*1 to blocks n*m (to make next step)
    (define blocks-div (map (lambda (row) (split-list row (* width height))) joined-transposed ) ) ; divide cols into blocks of "height"
    (define final-blocked-matrix (transpose-matrix blocks-div)) ; transpose matrix back
    (define mean-matrix (calculate-mean final-blocked-matrix)) ; Calculate means (intensity) in every widht*height blocks 
    (define char-matrix (map (lambda (row) (map (lambda (i) (string-ref chars (intense->char i chars) ) ) row)) mean-matrix)) ; convert intensiy to chars
    
    (cond
      [(empty? char-matrix) ""]
      [else (string-join
       (map (lambda (row) (string-append (list->string row))) char-matrix )
       "\n" #:after-last "\n")]) ; make string from matrix of chars
     ))

;;;; Color Functions

;; Convert RGB color into grayscale
(define (RGB->grayscale color)
  (+ (* 0.3 (color-red color))
     (* 0.59 (color-green color))
     (* 0.11 (color-blue color))))

;; Convert image into grayscale
(define (grayscale img) (map RGB->grayscale (image->color-list img)))

;; Convert intensity into char index
(define (intense->char i chars)
  (exact-floor (/ (* (string-length chars) (- 255 (floor i) )) 256) ))

;;;; Matrix functions

;; Split list into chunks of width "width"
(define (split-list lst width)
  (cond
    [(empty? lst) lst]
    [(< (length lst) width) (list lst)]
    [else (cons (take lst width) (split-list (drop lst width) width ) )]
    ))

;; Cut matrix to sizes (heigt*n)x(width*m)
(define (cut-matrix mtx width height)
  (take
   (map (lambda (row) (take row (- (length row) (remainder (length row) width) )) ) mtx )
        (- (length mtx) (remainder (length mtx) height) )))

;; Transpose matrix
(define (transpose-matrix lst)
  (if (empty? lst) lst (apply map list lst )))

;; Join n blocks of size m*1 to blocks n*m
(define (join-blocks matrix)
  (map (lambda (row) (foldr append '() row) ) matrix))

;; Calculate mean of list
(define (calculate-mean m)
  (map (lambda (row)
         (map (lambda (el)
                (/ (apply + el) (length el) )) row) ) m))