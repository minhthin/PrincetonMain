% Create algorithm to solve the solution to Lx = b from matrix A that is 
%tridiagonal

function [X] = solve_linear_LXB(A, B)
% use code to decompose A = LDL^T
n = size(A,1);
% determine size of matrix A and dimensions of X and B

[L,D] = symmetric_tridiagonal_LU(A);

%initialize X
X = zeros(n,1);

% set initial value for x1
X(1) = B(1);

%start recursion
for i = 2 : n
    %recursive function
    X(i) = B(i) - L(i,i-1)*X(i-1);
end

end