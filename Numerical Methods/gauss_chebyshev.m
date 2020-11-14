%experiment Gauss quadrature with Chebyshev polynomials
%evalue integral from 0 to pi of e^(cos(x))

%input the number of integration points (n+1 total points) to output error
function[x] = gauss_chebyshev(n)

%establish weight:
w = pi/(n+1);
%initiate sum 
sum = 0;

for i = 1:n+1
    %find the Chebyshev nodes at x(i-1)
    x = cos(pi/(n+1)*(1/2+(i-1)));
    
    %update sum
    sum = sum + exp(x);
end
%experimental value
integral = w*sum;

%theoretical value:
theoretical = pi*besseli(0,1);

%relative error:
rel_err = (theoretical - integral)/(theoretical);
x = rel_err;

%print
%fprintf('%.16e\n',x);

end 