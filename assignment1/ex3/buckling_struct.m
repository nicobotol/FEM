% Created with:       FlExtract v1.13
% Element type:       truss
% Number of nodes:    34
% Number of elements: 81

clear all;

A = 2;  % cross section
%incr_vector = [100];  % number of increment
i_max = 1000; % maximum number of iterations
nincr = 10; 
eSTOP = 1e-8;
Pfinal = 0.006;

% Node coordinates: x, y
X = [
0	0
0	5
5	0
5	5
10	0
10	5
15	0
15	5
20	0
20	5
25	0
25	5
30	0
30	5
35	0
35	5
40	0
40	5
45	0
45	5
50	0
50	5
55	0
55	5
60	0
60	5
65	0
65	5
70	0
70	5
75	0
75	5
80	0
80	5
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
8	5	1
7	5	1
8	6	1
7	6	1
8	7	1
10	7	1
9	7	1
10	8	1
9	8	1
10	9	1
12	9	1
11	9	1
12	10	1
11	10	1
12	11	1
14	11	1
13	11	1
14	12	1
13	12	1
14	13	1
16	13	1
15	13	1
16	14	1
15	14	1
16	15	1
18	15	1
17	15	1
18	16	1
17	16	1
18	17	1
20	17	1
19	17	1
20	18	1
19	18	1
20	19	1
22	19	1
21	19	1
22	20	1
21	20	1
22	21	1
24	21	1
23	21	1
24	22	1
23	22	1
24	23	1
26	23	1
25	23	1
26	24	1
25	24	1
26	25	1
28	25	1
27	25	1
28	26	1
27	26	1
28	27	1
30	27	1
29	27	1
30	28	1
29	28	1
30	29	1
32	29	1
31	29	1
32	30	1
31	30	1
32	31	1
34	31	1
33	31	1
34	32	1
33	32	1
34	33	1
];
% Element properties: Young's modulus, area
mprop = [
0.8	0.2
];
% Nodal diplacements: node_id, degree of freedom (1 - x, 2 - y), displacement
bound = [
1	2	0
2 2 0
33	2	0
34 2 0
1 1 0
];
% Nodal loads: node_id, degree of freedom (1 - x, 2 - y), load
loads = [
1 1 Pfinal*(0.5+1e-3)
33	1	-Pfinal*(0.5+1e-3)
2 1 Pfinal/2
34	1	-Pfinal/2
];
% Control parameters
plotdof = 5;
