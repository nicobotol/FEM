vib = readmatrix('results.txt'); % import the data
delta_t = 1.2E-6;
transient_iter_max = 50000;
time = 0:delta_t:delta_t*transient_iter_max - delta_t;

figure()
plot(time', vib)