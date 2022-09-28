%% Signorini stress strain curve
close all
clear all
clc

L0 = 3;
A = 2;
c1 = 1;
c2 = 50;
c3 = 0.1;
c4 = 100;

force = @(epsilon) A*( c1*((1+c4*epsilon) - (1+c4*epsilon)^(-2)) + c2*(1 - (1+c4*epsilon)^(-3)) + c3 * (1 - 3*(1+c4*epsilon) + (1+c4*epsilon)^3 - 2*(1+c4*epsilon)^(-3) + 3*(1+c4*epsilon)^(-2)));

fplot(force, [0 0.07])
xlabel("Strain")
ylabel("Force (N)")
grid on