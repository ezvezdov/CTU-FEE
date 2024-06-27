module Hw3 where
type Symbol = String
data Expr = Var Symbol | App Expr Expr | Lambda Symbol Expr deriving Eq
type Env = [(Symbol, Expr)]

-- To show Expr datatype
instance Show Expr where
    show (Var x) = x
    show (App e1 e2) = "(" ++ show e1 ++ " " ++ show e2 ++ ")"
    show (Lambda s e) = "(\\" ++ s ++ "." ++ show e ++ ")"


-- Check if pair (s,_) exits in env
is_in_env :: Symbol -> Env -> Bool
is_in_env s [] = False
is_in_env s env = if fst (head env) == s then True else (is_in_env s (tail env))



-- Get Expr `e`  from env [... (s,e) ...] if exists (else return (Var s) )
get_from_env :: Symbol -> Env -> Expr
get_from_env s [] = (Var s)
get_from_env s env = if fst (head env) == s then snd (head env) else get_from_env s (tail env)


-- remove_from_env :: Symbol -> Env -> Env
-- remove_from_env s [] = []
-- remove_from_env s env = if s == fst (head env) then (tail env) else [(head env)] ++ (remove_from_env s (tail env))

-- Remove pair (s,_) from env
remove_from_env :: Symbol -> Env -> Env
remove_from_env key env = filter (\(k, _) -> k /= key) env

-- Check if symbol is bounded in Expression
-- 1 = symbol is bounded
-- 0 = symbol is free
-- -1 = symbol is not in Expr
is_bound_in_Expr :: Symbol -> Expr -> Int
is_bound_in_Expr s_to_check (Var s)
    | s == s_to_check = 0
    | otherwise = -1
is_bound_in_Expr s_to_check (App e1 e2) = do
    let check_1 = (is_bound_in_Expr s_to_check e1)
    if check_1 == 1 then 1 else max check_1 (is_bound_in_Expr s_to_check e2)
is_bound_in_Expr s_to_check (Lambda s_cur e)
    | (s_to_check == s_cur) = 1
    | otherwise = is_bound_in_Expr s_to_check e

-- Check if variable `s` is free in env
-- Returns False when s isn't free or isn't in expressions
-- 1 = symbol is not free
-- 0 = symbol is free
-- -1 = symbol is not in Expr
is_free :: Symbol -> Env -> Int
is_free s [] = -1
is_free s env = do
    let f = (is_bound_in_Expr s (snd (head env)))
    if (f == 1) then 1
    else max f (is_free s (tail env))

b_reduce :: Expr -> Env -> Int -> Bool -> Expr
b_reduce (Var s) env i was_redux = get_from_env s env

b_reduce (App (Lambda s1 e1) e2) env i was_redux = (b_reduce e1 ([(s1,(b_reduce e2 env i False))] ++ env) i True)
b_reduce (App e1 e2) env i was_redux = (App (b_reduce e1 env i False) (b_reduce e2 env i False))

b_reduce (Lambda s e) env i was_redux
    | (is_in_env s env) && not (was_redux) = (Lambda s (b_reduce e (remove_from_env s env) i False) )
    | (is_free s env) == 0 = (Lambda ("a" ++ (show i)) (b_reduce e ( [(s, (Var ("a" ++ (show i) ) ) )] ++ env) (i+1) False))
    | otherwise = (Lambda s  (b_reduce e env i False) )

eval :: Expr -> Expr
eval e = do
    let new = b_reduce e [] 0 False
    if e == new then
        e
    else eval new
        

-- Testing variables
ex :: Expr
ex = App (Lambda "x" (Lambda "y" (Lambda "z" (App (App (Var "x") (Var "y")) (Var "z"))))) (App (Var "y") (Var "z"))

one :: Expr
one = Lambda "s" (Lambda "z" (App (Var "s") (Var "z")))

suc :: Expr
suc = Lambda "w"
        (Lambda "y"
            (Lambda "x"
                (App (Var "y")
                    (App (App (Var "w") (Var "y"))
                        (Var "x")
                    )
                )
            )
        )
