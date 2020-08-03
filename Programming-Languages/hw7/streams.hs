code = 1 : code

intList n = n : (intList (n+1))

takeN n [] = []
takeN n (x:xs) = if (n > 0) then x : takeN (n - 1) xs else []
