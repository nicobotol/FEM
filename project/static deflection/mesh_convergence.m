clc
close all
clear 

deflection = [1917 2175 2283 2358 2405]/1E5; % static deflection for 3 different mesh size
theoretical = 0.0244;
mesh_size = [3625 2167 1625 1292 900]/1E5; % mesh size

line_width = 1.75;
font_size = 30;

fig_mesh_convergence = figure('Position', get(0, 'Screensize'));
% plot(mesh_size, deflection, '-x', 'MarkerSize',15, ...
%   'LineWidth',line_width/2);
% hold on
plot(mesh_size.^2, deflection, '-x', 'MarkerSize',30, ...
  'LineWidth',line_width);
yline(theoretical,'--r', 'Analytical deflection', 'LineWidth',line_width, ...
   'FontSize', font_size)
ylim([0.018, 0.025])
xlabel('Mesh size h^2 [m^2]')
ylabel('Static deflection [m]')
title('Convergence analysis for static deflection')
set(gca, 'FontAngle', 'oblique', 'FontSize', font_size)
saveas(fig_mesh_convergence, ['C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM' ...
  '\project\images\fig_mesh_convergence.png'],'png');