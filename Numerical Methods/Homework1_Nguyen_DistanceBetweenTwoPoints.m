n = 2;
x = randn(n, 1);
x = x / norm(x) %x vector

v = randn(n, 1);
v = v - (x'*v)*x;
v = v / norm(v)%v vector

y = @(x,v,t) cos(t)*x + sin(t)*v; % define y vector

dist1 = @(x, y) real(acos(dot(x,y))); %define dist1
dist2 = @(x,y) 2 * real(asin(0.5 * sqrt(dot((x-y),(x-y))))); %define dist2

t = logspace(-20, pi, 1000); %create t vector

%define machine epsilon
e_mach = 1.11E-16;

%create g1 and g2 vectors 
g1 = [];
g2 = [];

bound = [];

for k = 1:1000   
g1(k) = abs(t(k) - dist1(x, y(x,v,t(k))))/t(k);
g2(k) = abs(t(k) - dist2(x, y(x,v,t(k))))/t(k);

bound(k) = e_mach * (1 + (2/abs(t(k) * tan(t(k)))));
end 

figure
loglog(t, g1, '.')
hold on
loglog(t, g2, '.')
loglog(t, bound, 'LineWidth', 2)
legend('g1', 'g2','Bound')
xlabel('t') 
ylabel('Relative Error') 
hold off
