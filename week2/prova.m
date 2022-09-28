c1 = 1;
c2 = 50;
c3 = 0.1;
c4 = 100;

epsilon = 50;

Et = @(epsilon) c4*(c1*(1 + 2*(1 + c3*epsilon)^(-3)) + 3*c2*(1 + c3*epsilon)^(-4) + 3*c3*(-1 + (1 + c3*epsilon)^2 - 2*(1 + c3*epsilon)^(-3) + 2*(1 + c3*epsilon)^(-4)));
Et(epsilon)