separate :: [Int] -> ([Int], [Int])
separate [] = ([],[])
separate [x] = ([x],[])
separate (x:y:xs) = let (evs,ods) = separate  xs
                    in (x:evs, y:ods)
                
main :: IO ()
main = do
    let (evens, odds) = separate [1, 2, 3, 4, 5]
        image.png ++ show evens
    putStrLn $ "Odds: " ++ show odds
    