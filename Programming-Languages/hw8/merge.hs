evens =  map(+2)  [0..]
odds  =  map(+2) [1..]
 
merge xs [] = xs
merge [] ys = ys 
merge (x: xs) (y:ys) = x : y : merge(xs)(ys)

-- head(merge evens odds) will terminate because the head of the list does not change.
-- length (merge evens odds) also never terminates, because the two lists being merged are
-- infinite sequences and therefore the two lists are never fully merged. 
stream1 = map(^3) [0..]
stream2 = map(3^) [0..]

xsA = map(+1) [0..]
xsB = map(^2) [0..]
stream3 = merge xsA xsB

stream4 = map(* (-1)) [0..]
