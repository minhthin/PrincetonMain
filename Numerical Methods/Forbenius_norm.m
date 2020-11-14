%Problem 3: QR factorization to calculate the Forbenius norm

function[w] = Forbenius_norm(A, B, C, D);

%assume full common rank
%determine the size of A, B, C, D
[m, r] = size(A);

n = 2*r
%decompose [B D]
matrix_1 = [B D];
%MGS#1
[Q1_1, R1_1] = modified_gram_schmidt(matrix_1);
%MGS#2
%matrix_1 = Q1R1_2R1_1
[Q1, R1_2] = modified_gram_schmidt(Q1_1);

%decompose [A -C]
matrix_2 = [A -C];
%MGS#1
[Q2_1, R2_1] = modified_gram_schmidt(matrix_2);
%MGS#2
%matrix_2 = Q2R2_2R2_1
[Q2, R2_2] = modified_gram_schmidt(Q2_1);

%matrix_1 = Q1R1
R1 = R1_2 * R1_1;

%matrix_2 = Q2R2
R2 = R2_2 * R2_2;

%find the matrix
w_matrix = R1*R2'*R2*R1';

%find the trace
trace_w = sum(diag(w_matrix));

%find the forbenius norm
w = sqrt(trace_w);

disp(w)

end