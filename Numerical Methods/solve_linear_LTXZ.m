%Solve L^Tx = z

function [X] = solve_linear_LTXZ(L, Z)

% use code to solve L^Tx = Z, given lower triangular matrix L from A
% decomposition
n = size(L,1);
% determine size of matrix L and dimensions of X and Z

L_T = L';
%take transpose

%set up solution
X = zeros(n,1);

%initialize x_n
X(n) = Z(n);

%recursively find X
for i = n-1 : -1: 1
    
    X(i) = Z(i) - L_T(i,i+1)*X(i+1);
    
end

end