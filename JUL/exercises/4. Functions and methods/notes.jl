function power(x::Real, p::Integer)
    if p == 0
        return 1
    elseif p > 0
        return x * power(x, p-1)
    else
        return power(1/x,-p)
    end
end

# Oneline function
even(x) = mod(x,2) == 0 ? true : false

# Function with optional parameters (keywords)
# Use: q(4,2;c=3)
q(x, y; a=1, b=2*a, c=3*(a+b)) = a * x^2 + b * x * y + c * y^2

function gauss(x; mu::Real = 0, o::Real = 1)
    if !(o^2 > 0)
        return 0
    end
    return (exp((-1/2) * ((x-mu)/o)^2)/(o * sqrt(2 * pi)))
end

# Variable count of arguments
nargs(x...) = println("Number of arguments: ", length(x))

function myround(x; type = :round, kwargs...)
    return if type == :floor
        floor(x; kwargs...)
    elseif type == :ceil
        ceil(x; kwargs...)
    else
        round(x; kwargs...)
    end
end

# The do ... end block creates an anonymous function with inputs (x, y)
map([1,3,-1], [2,4,-2]) do x, y
    println("x = $(x), y = $(y)")
    return x + y
end

# Opening files with do ... end
# open("outfile", "w") do io
#     write(io, data)
# end

# Function composition
# (g∘f)(x)=g(f(x)),∀x∈X.
# \circ + Tab = ∘
(sqrt ∘ +)(3, 6) # equivalent to sqrt(3 + 6)
(sqrt ∘ abs ∘ sum)([-3, -6, -7])  # equivalent to sqrt(abs(sum([-3, -6, -7])))
# Function piping
[-3, -6, -7] |> sum |> abs |> sqrt

using InteractiveUtils: supertype

function supertypes_tree(T::Type, level::Int = 0)
    println(repeat(" ",level),T)
    if T === Any
        return
    end 
    supertypes_tree(supertype(T),level+1)
    
end

function subtypes_tree(T::Type, level::Int = 0)
    println(repeat("   ", level), T)
    subtypes_tree.(subtypes(T), level + 1)
    return
end

supertypes_tree(Float64)
subtypes_tree(Number)

# Check is Type inside another type
Float64 <: AbstractFloat <: Real <: Number

# Exercise for Multiple dispatch
abstract type Student end

struct Master <: Student
    salary
end

struct Doctoral <: Student
    salary
    exam_mid::Bool
    exam_english::Bool
end

subtypes_tree(Student)

salary_monthly(s::Master) = s.salary
salary_monthly(s::Doctoral) = s.salary + s.exam_mid*2000 + s.exam_english*1000
salary_yearly(s::Student) = 12*salary_monthly(s)


s1 = Master(5000)
s2 = Doctoral(30000, 1, 0)
println("The yearly salary is $(salary_yearly(s1)).")
println("The yearly salary is $(salary_yearly(s2)).")

module A
    a = 1 # a global in A's scope
    b = 2 # b global in A's scope
end
using .A: b
A.a # 1
b # 2

# @time macro to check time and allocs
@time power(5,2);

# Exceptions
# error("argument must be non-negative integer")
# throw(DomainError(n, "argument must be non-negative integer"))

