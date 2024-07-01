# Exercise 1: Solving a system of linear equations
using BenchmarkTools
using LinearAlgebra

n = 1000
A = randn(n,n)
b = randn(n)

# inv(A)*b == A\b (specialized algorithm)
norm(inv(A)*b - A \ b)

#@btime inv($A)*($b)
#@btime ($A) \ ($b)

#################################################
# Exercise 2: Bisection method
# Finding minimum of funciton using derivate and bisection method
function bisection(f, a, b; tol=1e-6)
    fa = f(a)
    fb = f(b)
    fa == 0 && return a
    fb == 0 && return b
    fa*fb > 0 && error("Wrong initial values for bisection")
    while b-a > tol
        c = (a+b)/2
        fc = f(c)
        fc == 0 && return c
        if fa*fc > 0
            a = c
            fa = fc
        else
            b = c
            fb = fc
        end
    end
    return (a+b)/2
end

f(x) = x^2 - x
g(x) = 2*x - 1
x_opt = bisection(g, -1, 1)

#################################################
# Exercise 3: JuMP
using JuMP
using GLPK

model = Model(GLPK.Optimizer)

@variable(model, x1 >= 0)
@variable(model, x2 >= 0)
@variable(model, x3 >= 0)
@variable(model, x4 >= 0)
@variable(model, x5 >= 0)

@objective(model, Min, x1 + x2 + x5)

@constraint(model, c1, x1 + 2*x2 + 3*x3 + 4*x4 + 5*x5 == 8)
@constraint(model, c2, x3 + x4 + x5 == 2)
@constraint(model, c3, x1 + x2 == 2)

println(model)

optimize!(model)
termination_status(model)
primal_status(model)
dual_status(model)
objective_value(model)

answer = [value(x1), value(x2),value(x3),value(x4),value(x5)]

#################################################

# Exercise 4: SQP method

n = 10

f(x) = sum((1:n) .* x.^4)
f_grad(x) = 4*(1:n).*x.^3
f_hess(x) = 12*Diagonal((1:n).*x.^2)

# function f(x)
#     sum = 0
#     for i in 1:n
#         sum += i*x[i]^4
#     end

#     return sum
# end

# function f_grad(x)
#     grad = zeros(n)
#     sum_all = f(x)
#     for i in 1:n
#         grad[i] = sum_all - (i * x[i]^4) + (4*i*x[i]^3)
#     end
#     return grad
# end

# function f_hess(x)
#     hessian = zeros(n,n)
#     grad = f_grad(x)
#     for i in 1:n
#         for j in 1:n
#             hessian[i,j] = grad[i] * grad[j]
#         end
#     end
#     return hessian
# end

h(x) = sum(x) - 1
h_grad(x) = ones(n)
h_hess(x) = zeros(n,n)

x = randn(n)
μ = randn()
for i in 1:100
    global x, μ
    A = [f_hess(x) + μ*h_hess(x) h_grad(x); h_grad(x)' 0]
    b = [f_grad(x) + μ*h_grad(x); h(x)]
    step = A \ b
    x -= step[1:n]
    μ -= step[n+1]
end