omega0 = [0.01018,0.03998,0.06154,0.15709,0.16362,0.22086];
omega_mat = [0.03289,0.18484,0.28560,0.45543,0.77790,0.85692 ;
   0.00919,0.05570,0.14862,0.14960,0.27392,0.42336]';

sol =( omega_mat'*omega_mat) \ (omega_mat'*omega0' )
