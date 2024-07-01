using Pkg
using Plots, PyPlot
pyplot()

function ex1(x, y, c, R, N)
    A = zeros(length(y),length(x))
    for l in 1:length(x), k in 1:length(y)
        z = x[l] + y[k] * im
        for n in 0:N
            if abs(z) > (R^2 - R)
                A[k,l] = n/N
                break
            end
            z = z^2 + c
        end
    end
    return A
end

function juliaset(z, c, R, N)
    n = 0
    while n <= N && abs(z) <= R^2 - R
        n += 1
        z = z^2 + c
    end
    return n > N ? 0 : n/N
end

function ex2(x, y, c, R, N)
    A = zeros(length(y),length(x))
    for l in 1:length(x), k in 1:length(y)
        z = x[l] + y[k] * im
        A[k,l] = juliaset(z,c,R,N)
    end
    return A
end

x = [i for i in -1.5:(3/(1500-1)):1.5]
y = [i for i in -1:(2/(1000-1)):1]
# c = -0.4 + 0.61*im
# c = 0.285+0.01*im
# c = -0.835-0.2321*im
# c = -0.8+0.156*im
c = -0.70176+0.3842*im
R = 2
N = 1000

A = ex2(x,y,c,R,N)

# @show A


# Show plot
heatmap(A;
    c = :viridis,
    clims = (0, 0.15),
    cbar = :none,
    axis = :none,
    ticks = :none,
)

# Show anim and save gif

cs = 0.7885 .* exp.(range(π/2, 3π/2; length = 500) .* im)
anim = @animate for c in cs

    A = juliaset.(x' .+ y .* im, c, R, N)
    heatmap(A;
        c = :viridis,
        clims = (0, 0.15),
        cbar = :none,
        axis = :none,
        ticks = :none,
        size = (800, 600),
    )
end
gif(anim, "juliaset.gif", fps = 20) # save animation as a gif

# Alt + shift + enter