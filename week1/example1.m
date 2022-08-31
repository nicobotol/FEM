% File example1.m, DAY1, modified 3/9 by OS
%
% Example: 3-bar truss
% No. of Nodes: 3  
% No. of Elements : 3
clear all

% Coordinates of 3 nodes,
X = [  0.00  0.00 
       0.00  1.00 
       1.00  0.00 ];

% Topology matrix IX(node1,node2,propno),
IX = [ 1  3  1 
       1  2  1
       2  3  1 ];
      
% Element property matrix mprop = [ E A ],
mprop = [ 1.0 1.0 ];

% Prescribed loads mat(node,ldof,force)
loads = [ 3   2 -0.01 ];

% Boundary conditions mat(node,ldof,disp)   
bound = [ 1  1  0.0
          1  2  0.0
          2  1  0.0 ];

% Control Parameters
plotdof = 6;

