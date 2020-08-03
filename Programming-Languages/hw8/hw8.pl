%% just for reference
myCount([],0).
myCount([_|Xs], s(N)) :- myCount(Xs, N).

%% may be useful
myMember(X, [Y|T]) :- X = Y; myMember(X, T).

%% 1
split([], [], []).
split([A],[A],[]).
split([x1, x2 | Xs], [x1| Ys], [x2 | Zs]) :- split(Xs, Ys, Zs). 

%% 2
removerep([], []).
removerep([H | Xs], Zs) :- myMember(H, Xs), removerep(Xs, Zs). 
removerep([H | Xs], [H | Zs]) :- not(myMember(H, Xs)), removerep(Xs, Zs). 

%% 3 - Referenced post.pl
edge(a,b).
edge(b,c).
edge(c,d).

path(_, _, 0).
%%path(X,Y) :- edge(X,Y).
path(X, Y, 1) :- edge(X,Y).
path(X, Y, s(N)) :- edge(X,Z), path(Z, Y, N).

%% 4 - Referenced Mondays lecture
myTypeOf(_, n, int).
myTypeOf(_, true, bool).
myTypeOf(_, false, bool).
myTypeOf(Env, if(E1,E2,E3), T) :- myTypeOf(Env, E1, bool), myTypeOf(Env, E2, T), myTypeOf(Env, E3, T).
myTypeOf(Env, var(X), T) :- myMember(Env, [X | T]).
myTypeOf(Env, apply(E1, E2), T) :- myTypeOf(Env, E1, arrow(K, T), myTypeOf(Env, E2, K)).
myTypeOf(Env, lambda(X, E), arrow(K, T)) :- myTypeOf([[X,K] | Env], E, T).
myTypeOf(Env, let(X, E1, E2), T) :- myTypeOf(Env, E1, K), myTypeOf([[X,K] | Env], E2, T).
