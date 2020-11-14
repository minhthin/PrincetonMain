%Implement Newton's Method for given function with an increment stopping
%criteria
f = @(x) exp(cos(x))/(2+x) - 1; %equation definition
df = @(x) -exp(cos(x))*sin(x)/(2+x) - exp(cos(x))/(2+x)^2; %first order 
%derivative of f
x0 = 0; %initial guess
% choose a few integers, and can converge to root

N = 50; %Maximum number of iterations %stopping criteria #1
tolerance = 1E-16; %Convergence tolerance %stopping criteria #2

x = x0; %set initial integer
fprintf('x = %+.16e, \t f(x) = %+.16e\n',x,f(x));

x_before = x0;

root = 0; %define root

%Iterate until convergence
%Newton's Method algorithm
for k = 1 : N %stopping criteria 1
    
    x = x - f(x) / df(x); %simple iteration
    fprintf('x = %+.16e, \t f(x) = %+.16e\n',x,f(x));
    
    error1 = f(x); %stopping criteria 2: if the error is less than tolerance
    if (abs(error1) <= tolerance)
        break;
    end
    
    error2 = abs(x - x_before)/abs(x); %stopping criteria 3: if the relative
    %error is less than the tolerance
    if(error2 <= tolerance)
        break;
    end
    x_before = x; %update x_before
    
end

root = x; %final root obtained
fprintf('root = %+.16e',root);

