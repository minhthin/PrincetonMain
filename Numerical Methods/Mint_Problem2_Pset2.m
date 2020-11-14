%Implement Strategy 1
m = 250;
t1 = linspace(0,1,m);
t2 = t1.^2;
t1 = t1';
ones = zeros(m,1);

for i = 1 : m
    ones(i) = 1;
end

A1 = [ones t1 t2'];
%find pseudoinverse of A_1
A1_pseudo = inv(A1'*A1)*A1';
%display condition number of A_1
disp(norm(A1)*norm(A1_pseudo));

%Implement Strategy 2
delta = 0.1;
ta = linspace(0, delta, m/2);
tb = linspace(1-delta, 1, m/2);
t3 = [ta  tb];
t4 = t3.^2;

A2 = [ones t3' t4'];
%find pseudoinverse of A_2
A2_pseudo = inv(A2'*A2)*A2';
%display condition number of A_2
disp(norm(A2)*norm(A2_pseudo));


%test with a sample theoretical x:
height = rand(1)*1000000;
velocity = 500*randn - 1000;
gravity = -9.81;
x = [height velocity gravity/2];
x = x';
norm_x = norm(x);

%strategy 1:
b1 = A1 * x; %image of A1
norm_b1 = norm(b1);
relerr_x1 = (norm(A1_pseudo))*(norm_b1)*(10.^(-3))/norm_x;
disp(relerr_x1);

%strategy 2:
b2 = A2 * x; %image of A2
norm_b2 = norm(b2);
relerr_x2 = (norm(A2_pseudo))*(norm_b2)*(10.^(-3))/norm_x;
disp(relerr_x2);
