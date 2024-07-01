x = 1
y = 2
a = 3
b = 4

# If condition
if x < y
    z = y
else
    z = x
end

# We can write return before if, we can write var = before if
function fact(n)
    return if n == 0
        1
    else
        fact(n) * fact(n-1)
    end
end

# In the expression a && b, the subexpression b is only evaluated if a evaluates true.
# In the expression a || b, the subexpression b is only evaluated if a evaluates to false.

function fact2(n)
    isinteger(n) && n >= 0 || error("argument must be non-negative integer")
    return n == 0 ? 1 : fact2(n) * fact2(n-2)
end


# Ternary operator
x < y ? a : b

# While loop
i = 1
while i <= 5
    @show i # or @info
    i += 1
end

# Show macro
a, b, c = 1, "hello", :world;
@show (a, b, c); # (a, b, c) = (1, "hello", :world)

# For loop
for i in 1:5
    @show i
end

# For in lists
persons = ["Alice", "Bob", "Carla", "Daniel"];
for name in persons
    println("Hi, my name is $name.")
end

# For in dictionary
persons = Dict("Alice" => 10, "Bob" => 23, "Carla" => 14, "Daniel" => 34);
for (name, age) in persons
    println("Hi, my name is $name and I am $age old.")
end

# EX: Use for or while loop to print all integers between 1 and 100 which can be divided by both 3 and 7.
for i in 1:100
    if mod(i,3) == 0 && mod(i,7) == 0
        println(i)
    end 
end

# EX: Rewrite the code from the exercise above. Use a combination of the while loop and the keyword continue to print all integers between 1 and 100 divisible by both 3 and 7. In the declaration of the while loop use the true value instead of a condition. Use the break keyword and a proper condition to terminate the loop.
i = 0
while true
    i+=1
    i > 100 && break
    mod(i,3) == mod(i,7) == 0 || continue
    println(i)

end

# Nested loops
for i in 1:3
    for j in i:3
        @show (i, j)
    end
end
# Or
for i in 1:3, j in i:3
    @show (i, j)
end

# EX: Use nested loops to create a matrix with elements given by the formula
# Ai,j = 1/2exp{ 1/2(xi^2 - yj^2)}  i ∈ {1,2,3},j ∈ {1,2,3,4}
# where x ∈ {0.4,2.3,4.6} and y ∈ {1.4,−3.1,2.4,5.2}.

x = [0.4, 2.3, 4.6]
y = [1.4, -3.1,2.4,5.2]
A = zeros(Float64, length(x), length(y))
for i in 1:length(x), j in 1:length(y)
    A[i, j] = exp((x[i]^2 - y[j]^2)/2)/2
end

# Generate lists
x = [x^2 for x in 1:4]

# We can use if
[(x, y, x + y)  for x in 1:10, y in 1:10 if x + y < 5]

# Zip
for (i, j, k) in zip([1, 4, 2, 5], 2:12, (:a, :b, :c))
    @show (i, j, k)
end

# Using eachrow, return vector = ith row of A
for (i,row) in enumerate(eachrow(A))
    println("i = $i, i_sum = $(sum(row))")
end

# Using eachcol, return vector = ith column of A
for (i,col) in enumerate(eachcol(A))
    println("i = $i, i_sum = $(sum(col))")
end

# Use global or local variable in function/loop
# glboal s = 0
# local s = 0


import Pkg
Pkg.precompile()
using Plots

x = 0:0.01π:2π
plot(x, sin.(x); label = "sinus", linewidth = 2)
