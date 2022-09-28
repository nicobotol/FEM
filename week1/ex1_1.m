% Created with:       FlExtract v1.13
% Element type:       truss
% Number of nodes:    6
% Number of elements: 9

clear all;

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
3	1	1
4	2	1
3	2	1
4	3	1
6	3	1
5	3	1
6	4	1
6	5	1
];
% Element properties: Young's modulus, area
mprop = [
1	1
2	2
];
% Nodal diplacements: node_id, degree of freedom (1 - x, 2 - y), displacement
bound = [
1	1	0
1	2	0
2	1	0
];
% Nodal loads: node_id, degree of freedom (1 - x, 2 - y), load
loads = [
5	2	-1
];
% Control parameters
plotdof = 2;
