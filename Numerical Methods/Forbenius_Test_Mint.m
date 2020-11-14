%Test forbenius norm algorithm

%set matrix dimensions:
m = 10.^6;
r = 10.^2;

%generate four matrices A, B, C, D
A = randn(m,r);
D = randn(m,r);

%find the forbenius norm of A
disp(norm(A,'fro'));

%find the QR decomposition of A
[Q1,R1] = modified_gram_schmidt(A);
[Q,R] = modified_gram_schmidt(Q1);

%Q^TQ = I;
X = Q;

%define B and C
B = D + X;
C = A;

%calculate the forbenius norm using our algorithm
w_mint = Forbenius_norm(A, B, C, D);