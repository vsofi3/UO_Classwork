exception Unimplemented;


datatype 'a Seq = Cons of 'a * (unit -> 'a Seq);



fun dropUntil(s, n) =
    case s of
        Cons((inp, outp), th) => if n = inp 
            then s 
            else dropUntil(th(), n)


fun takeN(s, n): 'a list =
  case (s, n) of
    (_, 0)          => []
  | (Cons(h, t), n) => h :: (takeN (t(), n - 1))


fun merge(s1, s2) = 
    case (s1, s2) of
        (Cons (h1, t1), Cons(h2, t2)) => Cons((h1, h2), fn() => merge(t1(), t2()))    


(* fun merge(s1,s2) = case (s1, s2) of 
      (Cons(head, tail), Cons(head2, tail2)) => Cons(head, fn() => merge_other(s2, tail()))
*)
fun apply (Cons( (x1,fx1), xs) , x2) = if (x1 = x2) then fx1 else apply (xs(), x2);

fun compose(s1, s2) = 
    case (s1, s2) of
        (Cons ((x1,y1), f), Cons((x2,y2), g)) =>  Cons((x1, (apply(s2,y1))), fn() => compose(f(), g()))
(*test code*)

fun make_pos (n) = Cons(n, fn()=>make_pos(n+1));

fun make_stream(n, f) = Cons(n, (fn () => make_stream(f(n), f)));
fun make_in_out_stream(n, f) = 
    let
        fun helper(n) = Cons((n, f(n)), (fn () => helper(n + 1)));
    in
        helper(0)
    end;

val f = make_in_out_stream(0, (fn x => x + 1));
val g = make_in_out_stream(0, (fn x => x * 2));

val testMerge = takeN(merge(make_pos(1), make_pos(10)), 5) = [(1,10), (2,11), (3,12), (4,13), (5,14)];
val testCompos1 = takeN(compose(f, g), 10) = [(0,2),(1,4),(2,6),(3,8),(4,10),(5,12),(6,14),(7,16),(8,18),(9,20)];
val testCompos2 = takeN(compose(g, f), 10) = [(0,1),(1,3),(2,5),(3,7),(4,9),(5,11),(6,13),(7,15),(8,17),(9,19)];
