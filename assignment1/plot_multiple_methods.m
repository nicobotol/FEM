clear 
close all
clc

NR_modified;
NR_method;
euler_method;
text_size = 20;

close all
clear load
nincr = 1000;
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

euler = load('euler_method_200.mat');
NR = load('NR_200.mat');
NR_modified = load('NR_modified_200.mat');

%%
close all

figure_method_comparison = figure('Position', get(0, 'Screensize'));
plot(NR.D_plot, NR.P_plot, '-o', 'LineWidth', 1.5, 'MarkerSize',6)
hold on
plot(NR_modified.D_plot, NR_modified.P_plot,'-', 'LineWidth', 1.5, 'MarkerSize',6)
plot(euler.D_plot, euler.P_plot, 'LineWidth', 1.5, 'MarkerSize',6)
legend('NR', 'NR modified', 'Euler','Location','southeast')
xlabel('Displacement [m]')
ylabel('Load P [N]')
title('Load-vertical dispalcement curve for point A')

set(gca,'FontAngle','oblique','FontSize', text_size)
saveas(figure_method_comparison, 'C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\assignment1\method_comparison.png','png');

%%

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
