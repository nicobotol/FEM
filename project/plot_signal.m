vib = readmatrix('results.txt'); % import the data
% vib = textread('lb_dhn.txt'); % signal
% vib = vib(:,1);
%%
delta_t = 3E-6;
transient_iter_max = size(vib, 1);
time = 0:delta_t:delta_t*transient_iter_max - delta_t;

figure()
plot(time', vib)