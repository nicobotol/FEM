%% Forced vibration of a simply supported beam
% The beam is loaded with a sinusoidal input

% clear 
% close all
% clc

line_width = 1.5;

x = 1; % longitudinal position of where to evaluate the vibration
delta_t = 5e-2;
max_iteration = 2500;
t = 0:delta_t:delta_t*max_iteration; % time to compute the vibration

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

n_shapes = 1; % number of modeshapes to be evaluated
n = 1:1:n_shapes; % modeshapes to be evaluated
betaL = n*pi;
omega_n = betaL.^2*sqrt(E*I/(rho*A*L^4)); % modal frequencies

legend_names = string();
w = zeros(size(t, 2), 1)';
figure()
for i=1:n_shapes % sum the effects of the modeshapes
  mode_n = 2*f0/(rho*A*L)*1/(omega_n(i)^2 - omega^2).*sin(n(i)*pi*a/L) ...
    .*sin(n(i)*pi*x/L).*sin(omega.*t);
  w = w + mode_n;
%   plot(t, mode_n, 'LineWidth', line_width/5);
%   hold on
  legend_names(i) = strcat('Mode', num2str(i));
end
plot(t, w, 'LineWidth', line_width);
xlabel('Time [s]')
title('Theoretical vibration simply supported')
%legend_names(end + 1) = 'Tot';
%legend(legend_names)
ylabel('Lateral vibration [m]')