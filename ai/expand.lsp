;; EXPAND returns all valid next placements for a queen on board x.
(defun EXPAND (x)
  (setq open-list ())         ;; create open-list
  (for i 1 (length x)         ;; column
       (for j 1 (length x)    ;; row
            (if (THREAT (list i j) x)   ;; test if (i j) is threatened
                (setq open-list (cons   ;; if not, add to open-list
                                 (MODIFY (list i '0) x (list i j))))
                NIL))))       ;; if so, do nothing
