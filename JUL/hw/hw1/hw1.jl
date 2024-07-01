abstract type BracketingMethod end

struct Bisection <: BracketingMethod end
struct RegulaFalsi <: BracketingMethod end

midpoint(::Bisection, f, a, b) = (a+b)/2
midpoint(::RegulaFalsi, f, a, b) = (a * f(b) - b * f(a)) / (f(b) - f(a))

function findroot(
    method::BracketingMethod,
    f::Function,
    a::Real,
    b::Real;
    atol = 1e-8,
    maxiter = 1000
)
    if sign(f(a)) == sign(f(b))
        throw(DomainError("findroot($method,$f,$a,$b)","Function values in endpoints of interval have same sign."))
    end

    c = 0
    for _ in 0:maxiter
        a,b = b < a ? (b,a) : (a,b)
        c = midpoint(method,f,a,b)

        fa = f(a); fb = f(b); fc = f(c)
        (fa == 0) && return a
        (fb == 0) && return b
        (fc == 0) && return c

        a,b = sign(fa) == sign(fc) ? (c,b) : (a,c)

        (abs(f(c)) < atol) && return c
    end
    
    return c

end

# f(x)=x^3 - x - 2
# f2(x)=x^2

# findroot(Bisection.instance,f,1,2)
# findroot(RegulaFalsi.instance,f,1,2)
# # findroot(RegulaFalsi.instance,f,10,2)
# findroot(RegulaFalsi.instance,f2,0,2)
