case1 = load('E3_case1.mat', 'D_plot', 'P_plot');
case2 = load('E3_case2.mat', 'D_plot', 'P_plot');
case3 = load('E3_case3.mat', 'D_plot', 'P_plot');

E3_comparison = figure('Position', get(0, 'Screensize'));
plot(case2.D_plot, case2.P_plot, 'LineWidth', 1.5)
hold on
plot(case1.D_plot, case1.P_plot, 'LineWidth', 1.5)
plot(case3.D_plot, case3.P_plot, 'LineWidth', 1.5)
hold off
xlabel('\Delta_Y [m]')
ylabel('Force [N]')
legend('E_{c}=0.4, P_{f}=0.002', 'E_{c}=0.8, P_{f}=0.004', 'E_{c}=1.6, P_{f}=0.008','Location','northwest')
title('Influence of materila properties')
set(gca, 'FontAngle', 'oblique', 'FontSize', 20)
saveas(E3_comparison, 'C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\assignment1\E3_comparison.png','png');