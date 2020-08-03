#lang racket

;1
(define (sequence spacing low high)
    (if (<= low high)
        (cons low (sequence spacing (+ low spacing) high)) ;if the current value is less than high, add spacing and continue
         null));if the current value is greater than high, stop

;2
(define (list-nth-mod xs n)
  (cond [   (< n 0) ; case where n is less than 0
            (error "list-nth-mod: negative number")]
        [   (null? xs) ; case where list is empty
            (error "list-nth-mod: empty list")]
        [#t (car (list-tail xs (remainder n (length xs))))]))

;3
(define (stream-for-k-steps s k)
  (cond [   (= k 0) null] ; if we've taken k steps, stop
        [#t (cons (car (s)) (stream-for-k-steps (cdr (s)) (- k 1)))])) ;add first element from stream to list, proceed through rest of list, decrement k

;4
(define (funny-number-stream)
  (define (funny-helper x)
       (cond [   (= (remainder x 6) 0) (cons (- 0 x) (lambda() (funny-helper (+ x 1))))] ;if x % 6 = 0, negate it & add to list
             [#t (cons x (lambda() (funny-helper (+ x 1))))])) ;just add x to the list
  (funny-helper 1))

;5
(define (vector-assoc v vec)
  (define (helper n)
              (if (>= n (vector-length vec))
                      #f ;return f if no vector element matches v
                     (if (and (pair? (vector-ref vec n)) (equal? (car (vector-ref vec n)) v))
                            (vector-ref vec n)  ; if a match is found, return it
                            (helper (+ n 1))))) ; continue checking vector for a match
  (helper 0))

;6
(define (caching-assoc xs n)
  (letrec ([index 0] ;position value for inserting items into cache
           [cache (make-vector n #f)] ;initialize cache with #f
    [helper (lambda (v)
      (let ([val (vector-assoc v cache)])
        (cond
            [val val] ;if value is already in cache, return it 
            [#t (let ([assoc_val (assoc v xs)]) 
                (begin
                  (vector-set! cache index assoc_val) ;else add new item to cache
                  (if (> (- n 1) index) 
                      (set! index (+ 1 index))
                      (set! index 0)) ;reset index value if it goes out of bounds
                  assoc_val))])))])
    helper))

;7
(define-syntax while-greater
  (syntax-rules (do)
      ((while-greater e1 do e2)
         (letrec ([helper (let ([e e1]) (lambda() ;evaluate e1 and store value
              (cond [   (>= e e2) #t]  ;if e1 is greater than e2 stop
                    [#t (helper)])))]) ;else keep going
      (helper)))))

;8
(define (TR e)
  (cond
    [   (number? e) (quasiquote(unquote e))]
    [   (equal? (car e) `+ ) `(*, (TR (cadr e)), (TR (caddr e)))] ; swap + to *, recursively call to process rest of the expressions
    [   (equal? (car e) `* ) `(+, (TR (cadr e)), (TR (caddr e)))] ; swap * to +, recursively call to process rest of the expressions
    [#t (error "Error: Not a valid E term")]))

