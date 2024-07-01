using RDatasets
using LinearAlgebra
using Statistics

#########################################################
projection(x, x_min, x_max) = min.(max.(x, x_min), x_max)
#########################################################

function computeQ(X, y)
    n = length(y)
    Q = zeros(n,n)
    for i in 1:n
        for j in 1:n
            Q[i,j] = y[i] * y[j] * (X[i,:])' * (X[j,:])
        end
    end
    return Q
end
# computeQ(X, y) = (y * y') .* (X * X')

function computeW(X, y, z)
    n = size(X,1)
    w = zeros(size(X,2))
    for i in 1:n
        w += y[i] * z[i] * X[i,:]
    end
    return w
end


function solve_SVM_dual(Q, C;max_epoch=100, kwargs...)
    n = size(Q,1)
    z = zeros(n)

    for _ in 1:max_epoch
        for i in 1:n
            grad = (Q * z) .- 1
                
            z[i] = z[i] - grad[i]/Q[i,i];
            z[i] = projection(z[i],0,C)
        end
    end
    return z
end

function solve_SVM(X, y, C; kwargs...)
    Q = computeQ(X, y)
    z = solve_SVM_dual(Q,C; kwargs...)
    w = computeW(X,y,z)
    return w
end

function iris(C::Real;dims=1,kwargs...)
    classes = ["versicolor","virginica"]

    iris_dataset = dataset("datasets", "iris")
    filter!(row -> !(row.Species == "setosa"),  iris_dataset)
    # Transform dataset data (PetalLength,PetalWidth) to matrix X.
    X = Matrix(iris_dataset[:, 3:4])

    # Transform Species field from dataset to vector y.
    y = iris_dataset.Species
    
    # y[i] ∈ {-1,1}
    # versicolor: 1 | virginica: -1
    y = (y .== classes[1]) - (y .== classes[2])

    # Normalization
    col_mean = mean(X; dims)
    col_std = std(X; dims)
    X = (X .- col_mean) ./ col_std

    # Add bias
    X = hcat(X, ones(size(X,1)))

    # Calculate w via SVM
    w = solve_SVM(X,y,C; kwargs...)
    return w 
end

# using Plots
# using DataFrames
# using StatsPlots
# using RDatasets

# w = iris(10)
# iris_dataset = dataset("datasets", "iris")
# filter!(row -> !(row.Species == "setosa"),  iris_dataset)
# Transform dataset data (PetalLength,PetalWidth) to matrix X.
# X = Matrix(iris_dataset[:, 3:4])

# Transform Species field from dataset to vector y.
# y = iris_dataset.Species

# y[i] ∈ {-1,1}
# versicolor: 1 | virginica: -1
# classes = ["versicolor","virginica"]
# y = (y .== classes[1]) - (y .== classes[2])

# X = hcat(X, ones(size(X,1)))

# separ(x::Real, w) = (-w[3]-w[1]*x)/w[2]

# xs = extrema(iris_dataset.PetalLength) .+ [-0.1, 0.1]
# ys = extrema(iris_dataset.PetalWidth) .+ [-0.1, 0.1]

# @df iris_dataset scatter(
#     :PetalLength,
#     :PetalWidth;
#     group = :Species,
#     xlabel = "Petal length",
#     ylabel = "Petal width",
#     legend = :topleft,
#     xs,
#     ys
# )


# plot!(xs, x -> separ(x,w); label = "Separation", line = (:black,3))
