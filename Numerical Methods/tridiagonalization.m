function[T] = tridiagonalization(A)

%find the size of A
n = size(A,1);

%loop over the iteration from Householder algorithm
for k = 1:(n-2)
    
    %create vector x, extracted from A_k-1
    x = A(k+1:n,k);
    
    %Built the H matrix
    v = sign(x(1))*norm(x,2)*eye(size(x)) + x;
    %normalize
    v = v/norm(v,2);
    
    %computationally expensive matrix multiplication
%     H = eye(n-k) - 2*v*v';
%     Q = eye(n);
%     Q(k+1:n,k+1:n) = H;
%     B = Q'*B*Q;

%Q'A
A(k+1:n,k:n) = A(k+1:n,k:n) - 2* v*(v'*A(k+1:n,k:n));
A(k+2:n,k) = 0; %make exactly 0

%Q'AQ
A(k:n,k+1:n) = A(k:n,k+1:n) - 2 * (A(k:n,k+1:n)*v)*v';
A(k, k+2:n) = 0; %make exactly 0

end 

%update final matrix after tridiagonalization
T = A;
end 