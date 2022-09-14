clear 
close all
clc

NR_modified_method;
NR_method;
euler_correction_method;
euler_method;
NR_method_week3;

close all

NR = load('NR.mat', 'D_plot');
NR_modified = load('NR_modified.mat', 'D_plot');
euler = load('euler_method.mat', 'D_plot');
euler_correction = load('euler_correction.mat', 'D_plot');
load = load('NR.mat', 'P_plot');
close all
figure()
plot(NR.D_plot, load.P_plot)
hold on
plot(NR_modified.D_plot, load.P_plot)
plot(euler.D_plot, load.P_plot)
plot(euler_correction.D_plot, load.P_plot)
legend('NR', 'NR modified', 'euler', 'euler corrected','NE week 3', 'Location','southeast')
xlabel('Displacement [m]')
ylabel('Load P [N]')