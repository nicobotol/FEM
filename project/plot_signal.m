vib = readmatrix('results_damping.txt'); % import the data
% vib = textread('lb_dhn.txt'); % signal
% vib = vib(:,1);
%%
delta_t = 5E-4; 
transient_iter_max = size(vib, 1);
time = 0:delta_t:delta_t*transient_iter_max - delta_t;

figure()
plot(time', vib)
xlabel('Time [s]')
ylabel('Displacement [m]')
