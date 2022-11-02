clear 
close all
clc

x = linspace(0, 20, 100);
figure()
plot(x, tan(x))
hold on
plot(x, tanh(x))
hold off
%%
alpha = zeros(5, 1);
syms x real
alpha(1) = vpasolve(tan(x)==tanh(x), x, 3.9);
alpha(2) = vpasolve(tan(x)==tanh(x), x, 7.06);
alpha(3) = vpasolve(tan(x)==tanh(x), x, 10.21);
alpha(4) = vpasolve(tan(x)==tanh(x), x, 13); 
alpha(5) = vpasolve(tan(x)==tanh(x), x, 16);

b = 0.05; % beam heigh
thk = 20;
A = thk*b;
L = 1; % beam width
I = b^3*thk/12; % inertia of the section
E = 1; % young's module
rho = 1; % density

beta = alpha/L;

omega = beta.^2*sqrt(E*I/(rho*A))