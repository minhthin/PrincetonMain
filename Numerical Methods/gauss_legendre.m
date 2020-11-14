function[x, w] = gauss_legendre(n)

%determine the three-term recurrence coefficients for the system of
%polynomials 
a = zeros(n+1,1);
b = zeros(n+1,1); 

%for legendre polynomials of weight w(x) = 1 over interval from -1 to 1, we
%have:
%a_k = 0;
%b_k^2 = k^2/(4k^2-1)

%construct b
for i = 2:(n+1)
    b(i) = sqrt((i-1)^2/(4*(i-1)^2 - 1));
end

%define t be the integral of weight over -1 and 1
t = 2;

%construct the symmetric, tridiagonal matrix J_n+1
J = zeros(n+1,n+1);

J(1,1) = a(1);
J(1,2) = b(2);

for j = 2:n
    J(j,j-1) = b(j);
    J(j,j) = a(j);
    J(j,j+1) = b(j+1);
end

J(n+1,n) = b(n+1);
J(n+1,n+1) = a(n+1);

%compute the eigenvalues and eigenvectors of J_n+1
[V D] = eig(J);

%find the nodes 
x = zeros(1,n+1);

%find the weights
w = zeros(1,n+1);

for k = 1:n+1
    %nodes are eigenvalues of J
    x(k) = D(k,k);
    
    %weights
    v = V(:,k)./norm(V(:,k));
    w(k) = v(1)^2*t;
end

end
