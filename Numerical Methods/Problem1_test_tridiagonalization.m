% create symmetric A matrix
A = magic(8);
A = A + A';

%find eigenvalues of A
e = sort(eig(A));

% tridiagonalize matrix A given algorithm
T = tridiagonalization(A);

% print T
T

%find eigenvalues of new tridiagonal matrix
e1 = sort(eig(T));

%print the error of eigenvalues to 16 digits
fprintf('%.16e\n',e1-e);