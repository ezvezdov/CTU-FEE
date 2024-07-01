# include("/absolute/path/to/the/file/filename.jl") #including files

# Modules
module Points

using LinearAlgebra
export Point, distance

struct Point{T <: Real}
    x::T
    y::T
end

coordinates(p::Point) = (p.x, p.y)
Base.show(io::IO, p::Point) = print(io, coordinates(p))
distance(p::Point, q::Point) = norm(coordinates(q) .- coordinates(p), 2)

end

# end of Module Points

# from REPL
using .Points 
p = Point(4,2)  


# Test package in ImageInspector folder