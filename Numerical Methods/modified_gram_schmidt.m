function [Q, R] = modified_gram_schmidt(A)
% Modified Gram-Schmidt orthogonalization algorithm (better stability than CGS)

    [m, n] = size(A);
    Q = zeros(m, n);
    R = zeros(n, n);
    
    for j = 1 : n
        
        v = A(:, j);
        
        R(j, j) = norm(v);
        Q(:, j) = v / R(j, j);
        
        R(j, (j+1):n) = Q(:, j)' * A(:, (j+1):n);
        A(:, (j+1):n) = A(:, (j+1):n) - Q(:, j) * R(j, (j+1):n);
        
    end

end
