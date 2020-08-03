(* CSE 341, HW2 Provided Code *)

(* main datatype definition we will use throughout the assignment *)
datatype json =
         Num of real (* real is what SML calls floating point numbers *)
       | String of string
       | False
       | True
       | Null
       | Array of json list
       | Object of (string * json) list

(* some examples of values of type json *)
val json_pi    = Num 3.14159
val json_hello = String "hello"
val json_false = False
val json_array = Array [Num 1.0, String "world", Null]
val json_obj   = Object [("foo", json_pi), ("bar", json_array), ("ok", True)]

(* some provided one-liners that use the standard library and/or some features
   we have not learned yet. (Only) the challenge problem will need more
   standard-library functions. *)

(* dedup : string list -> string list -- it removes duplicates *)
fun dedup xs = ListMergeSort.uniqueSort String.compare xs

(* strcmp : string * string -> order compares strings alphabetically
   where datatype order = LESS | EQUAL | GREATER *)
val strcmp = String.compare                                        
                        
(* convert an int to a real *)
val int_to_real = Real.fromInt

(* absolute value of a real *)
val real_abs = Real.abs

(* convert a real to a string *)
val real_to_string = Real.toString

(* return true if a real is negative : real -> bool *)
val real_is_negative = Real.signBit

(* We now load 3 files with police data represented as values of type json.
   Each file binds one variable: small_incident_reports (10 reports), 
   medium_incident_reports (100 reports), and large_incident_reports 
   (1000 reports) respectively.

   However, the large file is commented out for now because it will take 
   about 15 seconds to load, which is too long while you are debugging
   earlier problems.  In string format, we have ~10000 records -- if you
   do the challenge problem, you will be able to read in all 10000 quickly --
   it's the "trick" of giving you large SML values that is slow.
*)

(* Make SML print a little less while we load a bunch of data. *)
       ; (* this semicolon is important -- it ends the previous binding *)
Control.Print.printDepth := 3;
Control.Print.printLength := 3;

use "parsed_small_police.sml"; 
(* use "parsed_medium_police.sml"; *)

(*use "parsed_large_police.sml"; *)  

val large_incident_reports_list =
    case small_incident_reports of
        Array js => js
      | _ => raise (Fail "expected large_incident_reports to be an array")


(* Now make SML print more again so that we can see what we're working with. *)
; Control.Print.printDepth := 20;
Control.Print.printLength := 20;

(**** PUT PROBLEMS 1-8 HERE ****)

(* 1 *)
fun make_silly_json i = 
    let fun silly_helper i =
        if i = 0 then []
        else
            (Object [("n", Num (int_to_real i)), ("b", True)]) :: silly_helper(i - 1)
    in Array (silly_helper i) end

(* 2 *)
fun assoc (k, xs) = 
    case xs of
          [] => NONE
        | (k1, v1) :: xs => if k = k1 then SOME v1 else assoc(k, xs)

(* 3 *)
fun dot (j, f) =  
    case j of 
        Object xs => assoc (f, xs)
        | _ => NONE

(* 4 *)
fun one_fields j = 
    case j of
        Object xs => let fun one_fields_helper (xs, masterList) = 
            case xs of 
                [] => masterList
                | (s, _) :: xs => one_fields_helper(xs, s::masterList)
        in 
            one_fields_helper(xs, [])
        end
    | _ => [] 

(* 5 *)
fun no_repeats sl = 
    if length(sl) = length(dedup sl) then true
    else false

(* 6 *)
fun recursive_no_field_repeats js = 
    let
        (* Helper for processing elements of the json array *) 
        fun r_n_r_help1 xs = 
            case xs of
                [] => true
                | js :: xs => recursive_no_field_repeats js andalso r_n_r_help1 xs
        (* Helper for processing the contents of a json object *)
        fun r_n_r_help2 xs = 
            case xs of 
                [] => true
                | (_,js) :: xs => recursive_no_field_repeats js andalso r_n_r_help2 xs
    in
        case js of 
            Array xs => r_n_r_help1 xs
            |  Object xs => no_repeats(one_fields (Object xs)) andalso r_n_r_help2 xs
            | _ => true
    end

(* 7 *)
fun count_occurrences(stringList, excep) =
    let
        fun count_helper (stringList, compVal, charString, csCount, masterList) = 
            case stringList of
                [] => (charString, csCount) :: masterList
                | s::stringList => if s = charString
                    then count_helper(stringList, compVal, charString, csCount + 1, masterList)
                    else if compVal = EQUAL orelse compVal = strcmp(s, charString)
                        then count_helper(stringList, strcmp(s, charString), s, 1, (charString, csCount)::masterList)
                        else raise excep
    in 
        case stringList of
            [] => []
            | s::stringList => count_helper(stringList, EQUAL, s, 1, [])
    end

(* 8 *)
fun string_values_for_field(s, jsonList) =
    case jsonList of 
        [] => []
        | j::jsonList => case dot (j, s) of
                SOME (String v) => v :: string_values_for_field(s, jsonList)
                |_ => string_values_for_field(s, jsonList)

(* histogram and historgram_for_field are provided, but they use your 
   count_occurrences and string_values_for_field, so uncomment them 
   after doing earlier problems *)

(* histogram_for_field takes a field name f and a list of objects js and 
   returns counts for how often a string is the contents of f in js. *)

exception SortIsBroken 

fun histogram (xs : string list) : (string * int) list =
  let
    fun compare_strings (s1 : string, s2 : string) : bool = s1 > s2

    val sorted_xs = ListMergeSort.sort compare_strings xs
    val counts = count_occurrences (sorted_xs,SortIsBroken)

    fun compare_counts ((s1 : string, n1 : int), (s2 : string, n2 : int)) : bool =
      n1 < n2 orelse (n1 = n2 andalso s1 < s2)
  in
    ListMergeSort.sort compare_counts counts
  end

fun histogram_for_field (f,js) =
  histogram (string_values_for_field (f, js))


(**** PUT PROBLEMS 9-11 HERE ****)

(* 9 *)
fun filter_field_value(stringA, stringB, jsonList) = 
    case jsonList of
        [] => []
        | json :: jsonList => case dot(json, stringA) of
            SOME (String v) => if v = stringB 
                then json::filter_field_value(stringA, stringB, jsonList)
                else filter_field_value(stringA, stringB, jsonList)
            | _ => filter_field_value(stringA, stringB, jsonList)              


;Control.Print.printDepth := 3;
Control.Print.printLength := 3;

(* 10 *) 
val large_event_clearance_histogram = histogram_for_field("event_clearance_description", large_incident_reports_list)
(* 11 *)
val large_hundred_block_location_histogram = histogram_for_field("hundred_block_location", large_incident_reports_list)

(**** PUT PROBLEMS 12-15 HERE ****)
(* 12 *)
val forty_third_and_the_ave_reports = filter_field_value("hundred_block_location", "43XX BLOCK OF UNIVERSITY WAY NE", large_incident_reports_list)
(* 13 *)
val forty_third_and_the_ave_event_clearance_description_histogram = histogram_for_field("event_clearance_description", forty_third_and_the_ave_reports)
(* 14 *)
val nineteenth_and_forty_fifth_reports = filter_field_value("hundred_block_location", "45XX BLOCK OF 19TH AVE NE", large_incident_reports_list)
(* 15 *) 
val nineteenth_and_forty_fifth_event_clearance_description_histogram = histogram_for_field("event_clearance_description", nineteenth_and_forty_fifth_reports)

;Control.Print.printDepth := 3;
Control.Print.printLength := 3;

(**** PUT PROBLEMS 12-15 HERE ****)


;Control.Print.printDepth := 20;
Control.Print.printLength := 20;

(**** PUT PROBLEMS 16-19 HERE ****)

(* For CHALLENGE PROBLEMS, see hw2challenge.sml *)

;Control.Print.printDepth := 20;
Control.Print.printLength := 20;

(**** PUT PROBLEMS 16-19 HERE ****)
(* 16 *)
fun concat_with(delim, stringList) = 
    case stringList of 
          [] => "" 
        | str :: [] => str
        | (str :: stringList) => str ^ delim ^ concat_with(delim, stringList)

(* 17 *)
fun quote_string str = 
    "\"" ^ str ^ "\"" 

(* 18 *)
fun real_to_string_for_json r = 
    if real_is_negative r then let val absR = real_abs r
        in "-" ^ real_to_string(absR) end
    else real_to_string r    

(* 19 *)
fun json_to_string json = 
    let
        fun arrayProcessor arr =
            case arr of 
                  [] => [] 
                | json :: arr => json_to_string json :: arrayProcessor arr
        fun objectProcessor obj =
            case obj of
                  [] => []
                | (f, j) :: obj => ((quote_string f) ^ " : " ^ (json_to_string j)) :: objectProcessor obj 
    in 
        case json of
              Num r => real_to_string_for_json r
            | String str => quote_string(str)
            | False => "false"
            | True => "true"
            | Null => "null"
            | Array jsonList => "["^concat_with(", ", arrayProcessor jsonList)^"]"
            | Object obj => "{"^concat_with(", ", objectProcessor obj)^"}"
    end 

(* For CHALLENGE PROBLEMS, see hw2challenge.sml *)
