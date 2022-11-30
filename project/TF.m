close all
clear 
clc

vib= readmatrix('results_damping.txt'); % import the data
force= readmatrix('results_force.txt'); % import the data
% vib = textread('lb_dhn.txt'); % signal
% vib = vib(:,1);
%%
% delta_t = 5E-2; 
% transient_iter_max = size(vib, 1);
% time = 0:delta_t:delta_t*transient_iter_max - delta_t;
% 
% figure()
% plot(time', vib)
% xlabel('Time [s]')
% ylabel('Displacement [m]')


%% Experimental transfer function
% t : time vector
% acc : acceleration vector
% fs : sampling frequency
% Tf : transfer function ( complex )
% Fr : vector of the frequencies
fs = 5E-4;
[Tf , Fr] = tfestimate(force, vib, [ ] , [ ] , [ ] , fs);

% Tm : magnitude of the transfer function
Tm = abs(Tf); % real vector
m_peak = max(Tm); % max peak

%id = find(Fr > 2000);

experimental_tf = figure('Position', get(0, 'Screensize'));
plot(Fr*2*pi, Tm, 'LineWidth', 1.5)
%plot(Fr(1: id-1)*2*pi, Tm(1: id-1), 'LineWidth', 1.5)
title('Experimental frequency response (magnitude)')
xlabel('Agular frequency (rad/s)')
ylabel('|G| (ms^{-2}N^{-1})')
yline(m_peak/sqrt(2))
grid on
