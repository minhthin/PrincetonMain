%Implement Secant Method to find the roots of the function
f = @(x) exp(cos(x))/(2+x) - 1;  %No need for the derivative of f

%Initialize initial two points
x0 = 0.0;
x1 = 1.0;

%root
root;

%Find f at the initial points
f0 = f(x0);
f1 = f(x1);

fprintf('x = %+.16e, \t f(x) = %+.16e\n', x1, f1);

N = 50; %Maximum number of iterations %stopping criteria #1
tolerance = 1E-16; %Convergence tolerance %stopping criteria #2

%Iterate until convergence
for k = 1 : N
    
    error1 = abs(x1 - x0)/abs(x0); %stopping criteria: if the error is less than tolerance
    if (error1 <= tolerance)
        break;
    end
    
    error2 = f(x1);
    if (abs(error2) <= tolerance)
        break;
    end
    
    %Compute the next iterate
    x2 = x1 - f1 * (x1 - x0) / (f1 - f0);
    
    %Evaluate the function there with a call to f
    f2 = f(x2);
    
    fprintf('x = %+.16e, \t f(x) = %+.16e\n', x2, f2);
    
    x0 = x1; %update the x and f
    f0 = f1;
    x1 = x2;
    f1 = f2;
    
end

root = x; %final root obtained
fprintf('root = %+.16e',root);
