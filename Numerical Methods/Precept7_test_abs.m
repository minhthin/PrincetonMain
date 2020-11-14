%set initial conditions for a, b, and t
a = -5;
b = 5;
t = linspace(a,b,501);

% establish the values of n to test
n_vector = [200];

%error vectors for each n
rel1_equi = zeros(1,4);
rel2_equi = zeros(1,4);
rel1_cheb = zeros(1,4);
rel2_cheb = zeros(1,4);

%establish function
f = @(x) 1./(1+x.^2);

%loop for each value of n in the n_vector
for i = 1:length(n_vector)
    
figure;
title('Lagrange Barycentric Interpolation: f(x) = (cos(x)+1)/2')
xlabel('x');
ylabel('f(x)');

%establish both equispace and chebyshev points
x_equi = linspace(a, b, n_vector(i)+1);
x_cheb = (a+b)/2 + (b-a)/2 * cos( (2*(0:n_vector(i)) + 1)*pi ./ (2*n_vector(i)+2) );

%matlab for equi
 plot(t,interp_lagrange_poly(x_equi, f(x_equi), t));
hold on;

%bary for equi
 plot(t,interp_lagrange(x_equi, f(x_equi), t));
 
 
 %matlab for cheb
 plot(t,interp_lagrange_poly(x_cheb, f(x_cheb), t));
 
 %bary for cheb
 plot(t,interp_lagrange(x_cheb, f(x_cheb), t));
 
 ylim([0 2]);
 %theoretical
 plot(t,f(t));
 
 legend({'Equally spaced - Matlab','Equally spaced - Barycentric','Chebyshev - Matlab','Chebyshev - Barycentric','|f(x)|'})
 hold off;
 
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
legend({'Equally spaced - Matlab','Equally spaced - Barycentric','Chebyshev - Matlab','Chebyshev - Barycentric'});
hold off;


%plot norm as function of n
func1 = zeros(1,4);
func2 = zeros(1,4);

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