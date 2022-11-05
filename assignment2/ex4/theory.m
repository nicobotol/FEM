clear 
close all
clc

k = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
%% numerical solution of the equation
syms x
nsol(1) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 5);
nsol(2) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 7.8);
nsol(3) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 11);
nsol(4) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 14);
nsol(5) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 17.2);
nsol(6) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 20.4);
nsol(7) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 23.4);
nsol(8) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 26.7);
nsol(9) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 29.8);
nsol(9) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 33);

len_nsol = size(nsol, 2);

%% Theoretical solution
E = 1;
rho = 1;
L = 16; % length of the bar

% inplane vibration
thk_ip = 0.2;
h_ip = 1; % height

A_ip = thk_ip*h_ip;
I_ip = h_ip^3*thk_ip/12;

fprintf('Inplane vibration \n')
for i = 1:len_nsol 
  omega_ip(i) = eval(nsol(i)^2*sqrt(E*I_ip/(rho*A_ip))/L^2);
  fprintf('%i %.8f\n',i,omega_ip(i));
end
fprintf('\n \n')

% out o plane vibration
thk_op = 1;
h_op = 0.2; % height

A_op = thk_op*h_op;
I_op = h_op^3*thk_op/12;

fprintf('Out of plane vibration \n')
for i = 1:len_nsol 
  omega_op(i) = eval(nsol(i)^2*sqrt(E*I_op/(rho*A_op))/L^2);
  fprintf('%i %.8f\n',i,omega_op(i));
end
fprintf('\n \n')

% Longitudinal vibration
fprintf('Longitudinal vibration \n')
omega_lon = [1, 2, 3, 4, 5, 6, 7]*pi/L*sqrt(E/rho);
for i =1:7
  fprintf('%i %.8f\n',i,omega_lon(i));
end
fprintf('\n \n')


% Put all togheter
% omega = [omega_ip, omega_op, omega_lon];
omega = [omega_ip, omega_lon];
omega = sort(omega);
dim_omega = size(omega, 2);

fprintf('All sorted vibration \n')
for i=1:6
  fprintf('%i %.8f\n',i,omega(i));
end

%% 
% close all
% x=32:0.001:33;
% y = -cos(x).*cosh(x) + 1;
% figure()
% plot(x, y)
% grid on

%% Solutions from Rao
% beta_l = [4.730041 7.853205 10.995608 14.137165];
% omega = (beta_l.^2)/L^2*sqrt((E*I)/(rho*A));
