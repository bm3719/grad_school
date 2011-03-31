;; MODIFY replaces any occurance of s1 in s2 with s3.
(defun MODIFY (s1 s2 s3)
  (cond	((null s2) s2)		;; base case
        ((atom s2) 		;; if atom
         (if (eq s2 s1)	    	;; if atom == s1
             s3		        ;; replace w/ s3
             s2))               ;; else return s2
        (t			;; if list
         (if (equal s2 s1)	;; if list == s1
             s3                 ;; replace w/ s3
             ;; otherwise, recursively call MODIFY
             ;; on the first elem and the rest of list
             (cons (MODIFY s1 (car s2) s3)
                   (MODIFY s1 (cdr s2) s3))))))
