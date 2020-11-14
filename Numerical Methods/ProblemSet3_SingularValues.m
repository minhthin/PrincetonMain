%Problem Set 3 Problem 4 
%Implement two methods for computing singular values of M

n1 = 4;
n2 = 8;
n3 = 16;

M1 = hilb(n1);
M2 = hilb(n2);
M3 = hilb(n3);

%Method 1
eig1 = eig(M1'*M1);
sing1_M = sort(sqrt(eig1));

eig2 = eig(M2'*M2);
sing2_M = sort(sqrt(eig2));

eig3 = eig(M3'*M3);
sing3_M = sort(sqrt(eig3));

%Method2
H1 = zeros(2*n1,2*n1);
H1(1:n1,n1+1:2*n1) = M1';
H1(n1+1:2*n1,1:n1) = M1;
new_eig1 = sort(eig(H1));
new_sing1_M = sort((new_eig1));
new_sing1_M = new_sing1_M(n1+1:2*n1);

H2 = zeros(2*n2,2*n2);
H2(1:n2,n2+1:2*n2) = M2';
H2(n2+1:2*n2,1:n2) = M2;
new_eig2 = sort(eig(H2));
new_sing2_M = sort((new_eig2));
new_sing2_M = new_sing2_M(n2+1:2*n2);

H3 = zeros(2*n3,2*n3);
H3(1:n3,n3+1:2*n3) = M3';
H3(n3+1:2*n3,1:n3) = M3;
new_eig3 = sort(eig(H3));
new_sing3_M = sort((new_eig3));
new_sing3_M = new_sing3_M(n3+1:2*n3);

%Theoretical computation
s1 = sort(svd(M1));
s2 = sort(svd(M2));
s3 = sort(svd(M3));

%absolute errors
method1_abs1 = abs(sing1_M - s1);
fprintf('%.16e\n',max(method1_abs1));

method1_abs2 = abs(sing2_M - s2);
fprintf('%.16e\n',max(method1_abs2));

method1_abs3 = abs(sing3_M - s3);
fprintf('%.16e\n',max(method1_abs3));

method2_abs1 = abs(new_sing1_M - s1);
fprintf('%.16e\n',max(method2_abs1));

method2_abs2 = abs(new_sing2_M - s2);
fprintf('%.16e\n',max(method2_abs2));

method2_abs3 = abs(new_sing3_M - s3);
fprintf('%.16e\n',max(method2_abs3));

%relative errors
method1_rel1 = method1_abs1./s1;
fprintf('%.16e\n',max(method1_rel1));

method1_rel2 = abs(sing2_M - s2)./s2;
fprintf('%.16e\n',max(method1_rel2));

method1_rel3 = abs(sing3_M - s3)./s3;
fprintf('%.16e\n',max(method1_rel3));

method2_rel1 = abs(new_sing1_M - s1)./s1;
fprintf('%.16e\n',max(method2_rel1));

method2_rel2 = abs(new_sing2_M - s2)./s2;
fprintf('%.16e\n',max(method2_rel2));

method2_rel3 = abs(new_sing3_M - s3)./s3;
fprintf('%.16e\n',max(method2_rel3));