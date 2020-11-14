function[q] = interp_lagrange(x, f, t)

%x = interpolation points
%f = function values corresponding to interpolation points
%t = data points 

n = length(x)-1; %total number of x
m = length(t); %total number of data points
L = ones(1,m) %Lagrange basis polynomials

q1 = zeros(1,m); %interpolation results numerator
q2 = zeros(1,m); %interpolation results denominator
q = zeros(1,m); %interpolation results final polynomial vector

dw = ones(1,n); %find the weighted values

%loop to evaluate weighted values
for j = 1:n
    for k = 1:n
        if j~=k
            dw(j) = dw(j)*1/((x(j)-x(k)));
        end 
    end
end

%consider the case where t coincides with x
exact = zeros(size(t));

%loop to determine the final polynomial vector using barycentric form
for i=1:n
    
    diff = t-x(i);
        %find w_i/(x-x_i)
        L = dw(i)./diff;
    
        %summation of numerator and denominator terms
        q1 = q1+f(i)*L; 
        q2 = q2 + L;
    %consider the case where t coincides with x(i)
    exact(diff==0) = 1;
end

%final result for function
q = q1./q2;

%consider case where t coincides with x
jj = find(exact);
q(jj) = q(exact(jj));

end
