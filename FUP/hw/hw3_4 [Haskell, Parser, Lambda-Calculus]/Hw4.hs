module Hw4 where
import Control.Applicative
import Data.Char
import Parser
import Hw3

type ENV = [(Symbol, Expr)]


get_from_ENV :: Symbol -> ENV -> Expr
get_from_ENV s [] = (Var s)
get_from_ENV s env = if fst (head env) == s then snd (head env) else get_from_ENV s (tail env)


change_var_expr :: Expr -> ENV -> Expr
change_var_expr (Var v) env = get_from_ENV v env
change_var_expr (Lambda v e) env = (Lambda v (change_var_expr e env))
change_var_expr (App e1 e2) env = (App (change_var_expr e1 env) (change_var_expr e2 env))



parse_env :: ENV -> ENV -> ENV
parse_env old_env new_env = do
    let s = fst (head old_env)
    let e = snd (head old_env)
    
    let parsed_e = change_var_expr e new_env
    
    if (length old_env) == 0 
        then new_env 
        else (parse_env (tail old_env) (new_env ++ [(s,parsed_e)]))



    

space :: Parser ()
space = do many (sat isSpace) 
           return ()

var :: ENV -> Parser Expr
var env = do 
    v <- some alphaNum
    return (get_from_ENV v env)

var_name :: Parser Symbol
var_name = do 
    v <- some alphaNum
    return v

lambda :: ENV -> Parser Expr
lambda env = do
    char '('
    char '\\'
    vn <- var_name
    char '.'
    e <- expr env
    char ')'
    return (Lambda vn e)


app :: ENV -> Parser Expr
app env = do
    char '('
    e1 <- expr env
    sep
    e2 <- expr env
    char ')'
    return (App e1 e2)

def :: Parser (Symbol,Expr)
def = do
    vn <- var_name
    sep
    string ":="
    sep
    e <- expr []
    sep
    return (vn, e)


expr :: ENV -> Parser Expr
expr env = do (var env) <|> (lambda env) <|> (app env)

prg :: Parser Expr
prg = do
    env <- many def
    e <- expr (parse_env env [])
    space
    return e

readPrg :: String -> Maybe Expr
readPrg p = case parse prg p of
                Nothing -> Nothing
                Just (e,s) | length s == 0 -> Just e
                Just (e,s) | length s /= 0 -> Nothing

