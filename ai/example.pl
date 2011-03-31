% -*- mode: Prolog -*-
% factorial 

factorial(0,1).

factorial(A,B) :-
	A > 0,
	C is A-1,
	factorial(C,D),
	B is A*D.

%%

witch(X)  :- 
	burns(X),
	female(X).
burns(X)  :- 
	wooden(X).
wooden(X) :-
	floats(X).
floats(X) :- 
	sameweight(duck, X).

female(girl).
sameweight(duck,girl).


%%

