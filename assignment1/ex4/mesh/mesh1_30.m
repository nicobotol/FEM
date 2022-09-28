% Created with:       FlExtract v1.13
% Element type:       truss
% Number of nodes:    24
% Number of elements: 75

%clear all

eta = 0.53;
p = 1.5;
V_star = 6;
iopt_max = 100;
rho_min = 1e-6;
epsilon = 1e-5;
P = 0.01;

% Node coordinates: x, y
X = [
0	0
0	0.5
0	1
0.25	0
0.25	0.5
0.25	1
0.5	0
0.5	0.5
0.5	1
0.75	0.5
0.75	1
1	0.5
1	1
1.25	0.5
1.25	1
1.5	0
1.5	0.5
1.5	1
1.75	0
1.75	0.5
1.75	1
2	0
2	0.5
2	1
];
% Element connectivity: node1_id, node2_id, material_id
IX = [
2	1	1
4	1	1
5	1	1
7	1	1
3	2	1
4	2	1
5	2	1
6	2	1
8	2	1
5	3	1
6	3	1
9	3	1
5	4	1
7	4	1
8	4	1
6	5	1
7	5	1
8	5	1
9	5	1
10	5	1
8	6	1
9	6	1
11	6	1
8	7	1
9	8	1
10	8	1
11	8	1
12	8	1
10	9	1
11	9	1
13	9	1
11	10	1
12	10	1
13	10	1
14	10	1
12	11	1
13	11	1
15	11	1
13	12	1
14	12	1
15	12	1
17	12	1
14	13	1
15	13	1
18	13	1
15	14	1
17	14	1
18	14	1
20	14	1
17	15	1
18	15	1
21	15	1
17	16	1
19	16	1
20	16	1
22	16	1
18	17	1
19	17	1
20	17	1
21	17	1
23	17	1
20	18	1
21	18	1
24	18	1
20	19	1
22	19	1
23	19	1
21	20	1
22	20	1
23	20	1
24	20	1
23	21	1
24	21	1
23	22	1
24	23	1
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
4	1	0
4	2	0
7	1	0
7	2	0
16	1	0
16	2	0
19	1	0
19	2	0
22	1	0
22	2	0
];
% Nodal loads: node_id, degree of freedom (1 - x, 2 - y), load
loads = [
10	2	-P
];
% Control parameters
plotdof = 2;
