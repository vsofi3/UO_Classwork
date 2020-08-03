(* Dan Grossman, CSE341, HW3 Provided Code *)

exception NoAnswer

datatype pattern = WildcardP
                 | VariableP of string
                 | UnitP
                 | ConstantP of int
                 | ConstructorP of string * pattern
                 | TupleP of pattern list

datatype valu = Constant of int
              | Unit
              | Constructor of string * valu
              | Tuple of valu list

fun g f1 f2 p =
    let 
        val r = g f1 f2 
    in
        case p of
            WildcardP         => f1 ()
          | VariableP x       => f2 x
          | ConstructorP(_,p) => r p
          | TupleP ps         => List.foldl (fn (p,i) => (r p) + i) 0 ps
          | _                 => 0
    end

(**** you can put all your code here ****)
(* 1 *)
fun only_lowercase (xs : string list ) = 
     List.filter(fn str => Char.isLower(String.sub(str, 0))) xs

(* 2 *)
fun longest_string1 (xs : string list)  = 
      List.foldl (fn(x,y) => if (String.size(x) > String.size(y)) then x else y) "" xs;

(* 3 *)
fun longest_string2 (xs : string list)  = 
      List.foldl (fn(x,y) => if (String.size(x) >= String.size(y)) then x else y) "" xs;
    
(* 4 *)
fun longest_string_helper f xs = 
    List.foldl( fn (s1, s2) => if f (String.size s1, String.size s2) then s1 else s2) "" xs

val longest_string3 = longest_string_helper (fn (s1, s2) => if (s1 > s2) then true else false)
val longest_string4 = longest_string_helper (fn (s1, s2) => if (s1 >= s2) then true else false)

(* 5 *)
val longest_lowercase = longest_string1 o only_lowercase

(* 6 *)
(* Make everything in the string upper case, convert string to list, filter out "X", convert back to a string *) 
val caps_no_X_string = String.implode o List.filter(fn str => if Char.compare(str, #"X") = EQUAL then false else true) o String.explode o String.map Char.toUpper

(* 7  *)
fun first_answer f xs =
    case xs of
         [] => raise NoAnswer
       | x :: xs => case f(x) of
                        NONE => first_answer f xs
                      | SOME v => v

(* 8 *) 
fun all_answers f xs = 
    let
        (* Helper function stores answers in accumulator list *) 
        fun helper (xls, acc) = 
            case xls of 
                  [] => SOME acc
                | x :: xls => case f(x) of 
                      NONE => NONE 
                    | SOME x => helper(xls, x @ acc)
    in 
        helper(xs, [])
    end

(* 9
      A - G takes in two functions and a pattern as inputs. 
          The function that is used is determined by the presence/absence of the wildcard character in the pattern. *)
(* B *)
fun count_wildcards ( p : pattern) = 
    g (fn()=>1) (fn x=>0) p
(* C *)
fun count_wild_and_variable_lengths ( p : pattern) = 
    g (fn()=>1) (fn x=>String.size x) p
(* D *)
fun count_a_var (str : string, p : pattern) = 
    g (fn()=>0) (fn x=> if x=str then 1 else 0) p

(* 10 *) 
fun check_pat (pat : pattern)  = 
    let
        (* Decomposes the pattern and returns a list of strings *)
        fun patternDecomp p = 
            case p of
                  VariableP x => [x]
                | TupleP tp => List.foldl (fn (x,y) => (patternDecomp(x)) @ y ) [] tp
                | ConstructorP (_, p) => patternDecomp(p)
                | _ => []
        (* Goes through the string list and checks for duplicates *)
        fun duplicateChecker xs = 
            case xs of 
                  [] => true
                | x :: xs => if (List.exists (fn y => x = y) xs) 
                             then false
                             else duplicateChecker(xs)
    in
        duplicateChecker(patternDecomp(pat))
    end

(* 11 *) 
fun match (value : valu, pat : pattern) =
  case (value, pat) of
       (_, WildcardP) => SOME []
     | (_, VariableP x) => SOME [(x, value)]
     | (Unit, UnitP) => SOME []
     | (Constructor(s1,v), ConstructorP (s2,pat)) => if s1=s2 then match (v,pat) else NONE
     | (Constant x, ConstantP y) => if x=y then SOME [] else NONE
     | (Tuple v, TupleP p) => 
         (all_answers (fn (x,y) => match(x, y)) (ListPair.zipEq(v,p)) handle UnequalLengths => NONE)
     | _ => NONE

(* 12 *)
fun first_match value pat = 
    case pat of 
          [] => NONE
        | x :: xs => if match(value, x)  = NONE then first_match value xs
                                                else match(value, x) 
            
