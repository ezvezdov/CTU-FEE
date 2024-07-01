using Plots

f(x) = sin(x[1] + x[2]) + cos(x[1])^2
f(x1,x2) = f([x1;x2])

# Derivation of f. We can compute gradient.
g(x) = [cos(x[1] + x[2]) - 2*cos(x[1])*sin(x[1]); cos(x[1] + x[2])]


# Visualize function without gradient vector
xs = range(-3, 1, length = 40)
ys = range(-2, 1, length = 40)
display(contourf(xs, ys, f, color = :jet))

# Computes the approximation of f'(x) by finite differences. E.g. compute gradient
finite_difference(f,x::Real; h=1e-8) = (f(x+h) - f(x)) / h


# Finite difference approximation  with different h
##############################################################
x = [-2; -1]
fin_diff(h) = finite_difference(y -> f(x[1], y), x[2]; h=h)
true_grad = g(x)[2]
hs = 10. .^ (-15:0.01:-1)
plot(hs, fin_diff,
    xlabel = "h",
    ylabel = "Partial gradient wrt y",
    label = ["Approximation" "True gradient"],
    xscale = :log10,
)

hline!([true_grad]; label =  "True gradient")
##############################################################


# Save plot of function
xs = range(-3, 1, length = 20)
ys = range(-2, 1, length = 20)
plt = contourf(xs, ys, f;
    xlims = (minimum(xs), maximum(xs)),
    ylims = (minimum(ys), maximum(ys)),
    color = :jet
)

# Visualize function with its gradients
α = 0.25
for x1 in xs, x2 in ys
    x = [x1; x2]
    x_grad = [x x.+α.*g(x)]

    plot!(x_grad[1, :], x_grad[2, :];
        line = (:arrow, 2, :black),
        label = "",
    )
end
display(plt)

# Unconstrained optimization (неограниченная)
##########################################################################################################

# Function which goes x to minimum ( fixed stepsize ) 
function optim(f, g, x, α; max_iter=100)
    # Each iteration is step
    xs = zeros(length(x), max_iter+1)
    xs[:,1] = x

    # Generate current gradient
    for i in 1:max_iter
        # Gradient shows direction to the maximum of function
        # We need to go to the opposite direction (to minimum)
        x -= α*g(x)
        xs[:,i+1] = x
    end
    return xs
end

# Create animation
##############################################################
using Random
function create_anim(
    f,
    path,
    xlims,
    ylims,
    file_name = joinpath(pwd(), randstring(12) * ".gif");
    xbounds = xlims,
    ybounds = ylims,
    fps = 15,
)
    xs = range(xlims...; length = 100)
    ys = range(ylims...; length = 100)
    plt = contourf(xs, ys, f; color = :jet)

    # add constraints if provided
    if !(xbounds == xlims && ybounds == ylims)
        x_rect = [xbounds[1]; xbounds[2]; xbounds[2]; xbounds[1]; xbounds[1]]
        y_rect = [ybounds[1]; ybounds[1]; ybounds[2]; ybounds[2]; ybounds[1]]

        plot!(x_rect, y_rect; line = (2, :dash, :red), label="")
    end

    # add an empty plot
    plot!(Float64[], Float64[]; line = (4, :arrow, :black), label = "")

    # extract the last plot series
    plt_path = plt.series_list[end]

    # create the animation and save it
    anim = Animation()
    for x in eachcol(path)
        push!(plt_path, x[1], x[2]) # add a new point
        frame(anim)
    end
    gif(anim, file_name; fps = fps, show_msg = false)
    return nothing
end
##############################################################



xlims = (-3, 1)
ylims = (-2, 1)

# step α = 0.1
x_gd = optim([], g, [0; -1], 0.1)
#create_anim(f, x_gd, xlims, ylims, "unconstrained_fixed_stepsize_1.gif")

# step α = 0.01
x_gd = optim([], g, [0; -1], 0.01)
#create_anim(f, x_gd, xlims, ylims, "unconstrained_fixed_stepsize_2.gif")

# step α = 1
x_gd = optim([], g, [0; -1], 1)
#create_anim(f, x_gd, xlims, ylims, "unconstrained_fixed_stepsize_3.gif")



# Structure with step (convenient to change step α)
abstract type Step end
struct GD <: Step
    α::Float64
end



# Same funtion as function optim() above, but allows us change step
optim_step(s::GD, f, g, x) = -s.α*g(x)
function optim(f, g, x, s::Step; max_iter=100)
    xs = zeros(length(x), max_iter+1)
    xs[:,1] = x
    for i in 1:max_iter
        x += optim_step(s, f, g, x)
        xs[:,i+1] = x
    end
    return xs
end

gd = GD(0.1)
x_opt = optim(f, g, [0;-1], gd)


# One of the possibilities of adaptive stepsize is the Armijo condition which looks 
# for α[k] (α in k iteration) satisfying: 
# f(x[k] - α[k] * gradf(x[k])) <= f(x[k]) - cα[k] * ||gradf(x[k])||^2
# where c is a small constant. Usually c = 10^-4.
# This method prevents oscillations.
# to find some α satisfying the Armijo conditions, we start with some α_max 
# and divide it by two until the condition is satisfied.
struct Armijo <: Step
    c::Float64
    α_max::Float64
end

function optim_step(s::Armijo, f, g, x,)
    α = s.α_max
    fun = f(x)
    gradf = g(x)
    while !( f(x - α * gradf) <= fun - s.c * α *(gradf'*gradf))
        α/=2
        if α <= 1e-6
            warning("Armijo line search failed.")
            break
        end
    end
    return -α*g(x)
end

armijo = Armijo(1e-4,1)
x_opt = optim(f, g, [0;-1], armijo)

#create_anim(f, x_opt, xlims, ylims, "armijo_stepsize.gif")

# Constrained optimization (ограниченная)
##########################################################################################################

# The implementation of projected gradients is the same as gradient descent 
# but it needs projection function P as input. For reasons of plotting, it returns both x and y.
function optim(f, g, P, x, α; max_iter=100)
    xs = zeros(length(x), max_iter+1)
    ys = zeros(length(x), max_iter)
    xs[:,1] = x
    for i in 1:max_iter
        ys[:,i] = xs[:,i] - α*g(xs[:,i])
        xs[:,i+1] = P(ys[:,i])
    end
    return xs, ys
end

P(x, x_min, x_max) = min.(max.(x, x_min), x_max)

x_min = [-1; -1]
x_max = [0; 0]

xs, ys = optim(f, g, x -> P(x,x_min,x_max), [0;-1], 0.1)

xlims = (-3, 1)
ylims = (-2, 1)

# create_anim(f, xs, xlims, ylims, "constrained_projection.gif";
#     xbounds=(x_min[1], x_max[1]),
#     ybounds=(x_min[2], x_max[2]),
# )

# Plot the path
xys = hcat(reshape([xs[:,1:end-1]; ys][:], 2, :), xs[:,end])
# create_anim(f, xys, xlims, ylims, "constrained_projection_path.gif";
#     xbounds=(x_min[1], x_max[1]),
#     ybounds=(x_min[2], x_max[2]),
# )