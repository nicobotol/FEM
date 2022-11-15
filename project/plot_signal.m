vib = readmatrix('results.txt'); % import the data
delta_t = 0.001;
transient_iter_max = 8000;
time = 0:delta_t:delta_t*transient_iter_max - delta_t;

figure()
plot(time', vib)