clear 
close all
clc

k = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10];
thk = 1;
h = 1;
E = 1;
rho = 1;
L = 16;

A = thk*h;
I = h^3*thk/12;
% omega = sqrt(E*I/(rho*A))*pi^2/L^2*k.^2;
% disp(omega);

%%
syms x
nsol(1) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 5);
nsol(2) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 7.8);
nsol(3) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 11);
nsol(4) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 14);
nsol(5) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 17.2);
nsol(6) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 20.4);
nsol(7) = vpasolve(-cos(x)*cosh(x) + 1 == 0, x, 23.4);

for i = 1:7
  omega(i) = eval(nsol(i)^2*sqrt(E*I/(rho*A))/L^2);
end
disp(omega)

%% 
close all
x=23.5:0.001:24.5;
y = -cos(x).*cosh(x) + 1;
figure()
plot(x, y)
grid on
