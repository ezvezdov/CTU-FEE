struct Rectangle
    bottomleft::Vector{Float64}
    width
    height
end

r = Rectangle([1,2], 3, 4)
Rectangle([1.0, 2.0], 3, 4)

methods(Rectangle) # available methods for current type

abstract type AbstractPoint{T} end

struct Point3D{T <: Real} <: AbstractPoint{T}
    x::T
    y::T
    z::T
end

coordinates(p::Point3D) = (p.x, p.y)

# Like a python classes
struct OrderedPair{T <: Real}
    x::T
    y::T

    function OrderedPair(x::Real, y::Real)
        x > y && error("the first argument must be less than or equal to the second one")
        xp, yp = promote(x, y)
        return new{typeof(xp)}(xp, yp)
    end
end


# Exercise 
struct PointND{N,T <: Real} <: AbstractPoint{T}
    x::NTuple{N,T}

    function PointND(args::Real...)
        values = promote(args...)
        return new{length(values),eltype(values)}(values)
    end
end

coordinates(p::PointND) = p.x
dim(p::PointND) = N

# Exercise
struct Gauss{T<:Real}
    mu::T = 0
    sigma::T = 0
    function Gauss()
        sigma^2 > 0 && error("sigma must be positive")
        pars = promote(mu, sigma)
        return new{eltype(pars)}(pars...)
    end
end

(g::Gauss)(x::T) = exp(((x-g.mu)/g.sigma)^2 * (-1/2)) / (g.sigma * sqrt(2* pi))
