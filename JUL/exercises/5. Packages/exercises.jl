using Plots


fx(t) = cos(3t)
fy(t) = sin(2t)
n = 100
t = range(0, 2π; length = 100)
linewidth = vcat(range(1, 50; length=n÷2),
    range(50, 1; length = n-n÷2))
# color = vcat(
#     collect(palette(:viridis, n ÷ 2)),
#     collect(palette(:viridis, n - n ÷ 2; rev = true)))
# plot(
#     fx.(t), fy.(t);
#     linewidth,
#     # color,
#     label = ["cos(3x)" "sin(2x)"],
#     axis = nothing,
#     lims = (-1.2, 1.2)
# )

###################################################################

a = 4.23
b = 2.35
length = 200
fx(t) = (a+b)*cos(t) - b * cos((a/b + 1 ) * t)
fy(t) = (a+b)*sin(t) - b * sin((a/b + 1 ) * t)
t = range(-15,20;length=length)

# plot(
#     fx.(t),
#     fy.(t);
#     linewidth=vcat(range(5,10;length=length÷2),range(10,5;length=length÷2)),
#     color=palette(:viridis, length)
# )

f(x,y) = (x^2 * y^2) / (x^4 + y^4)

#######################################################################################

x = range(-5,5;length=200)
# plot(
#     x,x,f;
#     seriestype = :contourf,
#     color = :viridis,
#     legend = false,
#     cbar = false,
#     axis = nothing,
#     border = :none
# )
# equals:
contourf(x, x, f;
    color = :viridis,
    legend = false,
    cbar = false,
    axis = nothing,
    border = :none
)

heatmap(x, x, f; color = :viridis,
    legend = false,
    cbar = false,
    axis = nothing,
    border = :none
)

surface(x, x, f;
    camera = (25, 65),
    color = :viridis,
    legend = false,
    cbar = false,
    axis = nothing,
    border = :none
)

# ProgressMeter
using ProgressMeter
@showprogress 1 "Computing..." for i in 1:50
    sleep(0.1)
end

# BenchmarkTools.jl
using BenchmarkTools
@benchmark sin(x) setup=(x=rand())