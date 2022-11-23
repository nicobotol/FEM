clc
close all
clear 
% 100000 steps
% load = 1000
% ramp+constant load
% delta_t = 2.2e-6 (s)

line_width = 1.75;
font_size = 30;

time_dependency = readmatrix('results_cantilever4_150_4_static2.txt');
fem_disp = -2.3584E-2; % displacement computed with the static fem code
theoretical = -2.44E-2; % theoretical displacement of the case study
delta_t = 2.2E-6;
transient_iter_max = size(time_dependency, 1);
time = 0:delta_t:delta_t*transient_iter_max - delta_t;

fig_time_displacement = figure('Position', get(0, 'Screensize'));
plot(time', time_dependency, 'LineWidth', line_width)
xlabel('Time [s]')
ylabel('Displacement [m]')
yline(fem_disp, '--r', 'FEM', 'FontSize', font_size, ...
  'LineWidth', line_width, 'LabelHorizontalAlignment', 'left')
yline(theoretical, '--g', 'Theoretical', 'FontSize', font_size, ...
  'LineWidth', line_width, 'LabelVerticalAlignment', 'bottom', ...
  'LabelHorizontalAlignment', 'left')
title('Comparison of static and dynamic deflection')
grid on
set(gca, 'FontAngle', 'oblique', 'FontSize', font_size)
saveas(fig_time_displacement, ['C:\Users\Niccolò\Documents\UNIVERSITA\' ...
  '5° ANNO\FEM\project\images\fig_time_displacement.png'],'png');