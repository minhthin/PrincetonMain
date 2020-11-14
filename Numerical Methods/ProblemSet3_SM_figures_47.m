%draw the sets of f1 and f2 from the Suli and Mayers exercises
figure
%Suli and Mayers 4.7
%define f
f = @(x) [x(1)^2 + x(2)^2 -2; x(1) - x(2)];
%define partials
df1 = @(x1) [2*x1 ; 1];
df2 = @(x2) [2*x2 ; -1];

%define solution points
x_0 = [1.0; 1.0];
x_1 = [-1.0, -1.0];

%plot f1 = 0 and f2 = 0
figure;
x1 = linspace(-2,2,401);
x2 = linspace(-2,2,401);
[X1, X2] = meshgrid(x1, x2);

%plot f1 = 0
f1 = X1.^2 + X2.^2 -2;
contour(x1,x2,f1,[0,0],'r');

%plot details
hold on;
xlim([-4 4])
ylim([-4 4])
xlabel('x1')
ylabel('x2')
title('Suli and Mayers 4.7')

%plot f2 = 0
f2 = X1 - X2;
hold on;
contour(x1, x2, f2, [0,0],'b')

%graph the gradients of f1 and f2
quiver(x_0(1),x_0(2), 2*x_0(1), 1,'g--');
quiver(x_0(1),x_0(2), 2*x_0(2), -1,'g--');
quiver(x_1(1),x_1(2), 2*x_1(1), 1,'g--');
quiver(x_1(1),x_1(2), 2*x_1(2), -1,'g--');

legend('f1','f2', 'df1_0', 'df2_0', 'df1_1', 'df2_1')
