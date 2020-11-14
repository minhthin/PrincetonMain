%find the eigenvalues of A
n = 10;
A = randn(n);
A = A + A';
b = norm(A,1); %eigenvalues lie between [-b,b]
eig_min = -b;
eig_max = b;

%eigenpair unpredictable
%Initialize based on uniform distribution 

%transform A into tridiagonal matrix
[P,H] = hess(A);

%count the total number of eigenvalues obtained, should get 10

%sample the eigenvalues
m = 1000;
sample = eig_min + rand(1,m)*(eig_max - eig_min);

%record eigenvalues and iterations
eigen_values = zeros(1,m);
iterations = zeros(1,m);

%loop for each initialization
for i = 1:m
    
    %apply rqi
    [x, mu, k] = rqi(H,sample(i));
    
    %record iterations and eigenvalues obtained
    iterations(i) = k;
    eigen_values(i) = mu;
end

%establish tolerance to find distinct eigenvalues
tolerance = 10^(-8);

%obtain distinct eigenvalues
eigenvalues = uniquetol(eigen_values, tolerance)

%plot histogram of iterations
histogram(iterations)
%plot details
title('Histogram of iterations for RQI')
xlabel('iterations')
ylabel('frequency')

%theoretical eigenvalues of A using MATLAB
e = eig(A);

%difference between values
diff = eigenvalues - e'