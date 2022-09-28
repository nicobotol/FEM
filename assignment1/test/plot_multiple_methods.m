clear 
close all
clc

NR_modified_method_week3;
NR_method_week3;
euler_correction_method;
euler_method;

close all
clear load

NR = load('NR_week3.mat');
NR_modified = load('NR_modified.mat', 'D_plot');
euler = load('euler_method.mat', 'D_plot');
euler_correction = load('euler_correction.mat', 'D_plot');
load_v = load('NR.mat', 'P_plot');

close all

figure()
plot(NR.D_plot, load_v.P_plot, 'o')
hold on
plot(NR_modified.D_plot, load_v.P_plot,'x')
plot(euler.D_plot, load_v.P_plot)
plot(euler_correction.D_plot, load_v.P_plot)
plot(NR.lin_space*3, NR.signorini_plot, '--', 'LineWidth',1.5 )
legend('NR', 'NR modified', 'euler', 'euler corrected', 'signorini','Location','southeast')
xlabel('Displacement [m]')
ylabel('Load P [N]')