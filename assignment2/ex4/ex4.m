clear 
close all
%clc
thk = 1;
h = 1;
E = 1;
rho = 1;
L = 16;
L_cf1 = 5; % length for the cantilever free beam left
L_cf2 = 10; % length for the cantilever free beam right


A = thk*h;
I = h^3*thk/12;
% omega = sqrt(E*I/(rho*A))*pi^2/L^2*k.^2;
% disp(omega);

%%
% Solution for lateral vibration of the fixed-fixed beam
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

% solution for the lateral vibration of the fixed-free beam
nsol_cf(1) = vpasolve(cos(x)*cosh(x) + 1 == 0, x, 1.87);
nsol_cf(2) = vpasolve(cos(x)*cosh(x) + 1 == 0, x, 4.69);
nsol_cf(3) = vpasolve(cos(x)*cosh(x) + 1 == 0, x, 7.85);
nsol_cf(4) = vpasolve(cos(x)*cosh(x) + 1 == 0, x, 10.99);
nsol_cf(5) = vpasolve(cos(x)*cosh(x) + 1 == 0, x, 13.6);

% inplane vibration
for i = 1:7
  omega_in(i) = eval(nsol(i)^2*sqrt(E*I/(rho*A))/L^2);
  % fprintf('%i %.8f\n',i,omega_in(i));
end

% inplane vibration cantilever free left
omega_in_cf1 = nsol_cf.^2*sqrt(E*I/(rho*A))/L_cf1^2;

% inplane vibration cantilever free left
omega_in_cf2 = nsol_cf.^2*sqrt(E*I/(rho*A))/L_cf2^2;

% theoretical longitudinal vibration fixed-fixed
omega_lon = [1, 2, 3, 4, 5, 6, 7]*pi/L*sqrt(E/rho);

% longitudinal vibration cantilevered-free
omega_lon_cf = (2*[0, 1, 2, 3] + 1)*pi*sqrt(E/rho)/L_cf2*0.5;

%% Load data from the simulation
% read the table with values of the simulation
freq = readmatrix('results_ex4.txt'); % results for different thk
freq_cantilever = readmatrix('results_cantilever.txt'); % cantilever
legend_name = strings; % initialize legend name
size_mat = size(freq, 1);

%%
text_size = 30;
plot_freq = figure('Position', get(0, 'Screensize'));
for i = 1:6
  plot(freq(:, 1), freq(:, i + 1), '--ko', MarkerSize=5, MarkerFaceColor='k', LineWidth=0.75)
  %legend_name(i) = strcat('x = ', num2str(freq(row_plot(i), 8)), '%');
  % fill hte legend vector
  hold on
end

% theoretical inplane lateral vibration fix-fix
plot([0.2 0.2 0.2 0.2 0.2], [omega_in(1), omega_in(2), omega_in(3), ...
  omega_in(4), omega_in(5)], 'rx', MarkerSize=20, LineWidth=1.75); 
% inplane vibration

% theoretical inplane longitudinal vibration fix-fix
plot(0.2, omega_lon(1), 'bx', MarkerSize=20, LineWidth=1.75); 
% longitudinal vibration

% theoretical inplane lateral vibration fixed-free right
plot([0 0 0 ], [omega_in_cf2(1), omega_in_cf2(2), ...
  omega_in_cf2(3)], 'r+', MarkerSize=20, LineWidth=1.75 );

plot([0 0 ], [ omega_in_cf1(1), omega_in_cf1(2)], 'g*', ...
  MarkerSize=20, LineWidth=1.75 );

% theoretical inlane longitudinal vibration fixed-free
plot(0, omega_lon_cf(1), 'b+', MarkerSize=20, LineWidth=1.75);

text(0.1, 0.3, '6', 'FontSize', text_size)
text(0.1, 0.215, '5', 'FontSize', text_size)
text(0.1, 0.185, '4', 'FontSize', text_size)
text(0.1, 0.115, '3', 'FontSize', text_size)
text(0.1, 0.08, '2', 'FontSize', text_size)
text(0.1, 0.035, '1', 'FontSize', text_size)
hold off
% legend('1', '2', '3', '4', '5', '6', 'lateral fixed-fixed', ...
%  'longitudinal fixed-fixed', ' lateral fixed-free', ...
%  'longitudinal fix-free', Location="eastoutside"); 
xlim([-0.005 0.205]);
ylim([0, 0.4])
xlabel('x')
xticks(0:0.02:0.2)
xticklabels({'0.0','0.1','0.2','0.3','0.4','0.5','0.6','0.7','0.8','0.9','1.0'})
ylabel('Eigenfrequency [rad/s]', 'FontSize', text_size)
title('Eigenfrequency as function of x', 'FontSize', text_size)
% axes('Position',[.05 .7 .4 .2])
% imshow('thk_0.png')
% [bottomleftcornerXposition bottomleftcornerYposition width height]
set(gca, 'FontAngle', 'oblique', 'FontSize', text_size)
saveas(plot_freq, ['C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\' ...
  'assignment2\ex4\images\plot_freq.png'],'png');