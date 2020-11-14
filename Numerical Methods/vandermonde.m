%Implement Vandermonde Algorithm 
function[] = vandermonde(n)

%establish x points
x = linspace(0, 2*pi, n+1);
x = x(1:end-1);
x = x';

%create function f
f = @(x) exp(1./(1.3 + cos(x));

%y vector
y = f(x);

%create vandermonde matrix
A = zeros(n,n);

%loop to create A
for i = 0:n-1
    if (mod(i,2)==0)
        A(:,i+1) = cos((i/2)*x);
    else
        A(:,i+1) = sin(((i+1)/2)*x);
    end
end

%c = D^(-1)A^Ty
%compute D^(-1)A^T
A = A';
A(1,:) = A(1,:)/n;
for k = 2:n
        A(k,:) = A(k,:) * (2/n);
end

%compute c
c = A*y;

%fine grid to evalue function f and pn
n1 = 1001;
t = linspace(0, 2*pi, n1);
t = t';

%find approximation
p = zeros(n1,1);

%create polynomial
for j = 0:n-1
    if (mod(j,2)==0)
        p = p + c(j+1)*cos((j/2)*t);
    else
        p = p + c(j+1)*sin(((j+1)/2)*t);
    end
end

%plot
figure 
plot(t,f(t));
hold on;
plot(t,p);

title('Vandermonde Periodic Approximation');
xlabel('x');
ylabel('f(x)');
legend('f(x)', 'p_n(x)');
hold off;

%abolute error
err = abs(f(t) - p);
fprintf('%.16e\n',max(err))
end