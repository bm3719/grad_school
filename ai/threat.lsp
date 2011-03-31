;;; THREAT checks if position x is threatened by any existing queens on board y.
(defun THREAT (x y)
  (cond ((null x) NIL)          ; base case
        (t                      ; if a column
         ;; check for any of 4 cases
         (or
          ;; if queen in same column
          (and
           (equal (car x) (car (car y)))
           (not (equal (cdr (car y)) '(0))))
          ;; if on same row
          (equal (cdr x) (cdr (car y)))
          ;; if diagonally attacking
          (=
           (-
            (car (cdr (car y)))
            (abs (-
                  (car x)
                  (car (car y)))))
           (car (cdr x)))
          ;; recursively call THREAT on remaining board
          (if (null (cdr (cdr y)))
              NIL
              (THREAT x (cdr y)))))))
