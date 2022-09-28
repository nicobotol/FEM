% File test1.m, DAY1, modified 3/9-2006 by OS
%
% Example: 3-bar truss
% No. of Nodes: 3  
% No. of Elements : 3
clear all
clf

A = 2;  % cross section
P = 200;  % externla load
n_incr = 20;  % number of increment
i_max = 100; % maximum number of iterations
tollerance = 1e-8; % tollerance to stop the integration

% rubber parameters
c1 = 1;
c2 = 50;
c3 = 0.1;
c4 = 100;
rubber_param = [c1 c2 c3 c4];
% Coordinates of 3 nodes,
X = [  0.00  0.00 
       0.00  1.00 
       1.00  0.30 ];

% Topology matrix IX(node1,node2,propno),
IX = [ 1  3  1 
       1  2  2
       2  3  1 ];
      
% Element property matrix mprop = [ E A ],
mprop = [ 1.0 1.0
          1.0 1.0 ];

% Prescribed loads mat(node,dof,force)
loads = [ 3   2 -0.01 ];

% Boundary conditions mat(node,dof,disp)   
bound = [ 1  1  0.0
          1  2  0.0
          2  1  0.0 ];

% Control Parameters
plotdof = 6;
