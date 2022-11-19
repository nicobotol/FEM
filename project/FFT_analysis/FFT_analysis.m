% close all
clear
clc

font_size = 20;
line_width = 2;

eigen = [11.8 73.9 206.3 402.6 661.7 649.5 ]; % frequency from eigne analysis

s_lat = readmatrix('cantilever4_lateral.txt'); %lateral vibration
s_long = readmatrix('cantilever4_longitudinal.txt'); %lateral vibration
Ts = 2E-6; % sample period
fs = 1/Ts; % sample frequency
f_max = 800; % max frequency to be plotted

% apply zero padding
N = size(s_lat, 1); % number of points of the signal
s_lat = [s_lat' zeros(4*N, 1)'];
s_long = [s_long' zeros(4*N, 1)'];
N = size(s_lat, 2); % number of points of the signal

f = fs*(0:N/2-1)/N; % trnsform the samples into frequencies

S_lat = fft(s_lat,N); % fft of the signal
S_oneSide_lat = S_lat(1:N/2); % one side FFT
S_mag_lat = abs(S_oneSide_lat)/(N/2);

S_long = fft(s_long); % fft of the signal
S_oneSide_long = S_long(1:N/2); % one side FFT
S_mag_long = abs(S_oneSide_long)/(N/2);

[~, max_sample] = min(abs(f - f_max)); % element number of the closes sample

fig_FFT = figure('Position', get(0, 'Screensize'));
plot(NaN, NaN, '--','Color',[0.4940 0.1840 0.5560])
hold on
yyaxis left
plot(f(1:max_sample), S_mag_lat(1:max_sample),'Color',[0 0.4470 0.7410], ...
  'LineWidth', line_width)
ylabel('Lateral vibration', 'Color',[0 0.4470 0.7410])
set(gca, 'YColor', [0 0.4470 0.7410])
hold on
yyaxis right
plot(f(1:max_sample), S_mag_long(1:max_sample), 'LineWidth', line_width)
ylabel('Longitudinal vibration')
xlabel('Frequency [Hz]')
title('Step response frequency analysis')
for i= 1:5
  xline(eigen(i), '--', num2str(eigen(i)), 'Color', ...
    [0.4940 0.1840 0.5560], 'LineWidth', line_width/2,'FontSize', font_size)
end
xline(eigen(6), '--', num2str(eigen(6)),'LabelHorizontalAlignment', ...
  'left','Color',[0.4940 0.1840 0.5560], 'LineWidth', line_width/2, ...
  'FontSize', font_size )

set(gca, 'FontAngle', 'oblique', 'FontSize', font_size)
saveas(fig_FFT, ['C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM' ...
  '\project\images\fig_FFT.png'],'png');

[legend_handle, icons] = legend('Eigenvalue analysis', ...
  'TextColor', [0.4940 0.1840 0.5560]); 
set(legend_handle, 'Box', 'off')
% get the current position of the legend object
leg_pos=get(legend_handle,'position');
% assign the required position of the legend to a new variable
new_leg_pos=[.75 0.45 .2 leg_pos(4)+.2] ;
% Get current line data (horizontal line)
xd = icons(2).XData;
yd = icons(2).YData;
% Swap X and Y data for line (make vertical line)
icons(2).XData = yd;
icons(2).YData = xd;
% Rotate and reposition the text
set(icons(1),'rotation',90)
icons(1).Position = [0.5 0.4 0];
% Adjust legend size to accomodate changes.
set(legend_handle,'position',new_leg_pos);