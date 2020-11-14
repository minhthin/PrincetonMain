%set initial conditions for a, b, and t
a = -1;
b = 1;
t = linspace(a,b,501);

% establish the values of n to test
n_vector = linspace(1,201,100);

%error vectors for each n
rel1_equi = zeros(1,length(n_vector));
rel2_equi = zeros(1,length(n_vector));
rel1_cheb = zeros(1,length(n_vector));
rel2_cheb = zeros(1,length(n_vector));

%establish function
f = @(x) abs(x);

%loop for each value of n in the n_vector
for i = 1:length(n_vector)

%establish both equispace and chebyshev points
x_equi = linspace(a, b, n_vector(i)+1);
x_cheb = (a+b)/2 + (b-a)/2 * cos( (2*(0:n_vector(i)) + 1)*pi ./ (2*n_vector(i)+2) );

 %relative error values
 rel1_equi(i) = max(abs(((interp_lagrange_poly(x_equi, f(x_equi), t)) - f(t))))/max(abs(f(t)));
 rel2_equi(i) = max(abs(((interp_lagrange(x_equi, f(x_equi), t)) - f(t))))/max(abs(f(t)));
 
 rel1_cheb(i) = max(abs(((interp_lagrange_poly(x_cheb, f(x_cheb), t)) - f(t))))/max(abs(f(t)));
 rel2_cheb(i) = max(abs(((interp_lagrange(x_cheb, f(x_cheb), t)) - f(t))))/max(abs(f(t)));
 
end

 %plot relative error
figure
semilogy(n_vector,rel1_equi);
hold on;
plot(n_vector,rel2_equi);
plot(n_vector,rel1_cheb);
plot(n_vector,rel2_cheb);
title('Lagrange Barycentric Interpolation: f(x) = abs(x) - Relative Error')
xlabel('n');
ylabel('error');
legend({'Equally spaced - Matlab','Equally spaced - Barycentric','Chebyshev - Matlab','Chebyshev - Barycentric'},'Location','northwest');
hold off;