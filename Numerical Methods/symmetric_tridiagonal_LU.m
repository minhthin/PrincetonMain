% Create algorithm to construct L and D such that A = LDL^T where
% A is tridiagonal and all leading values of the diagonal of A
% are nonzero.

function [L,D] = symmetric_tridiagonal_LU(A)
%function to compute LU factorization

%determine the size of matrix A (rows and columns)
n = size(A,1);

D = zeros(n,n);
%Set the diagonals of D to be the diagonals of A
for i = 1 : n
    D(i,i) = A(i,i);
end

L = eye(n); %identity matrix

%determine the diagonal entries for each iteration
for k = 2 : n
    
    %check to see if the leading entry is nonzero
    if D(k-1,k-1) == 0 
        break;
    end
    
    %update L
    L(k,k-1) = A(k-1,k) / D(k-1,k-1);
    
    %update D
    D(k,k) = D(k,k) - A(k-1,k) * L(k,k-1);
    
end

end