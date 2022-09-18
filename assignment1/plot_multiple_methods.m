clear 
close all
clc

NR_modified;
NR_method;
euler_method;
text_size = 20;

close all
clear load
nincr = 100;
% euler = load('euler_method.mat', 'D_plot');
% NR = load('NR.mat');
% NR_modified = load('NR_modified.mat', 'D_plot');
% 
% load_v = load('NR.mat', 'P_plot');
% 
% close all
% 
% figure()
% plot(NR.D_plot, load_v.P_plot, 'o')
% hold on
% plot(NR_modified.D_plot, load_v.P_plot,'x')
% plot(euler.D_plot, load_v.P_plot)
% plot(euler_correction.D_plot, load_v.P_plot)
% legend('NR', 'NR modified', 'euler', 'euler corrected','Location','southeast')
% xlabel('Displacement [m]')
% ylabel('Load P [N]')

euler = load('euler_method.mat');
NR = load('NR.mat');
NR_modified = load('NR_modified_results.mat');

%% Plot of the multiple method for 1 increment size (with zoom)
close all

figure_method_comparison = figure('Position', get(0, 'Screensize'));
plot(NR.D_plot, NR.P_plot, '-', 'LineWidth', 1.5, 'MarkerSize',6)
hold on
plot(NR_modified.D_plot, NR_modified.P_plot,'x', 'LineWidth', 1.5, 'MarkerSize',6)
plot(euler.D_plot, euler.P_plot, '--k', 'LineWidth', 1.5, 'MarkerSize',6)
legend('Newton-Raphson', 'Newton-Raphson modified', 'Euler','Location','southeast')
xlabel('Displacement [m]')
ylabel('Load P [N]')
grid on
title('Load-vertical displacement curve for point A', 'FontSize',16)
set(gca,'FontAngle','oblique','FontSize', text_size)
hold all
axes('position',[0.18 .58 .30 .30])
%a2.Position = [0 0 5 5]; % xlocation, ylocation, xsize, ysize
plot(NR.D_plot(51:63),NR.P_plot(51:63), 'LineWidth', 1.5, 'MarkerSize',6); %axis tight
hold on
plot(NR_modified.D_plot(51:63),NR_modified.P_plot(51:63),  'x', 'LineWidth', 1.5, 'MarkerSize',6); %axis tight
plot(euler.D_plot(51:63),euler.P_plot(51:63), '--k', 'LineWidth', 1.5, 'MarkerSize',6); axis tight
grid on
set(gca,'FontAngle','oblique','FontSize', text_size)
saveas(figure_method_comparison, 'C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\assignment1\method_comparison.png','png');

%% Plot multiple increments

figure()
iteration_n = [1:1:nincr];
plot(iteration_n, NR.residual_norm);
hold on
plot(iteration_n, euler.residual_norm);
plot(iteration_n, NR_modified.residual_norm);
hold off
xlabel('Increment number')
ylabel('Norm of he residual')
title('Norm of the residuals')
