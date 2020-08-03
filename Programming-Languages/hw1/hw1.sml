(* Date Format: (year, month, day) or (int, int, int)  *)
(* 1 *)
fun is_older (d1: (int * int * int), d2: (int * int * int)) = 
    if #3 d2 > #3 d1 then true
    else
        if #2 d2 > #2 d1 andalso #3 d2 = #3 d1
        then true
        else
            if #1 d2 > #1 d1 andalso #2 d2 = #2 d1 andalso #3 d2 = #3 d1
            then true
            else
                false

(* 2 *)
fun number_in_month (ld: (int * int * int) list, n: int) = 
    if null ld then 0
    else 
        if #2 (hd ld) = n
        then 1 + number_in_month(tl ld, n)
        else number_in_month(tl ld, n)

(* 3 *)
fun number_in_months ( ld: (int * int * int) list, lm : int list) =
    if null ld orelse null lm then 0
    else if null (tl lm) 
        then number_in_month(ld, hd lm)
        else 
            number_in_month(ld, hd lm) + number_in_months(ld, tl lm)

(* 4 *)
fun dates_in_month(ld : (int * int * int) list, n : int) =
    if null ld then []
    else
        if #2 (hd ld) = n then (hd ld) :: dates_in_month(tl ld, n)
        else dates_in_month(tl ld, n)

(* 5 *)
fun dates_in_months(ld: (int * int * int) list, lm: int list) = 
    if null lm then []
    else
        dates_in_month(ld, hd lm) @ dates_in_months(ld, tl lm)

(* 6 *)
fun get_nth (ls : string list, n : int) =
    if null ls then ""
    else if n = 1 then hd ls
    else get_nth(tl ls, n - 1)

(* 7 *)
fun date_to_string (date: (int * int * int)) = 
    let val monthList = ["January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"]
    in 
        get_nth(monthList, #2 date) ^ "-" ^ Int.toString(#1 date) ^ "-" ^ Int.toString(#3 date)
    end

(* 8 *)
fun number_before_reaching_sum (endVal : int, numList : int list) = 
    if null numList then 0
    else if hd numList >= endVal then 0
    else 1 + number_before_reaching_sum (endVal - hd numList, tl numList)

(* 9 *)
fun what_month(day : int) = 
    let val daysInMonth = [31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31]
    in 
        number_before_reaching_sum (day, daysInMonth) + 1
    end

(* 10 *) 
fun month_range(day1 : int, day2 : int) = 
    if day1 >= day2 then []
    else what_month(day1) :: month_range(day1 + 1, day2)

(* 11 *)
fun oldest(listOfDates : (int * int * int) list) = 
    if null listOfDates then NONE
    else
        let fun oldestNest (listOfDates : (int * int * int) list) = 
            if null(tl listOfDates) then hd listOfDates
            else let val oldestTail = oldestNest(tl listOfDates)
            in
                if is_older(hd listOfDates, oldestTail) then hd listOfDates
                else oldestTail
            end
        in SOME(oldestNest(listOfDates)) end

(* 12 *)
fun cumulative_sum (nums : int list) = 
    if null nums then []
    else
        let fun sumNest (nums : int list, curSum: int) =
            if null nums then []
            else
                (hd nums) + curSum :: sumNest(tl nums, (hd nums) + curSum) 
        in 
            hd nums :: sumNest(tl nums, hd nums)
        end 
