% Created with:       FlExtract v1.13
% Element type:       truss
% Number of nodes:    9
% Number of elements: 13

clear all;

% space beteen nodes 1 and 2
a = 0.05; 

%constraints on the frame
x2 = 0.5; 
x5 = 0.75;
x6 = 1.5;
%nodes x coordinates
x1 = x2 - a;
x3 = (x1 + x2) / 2;
x4 = (x3 + x5) / 2;
x9 = x6 + a;
x7 = (x6 + x9) / 2;
%x8 = (x5 + x7) / 2;
x8 = x4 + (x7 - x3)*0.5;
% Node coordinates: x, y
X = [
x1	0
x2	0
x3	0.5
x4	1
x5	0.5
x6	0
x7	0.5
x8	1
x9	0
];
% Element connectivity: node1_id, node2_id, material_id
IX = [
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
4 8 1
];
% Element properties: Young's modulus, area
mprop = [
1	1
];
% Nodal diplacements: node_id, degree of freedom (1 - x, 2 - y), displacement
bound = [
1	1	0
1	2	0
2	2	0
6	2	0
9	2	0
];
% Nodal loads: node_id, degree of freedom (1 - x, 2 - y), load
loads = [
5 2 -0.01
];
% Control parameters
plotdof = 3;
