%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%  RUN OVER DIFFERENT NUMBER OF NODES WITH ONLY NEIGHBOR CONNECTIONS      %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function fea()

close all
clc

%--- Input file ----------------------------------------------------------%
file_vector = [ ...
  "C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\assignment1\ex4\mesh1.m", ...
  "C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\assignment1\ex4\mesh2.m" ...
  "C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\assignment1\ex4\mesh3.m" ...
  "C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\assignment1\ex4\mesh4.m" ...
  "C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\assignment1\ex4\mesh5.m" 
  ];

file_number = size(file_vector, 2);
node_number_vector = [10 29 90 223 406]; % number of the node where the force is applied
disp_vector = zeros(1, file_number); % vector of final displacements
comple_vector = zeros(1, file_number);  % vector of final compliance
total_nodes = zeros(1, file_number); % number of nodes of the structure

for j=1:file_number
%loadfile(file_vector(j));
run(file_vector(j));
node_number = node_number_vector(j);
%ex4_1

neqn = size(X,1)*size(X,2);         % Number of equations
ne = size(IX,1);                    % Number of elements
disp(['Number of DOF ' sprintf('%d',neqn) ...
    ' Number of elements ' sprintf('%d',ne)]);

%--- Initialize arrays ---------------------------------------------------%

Kmatr=sparse(neqn,neqn);                % Stiffness matrix
P=zeros(neqn,1);                        % Force vector
D=zeros(neqn,1);                        % Displacement vector
R=zeros(neqn,1);                        % Residual vector
strain=zeros(ne,1);                     % Element strain vector
stress=zeros(ne,1);                     % Element stress vector
rho = zeros(ne,1);
rho_old = zeros(ne,1);

[~,~,~,~,~,g_grad,~] =recover(mprop,X,IX,D,ne,strain,stress,P, p, rho); % volume vector

    % initialize the density vector, supposing each element equal
  rho(:) = V_star / sum(g_grad); % v and g_grad are the same
  [P]=buildload(X,IX,ne,P,loads,mprop);       % Build global load vector
  
  for i=1:iopt_max
    rho_old = rho;
  
    [Kmatr]=buildstiff(X,IX,ne,mprop,Kmatr,p,rho_old);    % Build global stiffness matrix
  
    [Kmatr,Pmatr]=enforce(Kmatr,P,bound);       % Enforce boundary conditions
  
    D = Kmatr \ Pmatr;                          % Solve system of equations
  
    [~,~,~,~,f_grad,~,~] =recover(mprop,X,IX,D,ne,strain,stress,P, p, rho_old); % compute f_grad
  
    rho = bisect(rho_old, V_star, f_grad, g_grad, rho_min, eta, epsilon); % compute the better guess for rho
    
    [~,~,~,~,~,~,f(i)] =recover(mprop,X,IX,D,ne,strain,stress,P, p, rho); % compute the compliance f
  
     if norm(rho_old - rho) < norm(rho) * epsilon % check the convergence 
       disp(i)
      break
     end
  end
  disp_vector(j) = D(node_number * 2); % y displacement of the node of interest 
  compl_vector(j) = f(end); % final compliance of the method
  total_nodes(j) = size(X, 1); % number of nodes

end
%%
%--- Plot results --------------------------------------------------------%                                                        
compliance_vs_node_neighbor = figure('Position', get(0, 'Screensize'));
plot(total_nodes, compl_vector, 'o', 'LineWidth', 3.5);
xlabel('Nodes')
ylabel('Compliance [J]')
title('Compliance for only neighbor connections')
set(gca, 'FontAngle', 'oblique', 'FontSize', 20)
saveas(compliance_vs_node_neighbor, 'C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\assignment1\compliance_vs_node_neighbor.png','png');

displacement_vs_node_neighbor = figure('Position', get(0, 'Screensize'));
plot(total_nodes, disp_vector, 'o', 'LineWidth', 3.5)
xlabel('Nodes')
ylabel('Displacement [m]')
title('Displacement for only neighbor connection')
set(gca, 'FontAngle', 'oblique', 'FontSize', 20)
saveas(compliance_vs_node_neighbor, 'C:\Users\Niccolò\Documents\UNIVERSITA\5° ANNO\FEM\assignment1\compliance_vs_node_neighbor.png','png');
%%
return

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Build global load vector %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [P]=buildload(X,IX,ne,P,loads,mprop);

rowX = size(X, 1);
columnX = size(X, 2);

% P contains the external forces applied by the load
% P is a 2nnx1 matrix (nn is the number of nodes) 
P = zeros(columnX * rowX, 1); % assign the memory for P

for i=1:size(loads,1)
  pos = loads(i, 1)*columnX - (columnX - loads(i, 2));
  P(pos, 1) = loads(i, 3);
end


return

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Build global stiffness matrix %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [K]=buildstiff(X,IX,ne,mprop,K,p,rho);

% This subroutine builds the global stiffness matrix from
% the local element stiffness matrices

K = zeros(2*size(X, 1)); % allocate memory

for e = 1:ne % cycle on the different bar 
  % compute the bar length
  [L0, delta_x, delta_y] = length(IX, X, e);
  % displacement vector (4x1)
  B0 = 1/L0^2 * [-delta_x -delta_y delta_x delta_y]';
  
  % materials properties
  propno = IX(e, 3);
  E = mprop(propno, 1);
  A = mprop(propno, 2);
  
  % element stiffness matrix
  k_0e = E * A * L0 * B0 * B0'; % 4x4 matrix
  
  % vector of index used for building K
  [edof] = build_edof(IX, e);
  
  % build K by summing k_e
  for ii = 1:4
    for jj = 1:4
      K(edof(ii), edof(jj)) = K(edof(ii), edof(jj)) + rho(e)^p*k_0e(ii, jj);
    end
  end

end

return

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Enforce boundary conditions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [K,P]=enforce(K,P,bound);

% This subroutine enforces the support boundary conditions

% 0-1 METHOD
for i=1:size(bound,1)
    node = bound(i, 1); % number of node
    ldof = bound(i, 2); % direction
    pos = 2*node - (2 - ldof);
    K(:, pos) = 0; % putting 0 on the columns
    K(pos, :) = 0; % putting 0 on the rows
    K(pos, pos) = 1; % putting 1 in the dyagonal

    P(pos) = 0; % putting 0 in P
end

return
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%% Calculate element strain and stress %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
% function [strain,stress]=recover(mprop,X,IX,D,ne,strain,stress);
% 
% % This subroutine recovers the element stress, element strain, 
% % and nodal reaction forces
% 
% % allocate memory for stress and strain vectors
% strain = zeros(ne, 1); 
% stress = zeros(ne, 1);
% 
% for e=1:ne
%   d = zeros(4, 1); % allocate memory for element stiffness matrix
% 
%  [edof] = build_edof(IX, e); % index for buildg K
% 
%   % build the matrix d from D
%   for i = 1:4
%     d(i) = D(edof(i));
%   end
% 
%   % compute the bar length
%   [L0, delta_x, delta_y] = length(IX, X, e);
%   
%   % displacement vector
%   B0 = 1/L0^2 * [-delta_x -delta_y delta_x delta_y]';  
%   
%   % materials properties
%   propno = IX(e, 3);
%   E = mprop(propno, 1);
%   A = mprop(propno, 2);
% 
%   strain(e) = B0' * d; 
%   stress(e) = strain(e) * E;
% 
% end
% 
% 
% return

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

%%% Calculate element strain and stress %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [strain,stress, N, R, f_grad, g_grad, f]=recover(mprop,X,IX,D,ne,strain,stress,P,p,rho)

% This subroutine recovers the element stress, element strain, 
% and nodal reaction forces
strain = zeros(ne, 1);
stress = zeros(ne, 1);
B0_sum = zeros(2*size(X,1), 1);
g_grad = zeros(ne, 1); % vector of volumes IT IS THE SAME AS v
f_grad = zeros(ne,1); % gradient of f
f = 0; % compliance

for e=1:ne
  d = zeros(4, 1);
  [edof] = build_edof(IX, e);

  % build the matrix d from D
  for i = 1:4
    d(i) = D(edof(i));
  end  
  
  % compute the bar length
  [L0, delta_x, delta_y] = length(IX, X, e);
  % displacement vector (4x1)
  B0 = 1/L0^2 * [-delta_x -delta_y delta_x delta_y]';
  
  % materials properties
  propno = IX(e, 3);
  E0 = mprop(propno, 1);
  A = mprop(propno, 2);
  
  % element stiffness matrix
  k_0e = E0 * A * L0 * B0 * B0'; % 4x4 matrix

  % compute the volume
  g_grad(e) = L0 * A;

  strain(e) = B0' * d; 
  stress(e) = rho(e)^p*strain(e) * E0;
  N(e) = stress(e) * A;
  
  % sum B0 after having transformed it in order to be compliant for the sum
  % with P
  for jj = 1:4
      B0_sum(edof(jj)) = B0_sum(edof(jj)) + B0(jj)*N(e)*L0;
  end
  
  
  f_grad(e) = -p*rho(e)^(p - 1)*d'*k_0e*d; % ATTENTION TO DIMENSION OF d

  f = f + d' * rho(e)^p * k_0e * d; % compute the compliance
end

% compute the support reactions (N)
R = B0_sum - P; % 2nnx1 (nn is node number)

return

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Plot structure %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function PlotStructure(X,IX,ne,neqn,bound,loads,D,stress,rho)

% This subroutine plots the undeformed and deformed structure

h1=0;h2=0;
% Plotting Un-Deformed and Deformed Structure
clf
hold on
box on

colors = ['b', 'r', 'g']; % vector of colors for the structure
fake_zero = 1e-8; % fake zero for tension sign decision

for e = 1:ne
    xx = X(IX(e,1:2),1); % vector of x-coords of the nodes
    yy = X(IX(e,1:2),2); % vector of y-coords of the nodes 
    h1=plot(xx,yy,'k:','LineWidth',1.); % Un-deformed structure
    [edof] = build_edof(IX, e);

    xx = xx + D(edof(1:2:4));
    yy = yy + D(edof(2:2:4));
    
    % choice of thhe color according to the state
    if stress(e) > fake_zero  % tension
      col = colors(1);
    elseif stress(e) < - fake_zero  % compression
      col = colors(2);
    else col = colors(3);  % un-loaded
    end 
    
    thick = 3.5*rho(e);

    h2=plot(xx,yy, col,'LineWidth',thick); % Deformed structure
end
plotsupports
plotloads

legend([h1 h2],{'Undeformed state',...
                'Deformed state'})

axis equal;
hold off

return

%% Function to compute the length of a bar
function [L0, delta_x, delta_y] = length(IX, X, e)

  i = IX(e, 1);
  j = IX(e, 2);
  xi = X(i, 1);
  xj = X(j, 1);
  yi = X(i, 2);
  yj = X(j, 2);
  delta_x = xj - xi;
  delta_y = yj - yi;
  L0 = sqrt(delta_x^2 + delta_y^2);

return

%% Function to provide the edof vector
function [edof] = build_edof(IX, e)
  edof = zeros(4, 1);
  edof = [IX(e,1)*2-1 IX(e,1)*2 IX(e,2)*2-1 IX(e,2)*2];
return

