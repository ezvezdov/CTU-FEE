abstract type Currency end

struct Euro <: Currency
    value::Float64
end

struct Dollar <: Currency
    value::Float64
end

struct Pound <: Currency
    value::Float64
end

struct BankAccount{C<:Currency}
    owner::String
    transaction::Vector{Currency}

    function BankAccount(owner::String, C::Type{<:Currency})
        return new{C}(owner, Currency[C(0)])
    end
end


# Specific symbol  for Euro
symbol(T::Type{<:Currency}) = string(nameof(T))
symbol(::Type{Euro}) = "€"
Base.show(io::IO, c::Currency) = print(io, c.value, " ", symbol(typeof(c)))

# Specific symbol  for Pounds
symbol(::Type{Pound}) = "£"
rate(::Type{Euro}, ::Type{Pound}) = 1.13

# Conversion 
dollar2euro(c::Dollar) = Euro(0.83 * c.value)
euro2dollar(c::Euro) = Dollar(c.value / 0.83)

rate(::Type{Euro}, ::Type{Dollar}) = 0.83
rate(T::Type{<:Currency}, ::Type{Euro}) = 1 / rate(Euro, T)

eur = convert(Euro, Dollar(1.3))


struct CzechCrown <: Currency
    value::Float64
end

symbol(::Type{CzechCrown}) = "Kč"
rate(::Type{Euro}, ::Type{CzechCrown}) = 0.038
Base.promote_rule(::Type{CzechCrown}, ::Type{Dollar}) = Dollar
Base.promote_rule(::Type{CzechCrown}, ::Type{Pound}) = Pound

# Add plus operation
Base.:+(x::Currency, y::Currency) = +(promote(x, y)...)
Base.:+(x::T, y::T) where {T <: Currency} = T(x.value + y.value)
Base.broadcastable(c::Currency) = Ref(c)

# Add minus operation
Base.:-(x::Currency, y::Currency) = +(promote(x, y)...)
Base.:-(x::T, y::T) where {T <: Currency} = T(x.value - y.value)

# Add multiplication (by number) operation
Base.:*(x::T{T <: Currency}, MULTer::T{T <: Real}) = T(x.value * MULTer) # Multiply curency by number
Base.:*(MULTer::T{T <: Real}, x::T{T <: Currency}) = T(x.value * MULTer) # Multiply number by curency

# Add division (by number) operation
Base.:/(x::T{T <: Currency}, DIVer::T{T <: Real}) = T(x.value / DIVer) # div curency by number
Base.:/(DIVer::T{T <: Real},x::T{T <: Currency}) = T(x.value / DIVer) # div number to curency

# To check conversion rate
Base.:/(x::Currency, y::Currency) = /(promote(x, y)...)
Base.:/(x::T, y::T) where {T <: Currency} = x.value / y.value

# Curency comparison 
Base.:(==)(x::Currency, y::Currency) = ==(promote(x, y)...)
Base.:(==)(x::T, y::T) where {T <: Currency} = ==(x.value, y.value)

Base.isless(x::Currency, y::Currency) = isless(promote(x, y)...)
Base.isless(x::T, y::T) where {T <: Currency} = isless(x.value, y.value)