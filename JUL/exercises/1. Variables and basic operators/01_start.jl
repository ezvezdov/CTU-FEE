# 1 exercise
x = 1.234
y = 1//2
z = x + y*im
typeof(x)
typeof(y)
typeof(z)

#\euler + Tab

promote(1.5, im)
# (1.5 + 0.0im, 0.0 + 1.0im)
# smollest data type to store all data

x = 3141.5926
round(x) # to nearest int
floor(x) # to smaller int
ceil(x) # to larger int
round(Int64, x) # round to int and change type to Int64
round(x, digits = 3) # round to 3 digits after coma
round(x, sigdigits = 3) # rounds to the specified number of significant digits in the base specified by the base keyword argument.

str = """
      str1
      str2 :)
      """

string("str1 ", a, "str2")
hw = "Hello," * " world" # "Hello, world"

h3 = "Hello"^3 # "HelloHelloHello"
h3 = repeat("Hello", 3)

a = 1.123
stra = "The variable a is of type $(typeof(a)), and its value is $(a)"
# "The variable a is of type Float64, and its value is 1.123"

myfunc = typeof
myfunc(a) # Float64

v = [1,2,3] # vector
typeof(v) # Vector{Int64} 
eltype(v) # Int64
length(v) # 3

t = (1,2,3) # tuple

fruit = join(["apples", "bananas", "pineapples"], ", ", " and ")
# "apples, bananas and pineapples"
split(fruit)

split(str, " and ") # usually split by " "
# ["apples, bananas", "pineapples"]

contains("JuliaLang is pretty cool!", "Julia")
# True

occursin("Julia", "JuliaLang is pretty cool!")
# True

endswith("figure.png", "png")
# True

replace("Sherlock Holmes", "Holmes" => "Homeless")
# "Sherlock Homeless"

replace("Sherlock Holmes", "e" => uppercase)
# "ShErlock HolmEs"




