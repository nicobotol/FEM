% Created with:       FlExtract v1.13
% Element type:       truss
% Number of nodes:    10
% Number of elements: 16

clear all;

% Node coordinates: x, y
X = [
0	0.5
0	0
0.5	0.5
0.625	0.78
0.75	0.5
2	0
1.5	0.5
1.125	0.78
2	0.5
];
% Element connectivity: node1_id, node2_id, material_id
IX = [
4	1	1
3	1	1
2	1	1
3	2	1
4	3	1
5	3	1
5	4	1
8	5	1
7	5	1
7	6	1
9	6	1
8	7	1
9	7	1
9	8	1
4 8 1
];
% Element properties: Young's modulus, area
mprop = [
1	1
2	2
];
% Nodal diplacements: node_id, degree of freedom (1 - x, 2 - y), displacement
bound = [
2	1	0
2	2	0
6	2	0
];
% Nodal loads: node_id, degree of freedom (1 - x, 2 - y), load
loads = [
  5, 2, -0.01
];
% Control parameters
plotdof = 3;
