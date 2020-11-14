%Problem 3 Task 1
function[x, mu, k] = rqi(A, mu)

%define a tolerance for stopping criterion
tolerance = 10^(-16);

%determine the size of A, A is symmetric
[m,n] = size(A);

%find the vector x_0
%initialize starting vector 
x = randn(m,1);
mu_0 = mu;

%solve for the next x_1
M2 = A - mu_0*eye(m);
v = M2 \ x;

%update eigenvalue
mu_1 = mu_0 + v'*x/(v'*v);
%update eigenvector 
x = v/norm(v);

%initiate error
err = abs(mu_1 - mu_0);

%initialize the count 
k = 1;

%start loop
while((err > tolerance)) 
    
    %initialize loop
    mu_0 = mu_1;
    
    % RQI iteration
    B = A - mu_0*eye(m);
    y = B \ x;
    
    %update eigenvalue
    mu_1 = mu_0 + (y'*x)/(y'*y);
    
    %update eigenvector
    x = y/norm(y);
    
    %update count
    k = k+1;
    
    %update error 
    err = abs(mu_1 - mu_0);
end

%final values
mu = mu_1;
x = x;
k = k;

end