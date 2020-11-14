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

%plot norm as function of n
func1 = zeros(1,length(n_vector));
func2 = zeros(1,length(n_vector));

for i = 1:length(n_vector)
    p1 = polyfit(x_equi, f(x_equi), n_vector(i));
    func1(i) = norm(p1,inf);
    
    p2 = polyfit(x_cheb, f(x_cheb), n_vector(i));
    func2(i) = norm(p2,inf);
end
figure
semilogy(n_vector,func1);
hold on;
semilogy(n_vector,func2);

title('Polyfit norm')
xlabel('x');
ylabel('f(x)');
legend({'Equally spaced','Chebyshev'});

hold off;