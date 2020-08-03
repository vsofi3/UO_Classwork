use "hw1.sml"; 

(* Tests for question 1 *)
val test1A = is_older((4,18,2020), (5,20,2020)) = true
val test1B = is_older((3,15,2020), (2,10,2020)) = false
val test1C = is_older((3,15,2020), (3,16,2020)) = true

(* Tests for question 2 *)
val test2A = number_in_month([(2,8,2020),(2,2,2020)],2) = 1
val test2B = number_in_month([(8,10,2020),(5,5,2020),(2,10,2016)], 1) = 0
val test2C = number_in_month([(8,5,2020),(5,1,2020),(2,10,2016)], 5) = 1

(* Tests for question 3 *)
val test3A = number_in_months([(2,2,2020),(2,1,2020)],[]) = 0
val test3B = number_in_months([(2,2,2020),(1,2,2020)],[2]) = 2
val test3C = number_in_months([(2,2,2020),(28,2,2000),(13,3,2020),(17,4,2018)],[2,4]) = 3

(* Tests for question 4 *)
val test4A = dates_in_month([], 2) = []
val test4B = dates_in_month([(2,1,2020), (2,2,2020), (3,2,2020)], 2) = [(2,2,2020), (3,2,2020)]
val test4C = dates_in_month([(2,1,2020), (2,2,2020), (23,3,2020)], 3) = [(23,3,2020)]

(* Tests for question 5 *) 
val test5A = dates_in_months([(21,2,2020),(1,3,2020)],[2,3]) = [(21,2,2020),(1,3,2020)];
val test5B = dates_in_months([], [2,3]) = [];

(* Tests for question 6 *)
val test6A = get_nth([], 2) = ""
val test6B = get_nth(["it's", "a", "big", "blue", "world"], 3) = "big"

(* Tests for question 7 *)
val test7A = date_to_string((1,1,2020)) = "January-1-2020";
val test7B = date_to_string((7,5,2020)) = "May-7-2020";

(* Tests for question 8 *)
val test8A = number_before_reaching_sum(12, [2,4,6,8,10]) = 2 
val test8B = number_before_reaching_sum(12, []) = 0
val test8C = number_before_reaching_sum(18, [2,4,6,8,10,12,14]) = 3

(* Tests for question 9 *)
val test9A = what_month(360) = 12
val test9B = what_month(1) = 1
val test9C = what_month(100) = 4

(* Tests for question 10 *) 
val test10A = month_range(58, 62) = [2,2,3,3]
val test10B = month_range(62, 58) = []

(* Tests for question 11 *)
val test11A = oldest([(2,28,2012),(1,8,2010)]) = SOME (1,8,2010);
val test11B = oldest([]) = NONE;
val test11C = oldest([(12,30,2020), (10,23,2020), (11,31,2020)]) = SOME (10,23,2020);

(* Tests for question 12 *)
val test12A = cumulative_sum([12,27,13]) = [12,39,52]
val test12B = cumulative_sum([]) = []
val test12C = cumulative_sum([10,11,12,13]) = [10,21,33,46]
