%Test Ax = b tridiagonal implementation
A = [12 1 0 0 0 0; 1 18 2 0 0 0; 0 2 4 -5 0 0; 0 0 -5 8 -2 0; 0 0 0 -2 6 2; 0 0 0 0 2 16];
B = [1;2;3;4;5;6];

%Find L and U:
[L,D] = symmetric_tridiagonal_LU(A);
%print L and D
disp(L) 
disp(D)

%print norm
product = L*D*L';
difference = A - product;
Norm_difference = norm(difference);
fprintf('%.16e\n',Norm_difference)

%Solve Ax = b
[X] = solve_tridiagonal(A, B); %method 1
[X1] = solve_tridiagonal2(A, B); %method 2

%print X
fprintf('%.16e\n',X) %method 1
fprintf('%.16e\n',X1) %method 2

%Compare to A\b
disp(A\B);