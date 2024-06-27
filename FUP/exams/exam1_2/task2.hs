sym_diff :: Int -> Int -> Int
sym_diff n m = (n-m)*(m-n)

get_next :: (Int -> Int -> Int) -> [Int] -> [Int]
get_next op lst  = do
    if (length lst) == 1 then [] else [op (lst !! 0) (lst !! 1)] ++ get_next op (tail lst)
    
start_pascal :: (Int -> Int -> Int) -> [Int] -> Int -> Int -> [Int]
start_pascal op lst r cur_r = do
    let next_row = [(head lst)] ++ (get_next op lst) ++ [(head lst)]
    if r == cur_r then lst else start_pascal op next_row r (cur_r + 1)
    

pascal :: (Int -> Int -> Int) -> Int -> Int -> [Int]
pascal op v r = start_pascal op [v] r 0

get_function :: String -> (Int -> Int -> Int)
get_function "add" = (+)
get_function "mul" = (*)
get_function "sym_diff" = (sym_diff)

main :: IO ()
main = do
    putStrLn $ "Enter operation:"
    op_str <- getLine
    let op = get_function op_str

    putStrLn $ "Enter vertex:"
    v_str <- getLine
    let v = read v_str :: Int

    putStrLn $ "Enter row:"
    r_str <- getLine
    let r = read r_str :: Int

    putStrLn $ show (pascal op v r)

    
