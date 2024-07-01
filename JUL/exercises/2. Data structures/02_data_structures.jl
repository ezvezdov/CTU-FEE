using Pkg
Pkg.activate(pwd())

# Vector Indexing
v = [1,2,3]
v[begin] # return 1st element
v[end] # return the last element
v[1:3] # the first 3 elements
v[1:2:end] # all elements with odd index
v[:] # all element
v |> size # size(v)

# Ranges
x = 1:2:11 # from 1 to 11 with step 2
range(1, stop = 10, step = 2)
collect(x) # [1,3,5,7,9,11]

# Vector appending
v = [1,2,3]
append!(v,4) # [1,2,3,4]
append!(v[5,6]) # [1,2,3,4,5,6]
append!(v,7:8) # [1,2,3,4,5,6,7,8]

# One element type in array!!!
v = Float64[1,2,3]
v[2:3] .= 11.0 # change elements in range 2:3 to 11.0

# Matrix
M = [1 2 3 4; 5 6 7 8] # 2x4 Matrix
ndims(M) # return 2
M[2] # return 5
# 1  2  3  4
# 5  6  7  8
# matrix[row, column]
M[1, :] # firtst row

# Concatenation
hcat(M,M) # horizontal concatenation
vcat(M,M) # vertical concatenation
cat(M,M; dims = 1) # dims: 1-vcat, 2-hcat

# Vector filling
zeros(5, 2) # Matrix 5x2 filled by zeros
ones(5, 2) # Matrix 5x2 filled by ones
num = 65
fill(num, 5, 2) # Matrix 5x2 filled by num 
repeat("ha",3) # "hahaha"

# Help command
# shift + ? = help in julia conole

# Random
rand() # generage random number in range [0,1]
rand(5) # generage vector with size 5 with random number in range [0,1]
randn() # ??? 
rand(["cat","dog"],5) # vector with size 5 with random str from []

# Broadcasing
v = [1,2,3]
v .-= 3 # [-2, -1, 0], operation for all elements
abs.(a) # [2, 1, 0], abs for all elements

A = @. (exp((B +1)^2))/2 # Macros @, instead of (exp.((B .+1).^2))./2

a = [1,2,3]
b = [4,5,6]
# ERROR: a * b
a' * b # a' = transposed  a
b = a # pointer to a
b = copy(a) # copy of a

A = [1 2; 3 4]
view(A, :, 1) # reference to elements in range

# Tuples
t = (1, 2.0, "3") # tuple
a ,b ,c = t # unpacking elements

# Named Tuples
t = (a = 1, b = 2.0, c = "3")
t.a # equals t[1]

# Dictionaries
d = Dict("a" => [1,2,3], "b" => 1) # d["a"] = [1,2,3]
d = Dict(:a => [1,2,3], :b => 1) # Use symbols instead of strings, d[:a] = [1,2,3]
keys(d) # :a :b
d |> keys # equals keys(d)
delete!(d,:a) # delete :a element
haskey(d, :c) # check is dicitonary has key
pop!(d, :a) # pop value with key