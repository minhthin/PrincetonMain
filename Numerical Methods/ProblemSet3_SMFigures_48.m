%draw the sets of f1 and f2 from the Suli and Mayers exercises
figure
%Suli and Mayers 4.8
%define f
f = @(x) [x(1)^2 + x(2)^2 -2; x(1) + x(2)-2];

%solution point
x_0 = [1.0; 1.0];

%partials
df1 = @(x1) [2*x1 ; 1];
df2 = @(x2) [2*x2 ; 1];

%plot f1 = 0 and f2 = 0
x1 = linspace(-2,2,401);
x2 = linspace(-2,2,401);
[X1, X2] = meshgrid(x1, x2);

%f1 =0
f1 = X1.^2 + X2.^2 -2;
contour(x1,x2,f1,[0,0],'r');
hold on;

%f2 =0
f2 = X1 + X2-2;
hold on;
contour(x1, x2, f2, [0,0],'b')

%plot gradients
quiver(x_0(1),x_0(2), 2*x_0(1), 1,'m--');
quiver(x_0(1),x_0(2), 2*x_0(2), 1,'m--');

%plot details
hold on;
xlim([-4 4])
ylim([-4 4])
xlabel('x1')
ylabel('x2')
title('Suli and Mayers 4.8')
legend('f1','f2', 'df1_0', 'df2_0')
