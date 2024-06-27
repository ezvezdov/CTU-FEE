type Img = [String]

zero :: Img
zero = [".##.","#..#","#..#",".##."]

one :: Img
one =  ["...#","..##","...#","...#"]


merge_imgs :: Img -> Img -> Img
merge_imgs img1 img2 = [(img1 !! 0) ++ "." ++ (img2 !! 0),
                        (img1 !! 1) ++ "." ++ (img2 !! 1),
                        (img1 !! 2) ++ "." ++ (img2 !! 2),
                        (img1 !! 3) ++ "." ++ (img2 !! 3)]

get_bin :: Int -> Img
get_bin i = do
    let d = div i 2
    let m = mod i 2

    let cur = case m of 1 -> one
                        0 -> zero

    if d == 0 then cur else (merge_imgs (get_bin d) cur)

main :: IO ()
main = do
    putStrLn $ "Enter integer:"
    n_str <- getLine
    let n = read n_str :: Int
    let ans = get_bin n
    putStrLn $ (init(unlines ans))
    