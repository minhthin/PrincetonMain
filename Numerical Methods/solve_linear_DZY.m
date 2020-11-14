%Solve Dz = y

function [X] = solve_linear_DZY(D, Y)

% use code to solve Dz = Y, given D and Y
n = size(D,1);
% determine size of matrix D and dimensions of Y and Z

X = zeros(n,1);

for i = 1 : n
    
    X(i) = Y(i)/D(i,i);
end

end