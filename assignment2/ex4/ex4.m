clear 
close all
clc

%%
%read the table with values
freq = readmatrix('results_ex4.txt'); % normal input, not band
legend_name = strings; % initialize legend name
size_mat = size(freq, 1);
figure()
for i = 1:size_mat
  plot(freq(i, 2:7), 'o')
  legend_name(i) = num2str(freq(i, 1));
  hold on
end
hold off
xlabel('Mode number')
ylabel('Eigenfrequency [rad/s]')
legend(legend_name, Location='southeast')