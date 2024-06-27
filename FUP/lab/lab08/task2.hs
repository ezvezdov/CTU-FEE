let symbols = ['0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F']

numToStr :: Int -> Int -> String
numToStr num radix = 
        | num < radix = symbols !! num
        | num == 0 = show num
        | otherwise = "" ++ numToStr (num div radix) radix

main :: IO ()
main = do
    let test = numToStr 5 10
    -- putStrLn $ "Evens: " ++ show evens
    putStrLn $ "Output: " ++ test


