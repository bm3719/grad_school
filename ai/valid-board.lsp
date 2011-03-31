;; VALID-BOARD checks if a board state is a solution to the n-queens problem.
(defun VALID-BOARD (x)
  (cond ((null x) NIL)	        ;; base case
        (t  (not                ;; if positions in list
             (or                ;; check 2 conditions
              (THREAT (car x)   ;; if first spot threat
                      (MODIFY (car x)
                              x
                              (list (car (car x)) 0)))
              ;; if rest of board threats
              (VALID-BOARD (cdr x)))))))

