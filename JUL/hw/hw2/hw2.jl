using  Random

# Generate unique min/max/stationary points
function generate_solutions(f, g, P, x_min::AbstractArray, x_max::AbstractArray)
    data_rounded = []
    data = []

    for i in 0:1250
        cur_x = zeros(size(x_max,1))
        for j in eachindex(x_max)
            cur_x[j] = rand(x_min[j]-0.001:0.01:x_max[j]-0.001)
        end
        out = optim(f, g, x -> P(x,x_min,x_max), cur_x; α=0.1)
        
        out_rounded = round.(out,digits=0)

        if !(out_rounded in data_rounded)
            push!(data_rounded,out_rounded)
            push!(data,out)
        end
        
    end

    if size(data,1) != 0
        return mapreduce(permutedims, vcat, data)'
    end
    return Matrix{Float64}(undef, size(x_min,1), 0)
end

generate_solutions(f, g, P, x_min::Real, x_max::Real) = generate_solutions(f, g, P, [x_min], [x_max])

# Griewank function
function f_griewank(x::AbstractArray)
    prod_val = 1
    sum_val = 0
    for i in eachindex(x)
        sum_val += x[i]^2
        prod_val *= cos(x[i]/sqrt(i))
    end
    fun_val = 1 + sum_val/4000 - prod_val

    return fun_val
end
f_griewank(x::Real) = f_griewank([x])

# Derivation of Griewank function
function g_griewank(x::AbstractArray)    
    gradf = []
    for i in eachindex(x)
        prod_val = 1
        for j in eachindex(x)
            if j == i
                continue
            end
            prod_val *= cos(x[j]/sqrt(j))
        end
        prod_val *= (-sin(x[i]/sqrt(i)))/sqrt(i)
        cur_val = x[i]/2000 - prod_val
        append!(gradf,cur_val)
    end

    return gradf
end
g_griewank(x::Real) = g_griewank([x])[1]


# Find minimum
function optim(f, g, P, x; α=0.01, max_iter=10000)
    for _ in 1:max_iter
        y = x - α*g(x)
        x = P(y)
    end
    return x
end
# Projection to box
P(x, x_min, x_max) = min.(max.(x, x_min), x_max)

# f function
# f(x) = sin(x[1] + x[2]) + cos(x[1])^2
# f(x1,x2) = f([x1,x2])
# g(x) = [cos(x[1] + x[2]) - 2*cos(x[1])*sin(x[1]); cos(x[1] + x[2])]

# Ranges for plot
# min_x = [40,40]
# max_x = [55,55]
# xs = range(min_x[1], max_x[1], length = 1000)
# ys = range(min_x[2], max_x[2], length = 1000)
# using Plots

# 1D f_griewank
# matrix = generate_solutions(f_griewank, g_griewank, P,min_x[1], max_x[1])
# plot(xs, f_griewank, color = :jet, label="f_griewank 1D")

# 1D Visualize points 
# ys = []
# for i in eachindex(matrix)
#     push!(ys,f_griewank(matrix[i]))
# end
# display(scatter!(matrix[1,:], ys))


# 2D f_griewank
# matrix = generate_solutions(f_griewank, g_griewank, P,min_x, max_x)
# contourf([xs, ys], f_griewank, color = :jet, label="f_griewank 2D")

# 2D f
# matrix = generate_solutions(f, g, P, min_x, max_x)
# contourf(xs, ys, f, color = :jet, label="f 2D")

# display points of 2D function
# display(scatter!(matrix[1,:], matrix[2,:],color="white",label=nothing))

# show(IOContext(stdout, :limit=>false), MIME"text/plain"(), matrix)