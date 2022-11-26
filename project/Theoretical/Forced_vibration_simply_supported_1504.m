%% Forced vibration of a simply supported beam
% The beam is loaded with a sinusoidal input

clear 
close all
clc

%% Load data from file
results = readmatrix("results_forced_simply_supported_1504.txt");

line_width = 3;
font_size = 30;

x = 1; % longitudinal position of where to evaluate the vibration
delta_t = 4e-3;
max_iteration = 5000;
t = 0:delta_t:delta_t*max_iteration-delta_t; % time to compute the vibration

h = 0.05; % Width of the beam
L = 2; % length of the beam
thk = 0.05; % beam thickness
A = h*thk; % cross section of the beam
rho = 7800; % density of the beam
E = 210E9; % young's module
I = h^3*thk/12; % moment of iniertia of the beam

f0 = 1000; % magnitude of external force
a = x; % point of application of the force
omega = 1; % frequency of the external force (rad/s)

n_shapes = 10; % number of modeshapes to be evaluated
n = 1:1:n_shapes; % modeshapes to be evaluated
betaL = n*pi;
omega_n = betaL.^2*sqrt(E*I/(rho*A*L^4)); % modal frequencies

%% Plot the comparison with the theoretical

w = zeros(size(t, 2), 1)';

for i=1:n_shapes % sum the effects of the modeshapes
  mode_n = 2*f0/(rho*A*L)*1/(omega_n(i)^2 - omega^2).*sin(n(i)*pi*a/L) ...
    .*sin(n(i)*pi*x/L).*sin(omega.*t);
  w = w + mode_n;
end

fig_forced_vibration = figure('Position', get(0, 'Screensize'));
plot(t, -w, '-.', 'LineWidth', line_width);
hold on
plot(t', results, '--',  'LineWidth', line_width)
grid on
xlabel('Time [s]')
ylabel('Lateral vibration [m]')
title('Forced vibration simply supported beam')
legend('Theoretical result', 'FEM result','NumColumns',2)
set(gca, 'FontAngle', 'oblique', 'FontSize', font_size)
saveas(fig_forced_vibration, ['C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM' ...
  '\project\images\fig_forced_vibration.png'],'png');


