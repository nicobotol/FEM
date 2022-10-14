% Assignment 2 - Exercise 1
clear
close all
clc

%%
line_width = 2;
text_size = 20;

%% load data
%mesh_name = string(10);
mesh_name = ["2000", "1000", "500", "250", "125", "16", "8", "4", "2", ...
  "1"];

data_n_band = readtable('results_n_band.txt'); % normal input, not band
data_band = readtable('results_band.txt'); % normal input, banded
data_band_renum = readtable('results_band_renum.txt'); % renum input, band
data_band_bandfem = readtable('results_band_bandfem.txt'); % bandfem ...
% input, band

%% plot
bandwidth = figure('Position', get(0, 'Screensize'));
plot(data_band.bandwidth(:), 'o', 'LineWidth', line_width)
hold on
plot(data_band_renum.bandwidth(:), 'x', 'LineWidth', line_width);
plot(data_band_bandfem.bandwidth(:), '+', 'LineWidth', line_width);
hold off
set(gca,'XTick',  1:1:10)
set(gca,'XTickLabel', mesh_name)
xlabel("ne_x")
ylabel("Bandwidth")
title("Bandwidth")
legend('Original', 'Renum', 'Bandfem', 'Location','northwest')
set(gca, 'YScale', 'log')
set(gca, 'FontAngle', 'oblique', 'FontSize', text_size)
saveas(bandwidth , 'C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\assignment2\images\bandwidth.png','png');

neqn = figure('Position', get(0, 'Screensize'));
plot(data_band.neqn(:), 'o', 'LineWidth', line_width)
set(gca,'XTick',  1:1:10)
set(gca,'XTickLabel', mesh_name)
xlabel("ne_x")
ylabel("Number of equations")
title("Number of equations")
set(gca, 'FontAngle', 'oblique', 'FontSize', text_size)
saveas(neqn, 'C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\assignment2\images\neqn.png','png');

time = figure('Position', get(0, 'Screensize'));
plot(data_band.time(:), 'o', 'LineWidth', line_width)
hold on
plot(data_band_renum.time(:), 'x', 'LineWidth', line_width);
plot(data_band_bandfem.time(:), '+', 'LineWidth', line_width);
hold off
set(gca,'XTick',  1:1:10)
set(gca,'XTickLabel', mesh_name)
xlabel("ne_x")
ylabel("Time (s)")
title("CPU time")
legend('Original', 'Renum', 'Bandfem', 'Location','northwest')
set(gca, 'YScale', 'log')
set(gca, 'FontAngle', 'oblique', 'FontSize', text_size)
saveas(time, 'C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\assignment2\images\time.png','png');

figure()
plot(data_band.cost(:))
set(gca,'XTick',  1:1:10)
set(gca,'XTickLabel', mesh_name)
xlabel("ne_x")
ylabel("Computational cost")
title("Computational cost")

% mesh_number = size(data_n_band(:,1), 1); % number of mesh tested
% mesh_size = zeros(mesh_number);
% bw_band = zeros(mesh_number); % bandwidth
% neqn_band = zeros(mesh_number); % number of equations
% cost_band = zeros(mesh_number); % cost
% 
% mesh_name = data_band(:, 1);
% bw_band = data_band(:, 2);
% neqn_band = data_band(:, 3);
% cost_band = data_band(:, 4);
% 
% mesh_name_n = data_n_band(:, 1);
% bw_band_n = data_n_band(:, 2);
% neqn_band_n = data_n_band(:, 3);
% cost_band_n = data_n_band(:, 4);




