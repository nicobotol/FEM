% close all
clear
clc

s = readmatrix('results.txt'); % signal
% s = textread('lb_dhn.txt'); % signal
% s = s(:,1);
% N = size(s, 1); % number of points of the signal
% s = s(N/2:end);
Ts = 5E-3; % sample period
% Ts = 9E-5; % sample period
fs = 1/Ts; % sample frequency

figure()
plot(s)

% apply zero padding
N = size(s, 1); % number of points of the signal
s = [s' zeros(4*N, 1)'];
N = size(s, 2); % number of points of the signal

f = fs*(0:N/2-1)/N; % trnsform the samples into frequencies

S = fft(s); % fft of the signal
S_oneSide = S(1:N/2); % one side FFT
S_mag = abs(S_oneSide)/(N/2);

figure()
plot(f, S_mag)
xlabel('Frequency [Hz]')
ylabel('Magnitude')