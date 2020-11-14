function q = interp_lagrange_poly(x, f, t)
n = length(x)-1;
p = polyfit(x, f, n);
q = polyval(p, t);
end