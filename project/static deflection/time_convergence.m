%% STUDY OF CONVERGENCE FOR DIFFERENT TIME STEP
% file: cantilever 4
% load: ramp+constant; 3/4 time increasing + 1/4 constant
% load mag = 100000
% damping = 0
close all
clc

line_width = 1.75;
font_size = 30;

courant_time = 2.41E-6;
static_deflection = -0.02358; % static deflection 

delta_2e6 = readmatrix('results_cantilever4_1504_2e-6.txt');
delta_3e6 = readmatrix('results_cantilever4_1504_3e-6.txt');
delta_1dot5e6 = readmatrix('results_cantilever4_1504_1dot5e-6.txt');
delta_8e7 = readmatrix('results_cantilever4_1504_8e-7.txt');
delta_2dot2e6 = readmatrix('results_cantilever4_1504_2dot2e-6.txt');
delta_2dot38e6 = readmatrix('results_cantilever4_1504_2dot38e-6.txt');

delta_t = [2e-6 1.5e-6 8e-7 2.2E-6]; % 2.2E-6 
transient_iter_max = 100000; % steps of the iteration

%%
% plot the signals as function of steps
fig_disp_step = figure('Position', get(0, 'Screensize'));
plot(NaN, NaN, 'LineWidth', line_width)
hold on
%plot(delta_2dot38e6, 'LineWidth', line_width)
plot(delta_2dot2e6, 'LineWidth', line_width)
plot(delta_2e6, 'LineWidth', line_width)
plot(delta_1dot5e6, 'LineWidth', line_width)
plot(delta_8e7, 'LineWidth', line_width)
hold off
xlabel('Step increment')
ylabel('Displacement [m]')
legend('3 [\mus]', '2.2 [\mus]', '2 [\mus]', '1.5 [\mus]', ...
  '0.8 [\mus]', 'Location', 'southwest')
grid on
ylim([-0.045 0.01])
set(gca, 'FontAngle', 'oblique', 'FontSize', font_size)
title('Displacement as function of step increment')
axes('position',[0.58 .58 .30 .30])
%a2.Position = [0 0 5 5]; % xlocation, ylocation, xsize, ysize
plot(delta_3e6, 'LineWidth', line_width)
hold off
grid on
set(gca, 'FontAngle', 'oblique', 'FontSize', font_size)
saveas(fig_disp_step, ['C:\Users\Niccolò\Documents\UNIVERSITA\' ...
  '5° ANNO\FEM\project\images\fig_disp_step.png'],'png');


%%
% Avarege static deflection
delta_average_2e6 = mean(delta_2e6(transient_iter_max/2:end));
% delta_average_3e6 = mean(delta_3e6(transient_iter_max/2:end));
delta_average_3e6 = -0.02; % diverge
delta_average_1dot5e6 = mean(delta_1dot5e6(transient_iter_max/2:end));
delta_average_8e7 = mean(delta_8e7(transient_iter_max/2:end));
delta_average_2dot2e6 = mean(delta_2dot2e6(transient_iter_max/2:end));
delta_average_2dot38e6 = delta_average_3e6; % diverge

delta_average = [delta_average_2e6 delta_average_1dot5e6 ...
  delta_average_8e7 delta_average_2dot2e6];


fig_time_convergence = figure('Position', get(0, 'Screensize'));

plot(delta_t, delta_average, 'o', 'LineWidth', 5*line_width)
hold on
plot([2.38e-6 3e-6 ], [delta_average_2dot38e6 delta_average_3e6],'o', 'LineWidth', 5*line_width, ...
  'Color', [0.8500 0.3250 0.0980])
% plot(2.38e-6, delta_average_2dot38e6,'o', 'LineWidth', 5*line_width, ...
%   'Color', [0.8500 0.3250 0.0980])
hold off
% h = text((3e-6 - 1e-8), delta_average_3e6 - 0.0042, 'Divergence \Rightarrow', ...
%   'FontSize',font_size);
% h = text((2.38e-6 - 1e-8), delta_average_3e6 - 0.0042, 'Divergence \Rightarrow', ...
%   'FontSize',font_size);
% set(h, 'Rotation', 90)
xlabel('Time step \Delta_t [s]')
ylabel('Mean displacement [mm]')
title('Mean displacement after the application of total load')
xlim([0.9*min(delta_t), 1.1*3e-6])
xticks(0.9*min(delta_t):2e-7:1.1*3e-6)
legend('Convergence', 'Divergence', 'Location', 'southeast');
xline(courant_time, '--r', {'Courant time'}, 'LineWidth', line_width, ...
  'FontSize', font_size, 'LabelVerticalAlignment', 'middle', 'HandleVisibility','off')
yline(static_deflection, '--', {'Static deflection'}, 'LineWidth', line_width, ...
  'FontSize', font_size, 'LabelHorizontalAlignment', 'left', ...
  'Color', [0.4660 0.6740 0.1880], 'HandleVisibility','off')
grid on
set(gca, 'FontAngle', 'oblique', 'FontSize', font_size)
saveas(fig_time_convergence, ['C:\Users\Niccolò\Documents\UNIVERSITA\' ...
  '5° ANNO\FEM\project\images\fig_time_convergence.png'],'png');