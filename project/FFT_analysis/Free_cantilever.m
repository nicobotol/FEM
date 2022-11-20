
E = 2.11E11;
rho = 7.6E3;
thk = 0.05;
h = 0.05;
L = 2;
I = h^3*thk/12;
A = h*thk;

% laterla vibration
beta_L = [1.8751 4.6940 7.8547 10.9955];
omega_lateral = (beta_L).^2*sqrt(E*I/(rho*A*L^4));
freq_lateral = omega_lateral/pi*0.5;

% longitudinal vibration
c = sqrt(E/rho);
n_max = 4; % max number of eignefrequencies to investigate
n = 0:1:n_max;
omega_long = (2*n + 1)*pi*c/(2*L);
freq_long = omega_long/(2*pi);

% total vibrations
omega = sort([omega_lateral omega_long]);
omega(1:6)/pi*0.5