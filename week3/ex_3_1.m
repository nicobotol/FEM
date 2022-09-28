% Created with:       FlExtract v1.13
% Element type:       truss
% Number of nodes:    3
% Number of elements: 2

clear all;

A = 2;  % cross section
P = 200;  % externla load
incr_vector = [50];  % number of increment
i_max = 1000; % maximum number of iterations
n_incr = 10; 
eSTOP = 1e-8;
Pfinal = 0.03;
a = 0.4;
k = 0.02; % spring stiffness

% Node coordinates: x, y
X = [
0	0
1.5	-a
3	0
];
% Element connectivity: node1_id, node2_id, material_id
IX = [
2	1	1
3	2	1
];
% Element properties: Young's modulus, area
mprop = [
1	A 
];
% Nodal diplacements: node_id, degree of freedom (1 - x, 2 - y), displacement
bound = [
1	1	0
1	2	0
3	1	0
3	2	0
];
% Nodal loads: node_id, degree of freedom (1 - x, 2 - y), load
loads = [
2	2	Pfinal
];
% Control parameters
plotdof = 3;
