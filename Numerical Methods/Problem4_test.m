%test vandermonde

n_sample = [1 3 5 11 31 51 101 201];

m = size(n_sample);

for i = 1:m(2)
    vandermonde(n_sample(i));
end