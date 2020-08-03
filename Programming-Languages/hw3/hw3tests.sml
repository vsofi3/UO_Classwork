use "hw3.sml";

val test1a = only_lowercase ["A", "b", "c", "D"] = ["b", "c"];

val test2a = longest_string1 ["one", "two", "three"] = "three";
val test2b = longest_string1 [] = "";
val test2c = longest_string1 ["one", "two", "ab", "cd"] = "one";

val test3a = longest_string2 ["one", "two", "three"] = "three";
val test3b = longest_string2 [] = "";
val test3c = longest_string2 ["one", "two", "ab", "cd"] = "two";

val test4a = longest_string3 ["one", "two", "three", "eerht"] = "three";
val test4b = longest_string4 ["one", "two", "three", "eerht"] = "eerht";

val test5a = longest_lowercase ["not me", "definitely me", "me?"] = "definitely me"; 
val test5b = longest_lowercase [] = "";

val test6a = caps_no_X_string "aBxXXxDdx" = "ABDD"; 
val test6b = caps_no_X_string "gxoxXxDxUckxs" = "GODUCKS";

val test7a = first_answer (fn x=>if x > 8 then SOME([x]) else NONE) [5,6,2,5,10,12,15] = [10];

val test8a = all_answers (fn x=>if x > 8 then SOME([x]) else NONE) [5,6,2,5,10,12,15]  = NONE;
val test8b = all_answers (fn x=>if x > 8 then SOME([x]) else NONE) [10,12,15]  = SOME [15,12,10];

val pata = TupleP([VariableP "goldScar", ConstructorP("purpleScar", WildcardP)]);
val patb = TupleP([VariableP "pump", WildcardP, TupleP([VariableP "pump", WildcardP, TupleP([VariableP "launch", WildcardP])])]);

val test9b = count_wildcards pata = 1;
val test9c = count_wild_and_variable_lengths pata = 9;
val test9d = count_a_var ("pump", patb) = 2; 

val test10a = check_pat pata = true;
val test10b = check_pat patb = false;


val test11a = match (Tuple [Unit, Constant 9], TupleP [VariableP "greenPump", ConstantP 9]) = SOME [("greenPump", Unit)]
val test11b = match (Tuple [Unit, Constant 12], TupleP [UnitP, ConstantP 12]) = SOME []

val test12a = first_match Unit [TupleP [VariableP "10", WildcardP, UnitP]] = NONE
val test12b = first_match Unit [VariableP "blueSMG"] = SOME [("blueSMG", Unit)] 
