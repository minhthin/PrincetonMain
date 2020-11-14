%test experiment
err = 1; %set random initial error value
i  = 0; % number of integration points - 1

%iterate until error gets close to machine precision
while err > 10^(-15)
    
    i = i+1;
    err = gauss_chebyshev(i);
    
    %print error and number of integration points
    fprintf('%d\n',i+1);
    fprintf('%.16e\n',err);
    fprintf('\n');
end
