function [rho] = bisect(rho_old, V_star, f_grad, g_grad, rho_min, eta, epsilon)
% Function to compute rho with the bisection algorithm

lambda_1 = 1e-10; % guess value for low extreme value
lambda_2 = 1e10;  % guess value for high extreme value


while( (lambda_2 - lambda_1)/( lambda_1 + lambda_2) > epsilon)
  lambda_mid = (lambda_2 + lambda_1) / 2;

  ne = size(f_grad, 1); % number of elements in the structure
  rho = zeros(ne, 1);
  for e = 1:ne    
    Be = - f_grad(e) / (lambda_mid * g_grad(e));
  
    if rho_old(e) * Be^eta <= rho_min
      rho(e) = rho_min;
    elseif rho_min < rho_old(e) * Be^eta &&  rho_old(e) * Be^eta < 1
      rho(e) = rho_old(e) * Be^eta; 
    elseif rho_old(e) * Be^eta >= 1
      rho(e) = 1;
    end

  end

  if rho' * g_grad - V_star > 0
    lambda_1 = lambda_mid;
  else
    lambda_2 = lambda_mid;
  end

end


end