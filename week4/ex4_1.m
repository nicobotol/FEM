% Created with:       FlExtract v1.13
% Element type:       truss
% Number of nodes:    6
% Number of elements: 11

clear all;
V_star = 10; % boundary on the load
rho_min = 1e-6; % minimum density
Pload = 0.01; % load
iopt_max = 100;
p = 1.5;
rho_min = 1e-6;
eta = 0.3;
eta_vector = [0.5 0.6 0.7];
epsilon = 1e-5; % convergence interval

% Node coordinates: x, y
X = [
0	0
0	2
2	0
2	2
4	0
4	2
];
% Element connectivity: node1_id, node2_id, material_id
IX = [
2	1	1
4	1	1
3	1	1
4	2	1
3	2	1
4	3	1
6	3	1
5	3	1
6	4	1
5	4	1
6	5	1
];
% Element properties: Young's modulus, area
mprop = [
1	1
];
% Nodal diplacements: node_id, degree of freedom (1 - x, 2 - y), displacement
bound = [
1	1	0
1	2	0
2	1	0
];
% Nodal loads: node_id, degree of freedom (1 - x, 2 - y), load
loads = [
5	2	-Pload
];
% Control parameters
plotdof = 2;
