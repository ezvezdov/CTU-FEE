using Statistics
x = rand(10);
mean(x)
var(x)
std(x)

using LinearAlgebra
A = [-4.0 -17.0; 2.0 2.0]
det(A)
inv(A)
norm(A)
eigvals(A)
eigvecs(A)
D = Diagonal([1,2,3])

using Random
# seed!(1234);
rand(2) # 2 element vector 
randperm(4) # random permutation of a given length.
v = [1,2,3,4]
shuffle(v)


using Plots

x = range(0, 2π; length = 100)
y = hcat(sin.(x), cos.(x))
plot(x, y;
    label = ["sine" "cosine"],
    xguide = "x",
    yguide = "y",
    legend = :bottomleft,
    title = "Trigonometric functions",
    xticks = (0:0.5π:2π, ["0", "0.5π", "π", "1.5π", "2π"]),
    color = [:red :blue],
    linestyle = [:dash :dot],
    linewidth = [2 4],
)

# Animations
n = 300
plt = plot(Float64[], [sin, cos];
    legend = false,
    xlims = (0, 6π),
    ylims = (-1.1, 1.1),
    linewidth = range(1, 20; length = n),
    color = palette(:viridis, n),
    axis = nothing,
    border = :none
)
anim = Animation()
for x in range(0, 6π; length = n)
    push!(plt, x, [sin(x), cos(x)])
    frame(anim)
end
gif(anim, "animsincos.gif", fps = 15)


# Dataframes
using DataFrames
df = DataFrame(A = 1:4, B = ["M", "F", "F", "M"], C = rand(4))

df[!, :A] # select rows

using CSV
CSV.write("dataframe.csv", df)
table = CSV.read("dataframe.csv", DataFrame; header = true)

df.D = [:a, :b, :c, :d]  # adding column

# insert columnn
insertcols!(df, 3, :B => rand(4), :B => 11:14; makeunique = true) 

# insert row
push!(df, [10, "F", 0.1, 15, 0.235, :f])

names(df) # return columns names as string
propertynames(df) # return columns names as keys

# rename columns
rename!(df, [:a, :b, :c, :d, :e, :f])
rename!(df, :a => :A, :f => :F) # specified columns


# RDatasets
using RDatasets, DataFrames
iris = dataset("datasets", "iris")
first(iris, 6)
iris[2:4, [:SepalLength, :Species]] # subset of dataframe
 
# DataFrames provides Not, Between, Cols and All selectors for more complex column selection
# iris[2:4, Not([:SepalLength, :Species])]








