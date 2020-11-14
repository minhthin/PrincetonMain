%test gauss_legendre

%test for values of n = 1,2,3,4,5
n_vector = [1 2 3 4 5];

for i = 1:length(n_vector)
    
    [x, w] = gauss_legendre(n_vector(i));
    
    %print nodes
    fprintf('%.16e\n',x);
    fprintf('\n');
    %print weights
    fprintf('%.16e\n',w);
    fprintf('\n\n');
end


%should have degree of precision of 2n+1
%for the n = 5 case, test integration of monomials

%create function
n_test = 5;
[x_test, w_test] = gauss_legendre(n_test);

%determine max degree
max = 13; %up to degree 12 
%determine max integral value
exact = zeros(max,1);
for j = 1:max
    exact(j) = (1/j)*((1)^j - (-1)^j);
end

%compute integral
%create cell of functions of monomials
integral_values = zeros(max,1);
p = cell(max,1);
for i = 1:max
    %create monomonials and store in p
    f = @(x) x^(i-1);
    p{i} = f;
end

%evaluate approximation of integral for monomials up to maximum degree
for j = 1:max
    approx = zeros(n_test+1,1);
    
    for k = 1:n_test+1
       approx(k) = p{j}(x_test(k));
    end
    
    %store approximation value for each monomial in matrix
    integral_values(j) = sum(approx.*w_test');
end

%determine difference between integral and approximation using quadrature
%rule
diff = integral_values - exact;
fprintf('%.16e\n',diff);

