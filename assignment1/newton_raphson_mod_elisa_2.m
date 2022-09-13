%% Main resolution loop 

TrussExercise2_2022
c1 = 1.2;
c2 = 5;
c3 = 0.2;
c4 = 50;
% Input file
mcst = [c1, c2, c3, c4] ;
n_incr = incr_vector(1);

% Initialisation 
neqn = size(X,1)*size(X,2);         % Number of equations
ne = size(IX,1);                    % Number of elements
disp(['Number of DOF ' sprintf('%d',neqn) ...
    ' Number of elements ' sprintf('%d',ne)]);


% Initialisation of resolution loop 
Pfinal = buildload(X,IX,neqn,loads ,mprop) ; % vector 
pfinal = loads(3) ;                          % scalar 
p = 0 ;
dp = pfinal/n_incr ;
e_stop = 10^(-10) ;

P = zeros(neqn, 1)
D = zeros(neqn, 1) ;
R = Pfinal ;         %% Initial (internal) R equal to Pfinal load vector

% Increments displacement/force for plotting
D_incr = [];
P_incr = [];


% Incremental load vector
incr_loads = loads;
incr_loads(3) = dp;



for n = 1:n_incr
    
    % Increments for plotting 
    D_incr(n) = D(5);
    P_incr(n) = p;
    
    
    % Force vector / Incremental force vector
    p = p+dp ;  % scalar load 
    [dP] = buildload(X,IX,neqn, incr_loads, mprop);
    P = P + dP; % global load vector

    Di = D;
    Kt = buildstiff(X,IX, mprop, Di, mcst) ;
    [Kt,R] = enforce(Kt,R,bound);

    % Factorization: it is used to speed up the process
    [LM,UM] = lu(Kt);
    
    % Equilibrium iterations :
    for i = 1:i_max 
        Rint = build_residual(X,IX, mprop, Di, mcst) ;
        R = Rint - P;
        [Kt,R] = enforce(Kt,R,bound);
        if norm(R) <= e_stop * norm(Pfinal) 
           break
        end
        dDi = - (UM \ (LM \ R));
        %dDi = - (Kt \ R) ; 
        Di = Di + dDi ;
    end 
    D = Di ;
end      


PlotStructure(X,IX,ne,neqn,bound,loads,D) ;











%% Functions 

%%% Build global load vector %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [P]=buildload(X,IX,neqn, loads,mprop)
P=zeros(neqn,1)
for i = 1:size(loads,1) % for every applied load
    node = loads(1);    
    dir = loads(2);
    j = node*2;
    if dir == 1
        j = j-1;
    end 
    P(j,1) = loads(3);
end
end





%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Build global stiffness matrix %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [Kt]=buildstiff(X,IX,mprop,D, mcst)

neqn = size(X,1)*size(X,2);         % Number of equations
ne = size(IX,1);                    % Number of elements

Kt=zeros(neqn, neqn)
Ke={}; % Ke will contain all the Ke local stiffness matrices



% this cylce will build the Ke matrix
for e=1:ne

    i = IX(e,1); % first node
    j = IX(e,2); % second node
    xi = X(i,1); 
    xj = X(j,1); 
    yi = X(i,2); 
    yj = X(j,2); 
    delta_x = xj -xi;
    delta_y = yj - yi;
    L0 = sqrt(delta_x^2 + delta_y^2);
   
    
     % material properties
    propno = IX(e,3); % property number
    A = mprop(propno,2);

    % B0 vector 4x1
    B0 = 1/L0^2 * [-delta_x -delta_y delta_x delta_y]';
    
    % d vector 4x1
    d=[] ;
    edof = [i*2-1, i*2, j*2-1, j*2]; % remapping of positions the Ke elements
    for k=1:4
        d(k)=D(edof(k));
    end 
    d=d';
    
    
    % Strain, stress, Et
    eps=B0'*d
    c1=mcst(1) ;
    c2=mcst(2) ;
    c3=mcst(3) ;
    c4=mcst(4) ;
    [sigma,Et] = tang_stiff(c1,c2, c3, c4, eps) ;

   
    
    % calulation of Ke for each element 4x4
    Ke{e} = Et * A * L0 * B0 * B0';        
end



% Assembly all the Ke matrices into the general stiffness matrix
for e = 1:ne
    i = IX(e,1); % first node
    j = IX(e,2); % second node
    edof = [i*2-1, i*2, j*2-1, j*2]; % remapping of positions the Ke elements
    for l = 1:4
        for m = 1:4
            Kt(edof(l),edof(m)) = Ke{e}(l,m) + Kt(edof(l),edof(m)); % assembly/sum
        end
    end   
end
end 


% Function for residuals
function [Rint]=build_residual(X,IX, mprop, D, mcst)

neqn = size(X,1)*size(X,2);         % Number of equations
ne = size(IX,1);                    % Number of elements

Rint = zeros(neqn, 1) ;

% this cylce will build the Ke matrix
for e=1:ne

    i = IX(e,1); % first node
    j = IX(e,2); % second node
    xi = X(i,1); 
    xj = X(j,1); 
    yi = X(i,2); 
    yj = X(j,2); 
    delta_x = xj -xi;
    delta_y = yj - yi;
    L0 = sqrt(delta_x^2 + delta_y^2);
   
    
     % material properties
    propno = IX(e,3); % property number
    A = mprop(propno,2);

    % B0 vector 4x1
    B0e = 1/L0^2 * [-delta_x -delta_y delta_x delta_y]';
    
    % d vector 4x1
    d=[] ;
    edof = [i*2-1, i*2, j*2-1, j*2]; % remapping of positions the Ke elements
    for k=1:4
        d(k)=D(edof(k));
    end 
    d=d';
    
    
    % Strain, stress, Et
    eps=B0e'*d
    c1=mcst(1) ;
    c2=mcst(2) ;
    c3=mcst(3) ;
    c4=mcst(4) ;
    [sigma,Et] = tang_stiff(c1,c2, c3, c4, eps) ;
    
    
    % Calculate global B0 as sum of the B0e on the elements
    for l = 1:4
        Rint(edof(l)) = B0e(l)*A*sigma*L0 + Rint(edof(l)); % assembly/sum 
    end   
end
end



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%% Enforce boundary conditions %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function [K,P]=enforce(K,P,bound);

% This subroutine enforces the support boundary conditions
% Method of 0's and 1's

for i = 1:size(bound,1) % cycle on the different bounds
    node = bound(i,1); % number of bounded node
    dir = bound(i,2); % bonded direction
    l = node*2;
    if  dir == 1
        l = l-1;
    end 
        
    for j = 1:size(K,1) 
        K(l,j) = 0; % putting 0 on the rows
        K(j,l) = 0; % putting 0 on the columns
        K(l,l) = 1; % putting 1 in the diagonal
        P(l) = bound(i,3); % putting the bond condition in the load vector
    end    
end 
end 




%% Plot structure 

%%% Plot structure %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
function PlotStructure(X,IX,ne,neqn,bound,loads,D)

% This subroutine plots the undeformed and deformed structure

h1=0;h2=0;
% Plotting Un-Deformed and Deformed Structure
clf
hold on
box on
for e = 1:ne
    xx = X(IX(e,1:2),1);
    yy = X(IX(e,1:2),2);
    h1=plot(xx,yy,'k:','LineWidth',1.);
    edof = [2*IX(e,1)-1 2*IX(e,1) 2*IX(e,2)-1 2*IX(e,2)];
    xx = xx + D(edof(1:2:4));
    yy = yy + D(edof(2:2:4));
    
    
    h2=plot(xx,yy,'b','LineWidth',3.5);
    
%     if stress(e)>1e-10
%       h2=plot(xx,yy,'b','LineWidth',3.5);
%     elseif stress(e)<-1e-10
%       h2=plot(xx,yy,'r','LineWidth',3.5);
%     else 
%       h2=plot(xx,yy,'g','LineWidth',3.5);
%     end 


        
end
plotsupports
plotloads

% legend([h1 h2],{'Undeformed state',...
%                 'Deformed state'})

axis equal;
hold off

end 






%% Other functions 



function [sigma, Et] = tang_stiff(c1,c2, c3, c4, eps)

lambda=1+c4*eps; 
if lambda>0
    sigma = c1* (lambda-lambda^(-2)) + c2*(1 - lambda^(-3)) + c3*(1 - 3*lambda + lambda^3 - 2*lambda^(-3) + 3*lambda^(-2)) ;
    Et = c4*( c1*(1 + 2*lambda^(-3)) + 3*c2*lambda^(-4) +3*c3*(-1 + lambda^2 - 2*lambda^(-3) + 2*lambda^(-4))) ;
else 
    disp('lambda <= 0 : strain is too low')
end 
end 




