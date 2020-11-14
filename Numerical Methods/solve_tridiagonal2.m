%Solve Ax = b Method 2

% Create algorithm to solve the solution to Ax = b from matrix A that is 
%tridiagonal

function [X] = solve_tridiagonal2(A, B)
% use code to decompose A = LDL^T
n = size(A,1);
% determine size of matrix A and dimensions of X and B


%1. Decompose A such that A = LDL'
[L,D] = symmetric_tridiagonal_LU(A);

%2. Find y such that Ly = b
%initialize Y
[Y] = solve_linear_LXB(A,B);

%3. Solve for Z: 
[Z] = solve_linear_DZY(D, Y);

%4. Solve for X:
[X] = solve_linear_LTXZ(L,Z);

end