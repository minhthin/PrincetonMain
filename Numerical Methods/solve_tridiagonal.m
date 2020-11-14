% Create algorithm to solve the solution to Ax = b from matrix A that is 
%tridiagonal

function [X] = solve_tridiagonal(A, B)
% use code to decompose A = LDL^T
n = size(A,1);
% determine size of matrix A and dimensions of X and B


%1. Decompose A such that A = LDL'
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


%2. Find y such that Ly = b
%initialize Y
Y = zeros(n,1);

% set initial value for x1
Y(1) = B(1);

%start recursion
for i = 2 : n
    %recursive function
    Y(i) = B(i) - L(i,i-1)*Y(i-1);
end


%3. Solve for X:
%initialize X
X = zeros(n, 1);

% set initial value for x1
X(n) = Y(n)/D(n,n);

%start recursion
for i = n-1:-1:1
    
    %recursive function
    X(i) = ((Y(i) - L(i+1,i)*D(i+1,i+1)*X(i+1)))/D(i,i);
end

end