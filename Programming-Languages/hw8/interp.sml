datatype  Term = NUM of int | DIVIDE of Term  * Term

datatype ResultE = Int of int | Error

type ResultS = int * int

exception Unimplemented;
exception DivByZero;
exception Unknown; 

fun interp (t: Term) : int = case (t) of 
                       NUM n => n
                    |  DIVIDE (t1, t2) => (case (interp(t1), interp(t2)) of
                          (x, 0) => raise DivByZero
                        | (x, y) => x div y)  

fun interp_exception (t: Term) : ResultE = case (t) of 
                                  NUM n => Int n
                                | DIVIDE (t1, t2) => (case (interp_exception(t1), interp_exception(t2)) of 
                                      (Int x, Int 0) => Error
                                    | (Int x, Int y) => (Int (x div y))
                                    | _ => Error)
                                          

(*fun interp_state (t: Term) : ResultS =
    let 
        fun divCounter (t) = case (t) of 
              NUM n => 0 
            | DIVIDE (t1, t2) => 1 + divCounter(t1) + divCounter(t2) 
        fun divide (t) =  case (t) of 
               NUM n => (n)
            |  DIVIDE (t1, t2) => (case (divide(t1), divide(t2)) of
                    (x, 0) => raise DivByZero
                  | (x, y) => (x div y))
    in
        (divide(t), divCounter(t))
    end  
*)   

fun interp_state(t: Term) : ResultS = case(t) of
      NUM n => (n, 0)
    | DIVIDE (t1, t2) => let val (x, divCountX) = interp_state(t1)
                             val (y, divCountY) = interp_state(t2)
                         in 
                            ((x div y), 1 + divCountX + divCountY)
                         end


val test_interp = interp(DIVIDE(DIVIDE(NUM 8, NUM 4), NUM 2))
val another_interp = interp(DIVIDE(NUM 30, DIVIDE(DIVIDE(NUM 20, NUM 5), NUM 2)))

val test_error = (interp_exception(DIVIDE(DIVIDE(NUM 8,NUM 1),DIVIDE(NUM 8,(DIVIDE(NUM 0,NUM 1))))) = Error);
val test_state = (interp_state(DIVIDE(DIVIDE(NUM 8,NUM 1),DIVIDE(NUM 8,(DIVIDE(NUM 8,NUM 1))))));
val test_state2 = (interp_state(DIVIDE(NUM 8,NUM 1)));
val test_state3 = (interp_state(DIVIDE(NUM 8, DIVIDE(NUM 4, NUM 2))));
