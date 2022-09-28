close all

euler_20 = load('euler_method_20.mat');
euler = load('euler_method.mat');
euler_200 = load('euler_method_200.mat');
NR_20 = load('NR_20.mat');
NR = load('NR.mat');
NR_200 = load('NR_200.mat');
NR_modified_20 = load('NR_modified_20.mat');
NR_modified = load('NR_modified.mat');
NR_modified_200 = load('NR_modified_200.mat');

figure()
subplot(1,3,1)
plot(euler_20.D_plot, euler_20.P_plot, '-o', 'LineWidth', 1.5, 'MarkerSize',10)
hold on
plot(euler.D_plot, euler.P_plot,'-x', 'LineWidth', 1.5, 'MarkerSize',10)
plot(euler_200.D_plot, euler_200.P_plot, 'LineWidth', 1.5, 'MarkerSize',10)
legend('20 increments', '100 increments', '200 increments','Location','southeast')
xlabel('Displacement [m]')
ylabel('Load P [N]')
title('Euler method')

subplot(1,3,2)
plot(NR_20.D_plot, NR_20.P_plot, '-o', 'LineWidth', 1.5, 'MarkerSize',10)
hold on
plot(NR.D_plot, NR.P_plot,'-x', 'LineWidth', 1.5, 'MarkerSize',10)
plot(NR_200.D_plot, NR_200.P_plot, 'LineWidth', 1.5, 'MarkerSize',10)
legend('20 increments', '100 increments', '200 increments','Location','southeast')
xlabel('Displacement [m]')
ylabel('Load P [N]')
title('Newton Raphson')

subplot(1,3,3)
plot(NR_modified_20.D_plot, NR_modified_20.P_plot, '-o', 'LineWidth', 1.5, 'MarkerSize',10)
hold on
plot(NR_modified.D_plot, NR_modified.P_plot,'-x', 'LineWidth', 1.5, 'MarkerSize',10)
plot(NR_modified_200.D_plot, NR_modified_200.P_plot, 'LineWidth', 1.5, 'MarkerSize',10)
legend('20 increments', '100 increments', '200 increments','Location','southeast')
xlabel('Displacement [m]')
ylabel('Load P [N]')
title('Modified Newton Raphson')

clear 