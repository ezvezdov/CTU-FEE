
using Pkg
# Pkg.activate(pwd())

# # Linear regression
# ## Loading and preparing data

using Plots
using StatsPlots
using RDatasets

iris = dataset("datasets", "iris")
iris[1:5,:]

# ### Exercise:
# We will simplify the goal and estimate the dependence of petal width on petal length. # Create the data $X$ (do not forget to add the bias) and the labels $y$.
# 
# Make a graph of the dependence of petal width on petal length.
# 
# ---
# ### Solution:

y = iris.PetalWidth
X = hcat(iris.PetalLength, ones(length(y)))


display(scatter(iris.PetalLength,iris.PetalWidth,label="",xlabel = "Petal length",ylabel = "Petal width"))


# ---
# 
# ## Training the classifier
# ### Exercise:
# Use the closed-form formula to get the coefficients $w$ for the linear regression. Then
# use the `optim` method derived in the previous lecture to solve the optimization problem 
# via gradient descent. The results should be identical.
#
#---
#### Solution:

w = (X'*X) \ (X'*y)

g(w) = X'*(X*w - y)
f(w) = norm(X*w - y)^2

abstract type Step end

struct GD <: Step
    α::Real
end

optim_step(s::GD, f, g, x) = -s.α*g(x)

function optim(f, g, x, s::Step; max_iter=100)
    for i in 1:max_iter
        x += optim_step(s, f, g, x)
    end
    return x
end

w2 = optim(f, g, zeros(size(X,2)), GD(1e-4); max_iter=10000)




# ---
# 
# ### Exercise:
# Write the dependence on the petal width on the petal length. Plot it in the previous
# graph.
# 
# ---
# ### Solution:

f_pred(x,w) = w[1]*x + w[2]

x_min = floor(findmin(X)[1])
x_max = round(findmax(X)[1])

plot(x -> f_pred(x,w2),x_min:x_max,color="black",label="predict",line = (:black,3))
scatter!(iris.PetalLength,iris.PetalWidth,label="",xlabel = "Petal length",ylabel = "Petal width",color="blue")

# ### Exercise:
# Create the `iris_reduced` dataframe in the following way:
# - Label "setosa" will be deleted.
# - Label "versicolor" will be the negative class.
# - Label "virginica" will be the positive class.
# - Add the `intercept` column with ones as entries.
# For the features, consider only petal length and petal width.
# 
# **Hint**: Use the `Query` package or do it manually via the `!insertcols` function.
# 
# ---
# ### Solution:

iris_reduced = deepcopy(iris)
filter!(row -> !(row.Species == "setosa"),  iris_reduced)
iris_reduced = iris_reduced[:,[3;4;5]]

insertcols!(iris_reduced, 3, :intercept => 1)
insertcols!(iris_reduced, 5, :label => iris_reduced.Species .== "virginica")


X = Matrix(iris_reduced[:, 1:3])
y = iris_reduced.label

@df iris_reduced scatter(
    :PetalLength,
    :PetalWidth;
    group = :Species,
    xlabel = "Petal length",
    ylabel = "Petal width",
    legend = :topleft,
)

# ## Training the classifier
# ### Exercise:
# Write a function log_reg which takes as an input the dataset, the labels and the initial 
# point. It should use Newton's method to find the optimal weights ``w``. Print the results 
# when started from zero.
# 
# It would be possible to use the code optim(f, g, x, s::Step) from the previous lecture
# and define only the step function s for the Newton's method. However, sometimes it may be 
# better to write simple functions separately instead of using more complex machinery.
# 
# ---
# ### Solution:

using Statistics

# sigmoid function
σ(z) = 1/(1+exp(-z))

function log_reg(X, y, w; max_iter=100, tol=1e-6)
    X_mult = [row*row' for row in eachrow(X)]
    for i in 1:max_iter
        # Finding y with ^
        y_hat = σ.(X*w)

        # Finding gradient
        grad = X'*(y_hat.-y) / size(X,1)

        # Finding second gradient (hessian)
        hess = y_hat.*(1 .-y_hat).*X_mult |> mean

        # Update w 
        w -= hess \ grad
    end
    return w
end

w = log_reg(X, y, zeros(size(X,2)))


# Function to separate values
separ(x::Real, w) = (-w[3]-w[1]*x)/w[2]

xs = extrema(iris_reduced.PetalLength) .+ [-0.1, 0.1]
ys = extrema(iris_reduced.PetalWidth) .+ [-0.1, 0.1]

scatter(
    iris_reduced.PetalLength,
    iris_reduced.PetalWidth;
    group = iris_reduced.Species,
    xlabel = "Petal length",
    ylabel = "Petal width",
    legend = :topleft,
    xs,
    ys,
)

plot!(xs, x -> separ(x,w); label = "Separation", line = (:black,3))

function check_corectness()
    correct = 0
    for i in 1:length(iris_reduced.PetalLength)
        if (iris_reduced.PetalWidth[i] > separ(iris_reduced.PetalLength[i],w) && iris_reduced.label[i]) ||
            (iris_reduced.PetalWidth[i] < separ(iris_reduced.PetalLength[i],w) && !iris_reduced.label[i])
                correct+=1
        end
    end

    println("Correct number of predictions: $correct")
    println("Wrong   number of predictions: ",length(iris_reduced.PetalLength)-correct)
end

check_corectness()
