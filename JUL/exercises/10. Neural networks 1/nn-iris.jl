using Pkg
Pkg.activate(pwd())


##############################################################
# Prepare data
##############################################################

using RDatasets


iris = dataset("datasets", "iris")

# Transform dataset data (SepalLength,SepalWidth,PetalLength,PetalWidth) to matrix X.
X = Matrix(iris[:, 1:4])
# Transform Species field from dataset to vector y.
y = iris.Species


using Random

# This function splits dataset to training (80% of data by default) and testing set (20% of data).
# We use selectdim() to choose right subvector in many options of X matrix.
function split(X, y;dims=1,ratio_train=0.8,kwargs...)
    indexes = randperm(length(y))
    X_train = selectdim(X, dims, indexes[1:floor(Int,length(y) * ratio_train)])
    y_train = y[indexes[1:floor(Int,length(y) * ratio_train)]]

    X_test = selectdim(X, dims, indexes[(floor(Int,length(y) * ratio_train)+1):length(y)])
    y_test = y[indexes[(floor(Int,length(y) * ratio_train)+1):length(y)]]

    return [X_train,y_train,X_test,y_test]
end


X_train, y_train, X_test, y_test = split(X, y)



using Statistics

# Normalization function, we shoul normalize data to 
function normalize(X_train, X_test;dims=1,kwargs...)
    m = mean(X_train;dims) # EX - mean value (stredni hodnota)
    d = std(X_train;dims) # standart deviation, 𝝈, (varX)^2
    normalized_X_train = (X_train .- m) ./ d
    normalized_X_test = (X_test .- m) ./ d

    return [normalized_X_train, normalized_X_test]
end

X_train, X_test = normalize(X_train, X_test)



# Onehot transformation: From vector y = ["category1","category2","category1"...]
# to matrix y_onehot[catehories_n][i] , where y_onehot[:,i] = bitvector ( for ex. [1,0,0,0])
function onehot(y, classes)
    y_onehot = falses(length(classes), length(y))
    for (i, class) in enumerate(classes)
        y_onehot[i, y .== class] .= 1
    end
    return y_onehot
end

# Onecold transformation: From matrix y_onehot[catehories_n][i] to 
# vector y = ["category1","category2","category1"...]
function onecold(y,classes)
    oc = []
    for i in 1:size(y,2)
        oc = [oc; classes[findmax(y[:,i])[2]]]
    end
    return oc
end


#classes = unique(y)
#y_train_testing = onehot(y_train, classes)
#y_train_testing = onecold(y_train, classes)
# y_train == y_train_testing



using LinearAlgebra

# Function for preparing data
function prepare_data(X, y; do_normal=true, do_onehot=true, kwargs...)
    X_train, y_train, X_test, y_test = split(X, y; kwargs...)

    if do_normal
        X_train, X_test = normalize(X_train, X_test; kwargs...)
    end

    classes = unique(y)

    if do_onehot
        y_train = onehot(y_train, classes)
        y_test = onehot(y_test, classes)
    end

    return X_train, y_train, X_test, y_test, classes
end

Random.seed!(666)
iris = dataset("datasets", "iris")
X = Matrix(iris[:, 1:4])
y = iris.Species
X_train, y_train, X_test, y_test, classes = prepare_data(X', y; dims=2)


# Checking is correctly working using X transporanted with dims = 2
Random.seed!(666)
aux1 = prepare_data(X, y; dims=1)
Random.seed!(666)
aux2 = prepare_data(X', y; dims=2)
norm(aux1[1] - aux2[1]') # Norm = 0.0 => all is correct

##############################################################
# Creating Network
# * The first layer is a dense layer with the ReLU activation function.
# * The second layer is a dense layer with the identity activation function.
# * The third layer is the softmax.
##############################################################
struct SimpleNet{T<:Real}
    W1::Matrix{T}
    b1::Vector{T}
    W2::Matrix{T}
    b2::Vector{T}
end

# Constructor for SimpleNet. Input are integers, that represents input size of the three layers
# randn() - Generate a normally-distributed random number with mean 0 and standard deviation 1.
# randn(len) - generates vector, randn(len1,len2) generates matrix len1×len2
SimpleNet(len1,len2,len3) = SimpleNet(randn(len2, len1),randn(len2),randn(len3, len2),randn(len3))

Random.seed!(666)
m = SimpleNet(size(X_train,1), 5, size(y_train,1))


# Functor
# Computes the prediction (forward pass) of the neural network SimpleNet.
function (m::SimpleNet)(x)
    z1 = m.W1*x .+ m.b1
    a1 = max.(z1, 0)
    z2 = m.W2*a1 .+ m.b2
    return exp.(z2) ./ sum(exp.(z2), dims=1)
end


# m(X_train[:,1:2]) 

##############################################################
# Train the network
##############################################################


function grad(m::SimpleNet, x::AbstractVector, y; ϵ=1e-10)
    z1 = m.W1*x .+ m.b1
    a1 = max.(z1, 0)
    z2 = m.W2*a1 .+ m.b2
    a2 = exp.(z2) ./ sum(exp.(z2), dims=1)
    l = -sum(y .* log.(a2 .+ ϵ))

    e_z2 = exp.(z2)
    l_part = (- e_z2 * e_z2' + Diagonal(e_z2 .* sum(e_z2))) / sum(e_z2)^2

    l_a2 = - y ./ (a2 .+ ϵ)
    l_z2 = l_part * l_a2
    l_a1 = m.W2' * l_z2
    l_z1 = l_a1 .* (a1 .> 0)
    l_x = m.W1' * l_z1

    l_W2 = l_z2 * a1'
    l_b2 = l_z2
    l_W1 = l_z1 * x'
    l_b1 = l_z1

    return l, l_W1, l_b1, l_W2, l_b2
end

# The previous function grad can compute the gradient for only one sample. 
# Since the objective in training a neural network is a mean over all samples,
# this mean needs to be included externally. This is NOT the correct way of writing
# function. However, we decided to present it in the current way to keep the 
# presentation (relatively) simple. When such a simplification is included in the code,
# we should include a check such as x::AbstractVector to prevent unexpected errors.

g_all = [grad(m, X_train[:,k], y_train[:,k]) for k in 1:size(X_train,2)]
typeof(g_all)

function mean_tuple(d::AbstractArray{<:Tuple})
    Tuple([mean([d[k][i] for k in 1:length(d)]) for i in 1:length(d[1])])
end

g_mean = mean_tuple(g_all)
typeof(g_mean)


# Trainig the network via Gradient descent with the stepsize ꭤ = 1e-1 and 200 iterations
α = 1e-1
max_iter = 200
L = zeros(max_iter)
for iter in 1:max_iter
    grad_all = [grad(m, X_train[:,k], y_train[:,k]) for k in 1:size(X_train,2)]
    grad_mean = mean_tuple(grad_all)

    L[iter] = grad_mean[1]

    m.W1 .-= α*grad_mean[2]
    m.b1 .-= α*grad_mean[3]
    m.W2 .-= α*grad_mean[4]
    m.b2 .-= α*grad_mean[5]
end


# Plotting Loss function on the training set
using Plots
plot(L;
    xlabel="Iteration",
    ylabel="Loss function",
    label="",
    title="Loss function on the training set"
)


##############################################################
# Prediction
##############################################################

# Function which predict the labels for samples
predict(X) = m(X)
accuracy(X, y) = mean(onecold(predict(X), classes) .== onecold(y, classes))

# Show the accuracy on both training and testing sets.
println("Train accuracy = ", accuracy(X_train, y_train))
println("Test accuracy = ", accuracy(X_test, y_test))


##############################################################
# Exercises
##############################################################

#########################################
# Exercise 1: Keyword arguments
#########################################

# Function which returns ratio (procent of train samples)
ratio_train(train, test) = size(train,2) / (size(train,2) + size(test, 2))

# Testing ratio_train
X_train, y_train, X_test, y_test, classes = prepare_data(X', y;dims = 2,do_normal = false)
println("Ratio train/test = ", ratio_train(X_train, X_test))
X_train, y_train, X_test, y_test, classes = prepare_data(X', y;dims = 2,ratio_train=0.5)
println("Ratio train/test = ", ratio_train(X_train, X_test))


#########################################
# Exercise 2: Showing the contours
#########################################
Random.seed!(666)
X_train, y_train, X_test, y_test, classes = prepare_data(X[:,3:4]', y; dims = 2)

# Trainig the network via Gradient descent with the stepsize ꭤ = 1e-1 and 1000 iterations
m = SimpleNet(size(X_train,1), 5, size(y_train,1))

α = 1e-1
max_iter = 1000
L = zeros(max_iter)
for iter in 1:max_iter
    grad_all = [grad(m, X_train[:,k], y_train[:,k]) for k in 1:size(X_train,2)]
    grad_mean = mean_tuple(grad_all)

    L[iter] = grad_mean[1]

    m.W1 .-= α*grad_mean[2]
    m.b1 .-= α*grad_mean[3]
    m.W2 .-= α*grad_mean[4]
    m.b2 .-= α*grad_mean[5]
end

# Vizualizing predicitoin
colours = [:blue, :red, :green]
xs = -2:0.01:2
plt = heatmap(xs, xs, (x, y) -> onecold(m([x; y]), 1:3)[1];
    color = colours,
    opacity = 0.2,
    axis = false,
    ticks = false,
    cbar = false,
    legend = :topleft,
)

# Vizualizing predictions from testing split
for (i, class) in enumerate(classes)
    inds = findall(onecold(y_test, classes) .== class)
    scatter!(plt, X_test[1, inds], X_test[2, inds];
        label = class,
        marker=(8, 0.8, colours[i]),
    )
end
display(plt)

#########################################
# Exercise 3: Overfitting
#########################################

Random.seed!(666)
X_train, y_train, X_test, y_test, classes = prepare_data(X[:,3:4]', y; dims = 2)

# Trainig the network via Gradient descent with the stepsize ꭤ = 1e-1 and 1000 iterations
m = SimpleNet(size(X_train,1), 25, size(y_train,1))

α = 1e-1
max_iter = 25000
L = zeros(max_iter)
for iter in 1:max_iter
    grad_all = [grad(m, X_train[:,k], y_train[:,k]) for k in 1:size(X_train,2)]
    grad_mean = mean_tuple(grad_all)

    L[iter] = grad_mean[1]

    m.W1 .-= α*grad_mean[2]
    m.b1 .-= α*grad_mean[3]
    m.W2 .-= α*grad_mean[4]
    m.b2 .-= α*grad_mean[5]
end

# Vizualizing predicitoin
colours = [:blue, :red, :green]
xs = -2:0.01:2
plt = heatmap(xs, xs, (x, y) -> onecold(m([x; y]), 1:3)[1];
    color = colours,
    opacity = 0.2,
    axis = false,
    ticks = false,
    cbar = false,
    legend = :topleft,
)

# Vizualizing predictions from testing split
for (i, class) in enumerate(classes)
    inds = findall(onecold(y_test, classes) .== class)
    scatter!(plt, X_test[1, inds], X_test[2, inds];
        label = class,
        marker=(8, 0.8, colours[i]),
    )
end
display(plt)