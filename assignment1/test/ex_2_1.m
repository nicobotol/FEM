% Created with:       FlExtract v1.13
% Element type:       truss
% Number of nodes:    3
% Number of elements: 2

clear all;

A = 2;  % cross section
P = 200;  % externla load
incr_vector = [20];  % number of increment
i_max = 100; % maximum number of iterations
tollerance = 1e-8; % tollerance to stop the integration
%nincr = 5;
% rubber parameters
c1 = 1;
c2 = 50;
c3 = 0.1;
c4 = 100;
rubber_param = [c1 c2 c3 c4];
eSTOP = 1e-8;
Pfinal = 200;

% Node coordinates: x, y
X = [
0	0
1.5	0
3	0
];
% Element connectivity: node1_id, node2_id, material_id
IX = [
2	1	1
3	2	1
];
% Element properties: Young's modulus, area
mprop = [
1 A 1 50 0.1 100
%1	A 1.2 5 0.2 50
];
% Nodal diplacements: node_id, degree of freedom (1 - x, 2 - y), displacement
bound = [
1	1	0
1	2	0
2	2	0
3	2	0
];
% Nodal loads: node_id, degree of freedom (1 - x, 2 - y), load
loads = [
3	1	P
];
% Control parameters
plotdof = 3;
