% Created with:       FlExtract v1.13
% Element type:       truss
% Number of nodes:    71
% Number of elements: 214

%clear all

eta = 0.5;
p = 1.5;
V_star = 6;
iopt_max = 100;
rho_min = 1e-6;
epsilon = 1e-5;

% Node coordinates: x, y
X = [
0	0
0	0.25
0	0.5
0	0.75
0	1
0.125	0
0.125	0.25
0.125	0.5
0.125	0.75
0.125	1
0.25	0
0.25	0.25
0.25	0.5
0.25	0.75
0.25	1
0.375	0
0.375	0.25
0.375	0.5
0.375	0.75
0.375	1
0.5	0
0.5	0.25
0.5	0.5
0.5	0.75
0.5	1
0.625	0.5
0.625	0.75
0.625	1
0.75	0.5
0.75	0.75
0.75	1
0.875	0.5
0.875	0.75
0.875	1
1	0.5
1	0.75
1	1
1.125	0.5
1.125	0.75
1.125	1
1.25	0.5
1.25	0.75
1.25	1
1.375	0.5
1.375	0.75
1.375	1
1.5	0
1.5	0.25
1.5	0.5
1.5	0.75
1.5	1
1.625	0
1.625	0.25
1.625	0.5
1.625	0.75
1.625	1
1.75	0
1.75	0.25
1.75	0.5
1.75	0.75
1.75	1
1.875	0
1.875	0.25
1.875	0.5
1.875	0.75
1.875	1
2	0
2	0.25
2	0.5
2	0.75
2	1
];
% Element connectivity: node1_id, node2_id, material_id
IX = [
2	1	1
7	1	1
6	1	1
3	2	1
8	2	1
7	2	1
6	2	1
4	3	1
9	3	1
8	3	1
7	3	1
5	4	1
10	4	1
9	4	1
8	4	1
10	5	1
9	5	1
7	6	1
12	6	1
11	6	1
8	7	1
13	7	1
12	7	1
11	7	1
9	8	1
14	8	1
13	8	1
12	8	1
10	9	1
15	9	1
14	9	1
13	9	1
15	10	1
14	10	1
12	11	1
17	11	1
16	11	1
13	12	1
18	12	1
17	12	1
16	12	1
14	13	1
19	13	1
18	13	1
17	13	1
15	14	1
20	14	1
19	14	1
18	14	1
20	15	1
19	15	1
17	16	1
22	16	1
21	16	1
18	17	1
23	17	1
22	17	1
21	17	1
19	18	1
24	18	1
23	18	1
22	18	1
20	19	1
25	19	1
24	19	1
23	19	1
25	20	1
24	20	1
22	21	1
23	22	1
24	23	1
27	23	1
26	23	1
25	24	1
28	24	1
27	24	1
26	24	1
28	25	1
27	25	1
27	26	1
30	26	1
29	26	1
28	27	1
31	27	1
30	27	1
29	27	1
31	28	1
30	28	1
30	29	1
33	29	1
32	29	1
31	30	1
34	30	1
33	30	1
32	30	1
34	31	1
33	31	1
33	32	1
36	32	1
35	32	1
34	33	1
37	33	1
36	33	1
35	33	1
37	34	1
36	34	1
36	35	1
39	35	1
38	35	1
37	36	1
40	36	1
39	36	1
38	36	1
40	37	1
39	37	1
39	38	1
42	38	1
41	38	1
40	39	1
43	39	1
42	39	1
41	39	1
43	40	1
42	40	1
42	41	1
45	41	1
44	41	1
43	42	1
46	42	1
45	42	1
44	42	1
46	43	1
45	43	1
45	44	1
50	44	1
49	44	1
46	45	1
51	45	1
50	45	1
49	45	1
51	46	1
50	46	1
48	47	1
53	47	1
52	47	1
49	48	1
54	48	1
53	48	1
52	48	1
50	49	1
55	49	1
54	49	1
53	49	1
51	50	1
56	50	1
55	50	1
54	50	1
56	51	1
55	51	1
53	52	1
58	52	1
57	52	1
54	53	1
59	53	1
58	53	1
57	53	1
55	54	1
60	54	1
59	54	1
58	54	1
56	55	1
61	55	1
60	55	1
59	55	1
61	56	1
60	56	1
58	57	1
63	57	1
62	57	1
59	58	1
64	58	1
63	58	1
62	58	1
60	59	1
65	59	1
64	59	1
63	59	1
61	60	1
66	60	1
65	60	1
64	60	1
66	61	1
65	61	1
63	62	1
68	62	1
67	62	1
64	63	1
69	63	1
68	63	1
67	63	1
65	64	1
70	64	1
69	64	1
68	64	1
66	65	1
71	65	1
70	65	1
69	65	1
71	66	1
70	66	1
68	67	1
69	68	1
70	69	1
71	70	1
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
6	1	0
6	2	0
11	1	0
11	2	0
16	1	0
16	2	0
21	1	0
21	2	0
47	1	0
47	2	0
52	1	0
52	2	0
57	1	0
57	2	0
62	1	0
62	2	0
67	1	0
67	2	0
];
% Nodal loads: node_id, degree of freedom (1 - x, 2 - y), load
loads = [
29	2	-0.01
];
% Control parameters
plotdof = 2;
