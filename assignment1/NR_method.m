%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%                        Euler method with one step correction                              %
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

function fea()

close all
clc

%--- Input file ----------------------------------------------------------%
%example1                % Input file
%test1                   % Input file
TrussExercise2_2022             % Input file

neqn = size(X,1)*size(X,2);         % Number of equations
ne = size(IX,1);                    % Number of elements
disp(['Number of DOF ' sprintf('%d',neqn) ...
    ' Number of elements ' sprintf('%d',ne)]);

%--- Initialize arrays ---------------------------------------------------%
Kmatr=sparse(neqn,neqn);                % Stiffness matrix
K=sparse(neqn,neqn);                % Stiffness matrix
P_final=zeros(neqn,1);                  % Force vector
P=zeros(neqn,1);                        % Force vector
D=zeros(neqn,1);                        % Displacement vector
R=zeros(neqn,1);                        % Residual vector
strain=zeros(ne,size(incr_vector, 2));                     % Element strain vector
stress=zeros(ne,size(incr_vector, 2));                     % Element stress vector

%--- Calculate displacements ---------------------------------------------%

[P_final] = buildload(X,IX,ne,P_final,loads,mprop); % vector of the external loads
rubber_param = [mprop(3) mprop(4) mprop(5) mprop(6)];

for j = 1:size(incr_vector,2)
 
  % choose the step size
  nincr = incr_vector(j);

  % load increment
  delta_P = P_final / nincr; 
  
  % Initialize arrays
  P=zeros(neqn,1);                        % Force vector
  D0=zeros(neqn,1);                        % Displacement vector
  D=zeros(neqn,1);                        % Displacement vector

  for n = 1:nincr  % cycle to the number of increments
    P = P + delta_P;  % increment the load 
    D0 = D;
    
    for i = 1:i_max
      [~, ~, ~, R]=recover(mprop,X,IX,D0,ne,strain,stress,P,rubber_param); % compute R
      [~,R]=enforce(Kmatr,R,bound);       % Enforce boundary conditions on R
      
      if abs(R) <= eSTOP * abs(P) % break when we respect the eSTOP
        break
      end
  
      [K, epsilon]=buildstiff(X,IX,ne,mprop,Kmatr,D0,rubber_param);    % Build global tangent stiffness matrix
  %     [LM, UM] = lu(K);
  %     D0 = UM \ (LM\P);
      [K, ~] = enforce(K,R,bound);
  
      delta_D0 = - K \ R;
      D0 = D0 + delta_D0;
   
    end
    
    D = D0;
    
    P_plot(n, j) = P(48);
    D_plot(n, j) = D(48);
    signorini_plot(n, j) = signorini(epsilon, rubber_param, 1, IX, mprop);
  
  end
  
  %[strain(:,j), stress(:,j), N(:,j), R(:,j)]=recover(mprop,X,IX,D0,ne,strain,stress,P,rubber_param); % compute the final support reaction
end

%--- Print the results on the command window -----------------------------%
% % External matrix
% disp('External forces applied (N)')
% P'

% % Stress
% disp('Stress on the bars (MPa)')
% stress'
% 
% % Strain
% disp('Strain of the bars')
% strain'
% 
% % Forces on the bars
% disp('Internal forces on the bar (N)')
% N
% 
% % Support reaction
% disp('Support reactions forces (N)')
% R'

%--- Plot results --------------------------------------------------------%                                                        
PlotStructure(X,IX,ne,neqn,bound,loads,D,stress)        % Plot structure

figure(2)
for j=1:size(incr_vector,2)
  plot(D_plot(:,j), P_plot(:,j), 'o', 'LineWidth', 2.5)
  % build a vector with the name 
  legend_name(j) = strcat("Number of increment n = ", num2str(incr_vector(j)));
  hold on
end
legend_name(size(incr_vector,2) + 1) = "Signorini";
plot(D_plot(:,end), signorini_plot(:,end))
xlabel("Displacement (m)")
ylabel("Force (N)")
legend(legend_name,'Location','southeast')
hold off
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
function [K, epsilon]=buildstiff(X,IX,ne,mprop,K,D,rubber_param);

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
  A = mprop(propno, 2);
 
  % vector of index used for building K
  [edof] = build_edof(IX, e);
  
  % build d vector
  [d] = build_d(D, edof);
  
  % compute the displacement
  epsilon = B0' * d';
  Et = Etfunction(epsilon, rubber_param);

  % element stiffness matrix
  k_e = Et * A * L0 * B0 * B0'; % 4x4 matrix
  
  % build K by summing k_e
  for ii = 1:4
    for jj = 1:4
      K(edof(ii), edof(jj)) = K(edof(ii), edof(jj)) + k_e(ii, jj);
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

% %%% Calculate element strain and stress %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
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
function [strain, stress, N, R]=recover(mprop,X,IX,D,ne,strain,stress,P,rubber_param);

% This subroutine recovers the element stress, element strain, force on each element 
% and nodal reaction forces
strain = zeros(ne, 1);
stress = zeros(ne, 1);
R_int = zeros(2*size(X,1), 1);
for e=1:ne
  d = zeros(4, 1);
  [edof] = build_edof(IX, e);

  % build the matrix d from D
  d = build_d(D, edof);

  % compute the bar length
  [L0, delta_x, delta_y] = length(IX, X, e);
  
  % displacement vector
  B0 = 1/L0^2 * [-delta_x -delta_y delta_x delta_y]';  
  
  % materials properties
  propno = IX(e, 3);
  A = mprop(propno, 2);

  strain(e) = B0' * d'; 
  stress(e) = stress_function(strain(e), rubber_param);
  N(e) = stress(e) * A;
  
  % sum B0 after having transformed it in order to be compliant for the sum
  % with P
  for jj = 1:4
      R_int(edof(jj)) = R_int(edof(jj)) + B0(jj)*N(e)*L0;
  end

end

% compute the support reactions (N)
R = R_int - P; % 2nnx1 (nn is node number)

return

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Plot structure %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function PlotStructure(X,IX,ne,neqn,bound,loads,D,stress)

% This subroutine plots the undeformed and deformed structure

h1=0;h2=0;
% Plotting Un-Deformed and Deformed Structure
figure(1)
clf
hold on
box on

colors = ['b', 'r', 'g']; % vector of colors for the structure
eSTOP = 1e-8; % fake zero for tension sign decision

for e = 1:ne
    xx = X(IX(e,1:2),1); % vector of x-coords of the nodes
    yy = X(IX(e,1:2),2); % vector of y-coords of the nodes 
    h1=plot(xx,yy,'k:','LineWidth',1.); % Un-deformed structure
    [edof] = build_edof(IX, e);

    xx = xx + D(edof(1:2:4));
    yy = yy + D(edof(2:2:4));
    
    % choice of thhe color according to the state
    if stress(e) > eSTOP  % tension
      col = colors(1);
    elseif stress(e) < - eSTOP  % compression
      col = colors(2);
    else col = colors(3);  % un-loaded
    end 

    h2=plot(xx,yy, col,'LineWidth',3.5); % Deformed structure
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

%% Function to build d from D
function [d] = build_d(D, edof)
  for i = 1:4
    d(i) = D(edof(i));
  end
return 

%% Stress
function [stress] = stress_function(epsilon, rubber_param)
  c1 = rubber_param(1);
  c2 = rubber_param(2);
  c3 = rubber_param(3);
  c4 = rubber_param(4);

  stress = c1*((1+c4*epsilon) - (1+c4*epsilon)^(-2)) + c2*(1 - (1+c4*epsilon)^(-3)) + c3 * (1 - 3*(1+c4*epsilon) + (1+c4*epsilon)^3 - 2*(1+c4*epsilon)^(-3) + 3*(1+c4*epsilon)^(-2));
return

%% Function to Et
function [Et] = Etfunction(epsilon, rubber_param)
  c1 = rubber_param(1);
  c2 = rubber_param(2);
  c3 = rubber_param(3);
  c4 = rubber_param(4);

  Et = c4*(c1*(1 + 2*(1 + c4*epsilon)^(-3)) + 3*c2*(1 + c4*epsilon)^(-4) + 3*c3*(-1 + (1 + c4*epsilon)^2 - 2*(1 + c4*epsilon)^(-3) + 2*(1 + c4*epsilon)^(-4)));
return

%% Signorini method
function [force] = signorini(epsilon, rubber_param, e, IX, mprop)
  c1 = rubber_param(1);
  c2 = rubber_param(2);
  c3 = rubber_param(3);
  c4 = rubber_param(4);

  propno = IX(e, 3);
  A = mprop(propno, 2);

  force =A *( c1*((1+c4*epsilon) - (1+c4*epsilon)^(-2)) + c2*(1 - (1+c4*epsilon)^(-3)) + c3 * (1 - 3*(1+c4*epsilon) + (1+c4*epsilon)^3 - 2*(1+c4*epsilon)^(-3) + 3*(1+c4*epsilon)^(-2)));
return