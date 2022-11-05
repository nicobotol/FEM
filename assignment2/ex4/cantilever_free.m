clear 
close all
clc

k = [0, 1, 2, 3, 4, 5, 6];
h = 1;
E = 1;
rho = 1;
L1 = 5.5;
L2 = 10.5;

omega1 = k.^2*pi^2/L1^2*sqrt(E*h^2/(rho*12));
omega2 = k.^2*pi^2/L2^2*sqrt(E*h^2/(rho*12));

% longitudinal vibration
omega_lon = (2.*k + 1)*pi*sqrt(E/rho)/L2*0.5;
disp(omega_lon);

% beta_l = [1.875104 4.694091 7.854757 10.995541];
% 
% omega1 = (beta_l./L1).^2*sqrt(1/12);
% omega2 = (beta_l./L2).^2*sqrt(1/12);
% disp(omega1)
% disp(omega2)

