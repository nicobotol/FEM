% Created with:       FlExtract v1.13
% Element type:       truss
% Number of nodes:    71
% Number of elements: 1140

%clear all

eta = 0.53;
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
3	1	1
4	1	1
6	1	1
7	1	1
8	1	1
9	1	1
11	1	1
12	1	1
13	1	1
14	1	1
16	1	1
17	1	1
18	1	1
19	1	1
21	1	1
22	1	1
23	1	1
3	2	1
4	2	1
5	2	1
6	2	1
7	2	1
8	2	1
9	2	1
10	2	1
11	2	1
12	2	1
13	2	1
14	2	1
15	2	1
16	2	1
17	2	1
18	2	1
19	2	1
20	2	1
21	2	1
22	2	1
23	2	1
24	2	1
27	2	1
4	3	1
5	3	1
6	3	1
7	3	1
8	3	1
9	3	1
10	3	1
11	3	1
12	3	1
13	3	1
14	3	1
15	3	1
16	3	1
17	3	1
18	3	1
19	3	1
20	3	1
21	3	1
22	3	1
23	3	1
24	3	1
25	3	1
26	3	1
27	3	1
28	3	1
29	3	1
30	3	1
32	3	1
5	4	1
6	4	1
7	4	1
8	4	1
9	4	1
10	4	1
11	4	1
12	4	1
13	4	1
14	4	1
15	4	1
16	4	1
17	4	1
18	4	1
19	4	1
20	4	1
22	4	1
23	4	1
24	4	1
25	4	1
26	4	1
27	4	1
28	4	1
29	4	1
30	4	1
31	4	1
33	4	1
7	5	1
8	5	1
9	5	1
10	5	1
12	5	1
13	5	1
14	5	1
15	5	1
17	5	1
18	5	1
19	5	1
20	5	1
23	5	1
24	5	1
25	5	1
26	5	1
27	5	1
28	5	1
30	5	1
31	5	1
34	5	1
7	6	1
8	6	1
9	6	1
11	6	1
12	6	1
13	6	1
14	6	1
16	6	1
17	6	1
18	6	1
19	6	1
21	6	1
22	6	1
23	6	1
24	6	1
8	7	1
9	7	1
10	7	1
11	7	1
12	7	1
13	7	1
14	7	1
15	7	1
16	7	1
17	7	1
18	7	1
19	7	1
20	7	1
21	7	1
22	7	1
23	7	1
24	7	1
25	7	1
27	7	1
30	7	1
9	8	1
10	8	1
11	8	1
12	8	1
13	8	1
14	8	1
15	8	1
16	8	1
17	8	1
18	8	1
19	8	1
20	8	1
21	8	1
22	8	1
23	8	1
24	8	1
25	8	1
26	8	1
27	8	1
28	8	1
29	8	1
30	8	1
31	8	1
32	8	1
33	8	1
35	8	1
10	9	1
11	9	1
12	9	1
13	9	1
14	9	1
15	9	1
16	9	1
17	9	1
18	9	1
19	9	1
20	9	1
21	9	1
22	9	1
23	9	1
24	9	1
25	9	1
26	9	1
27	9	1
28	9	1
29	9	1
30	9	1
31	9	1
32	9	1
33	9	1
34	9	1
36	9	1
12	10	1
13	10	1
14	10	1
15	10	1
17	10	1
18	10	1
19	10	1
20	10	1
22	10	1
23	10	1
24	10	1
25	10	1
26	10	1
27	10	1
28	10	1
29	10	1
30	10	1
31	10	1
33	10	1
34	10	1
37	10	1
12	11	1
13	11	1
14	11	1
16	11	1
17	11	1
18	11	1
19	11	1
21	11	1
22	11	1
23	11	1
24	11	1
27	11	1
13	12	1
14	12	1
15	12	1
16	12	1
17	12	1
18	12	1
19	12	1
20	12	1
21	12	1
22	12	1
23	12	1
24	12	1
25	12	1
27	12	1
28	12	1
30	12	1
14	13	1
15	13	1
16	13	1
17	13	1
18	13	1
19	13	1
20	13	1
21	13	1
22	13	1
23	13	1
24	13	1
25	13	1
26	13	1
27	13	1
28	13	1
29	13	1
30	13	1
31	13	1
32	13	1
33	13	1
34	13	1
35	13	1
36	13	1
38	13	1
15	14	1
16	14	1
17	14	1
18	14	1
19	14	1
20	14	1
21	14	1
22	14	1
23	14	1
24	14	1
25	14	1
26	14	1
27	14	1
28	14	1
29	14	1
30	14	1
31	14	1
32	14	1
33	14	1
34	14	1
35	14	1
36	14	1
37	14	1
39	14	1
17	15	1
18	15	1
19	15	1
20	15	1
22	15	1
23	15	1
24	15	1
25	15	1
26	15	1
27	15	1
28	15	1
29	15	1
30	15	1
31	15	1
32	15	1
33	15	1
34	15	1
36	15	1
37	15	1
40	15	1
17	16	1
18	16	1
19	16	1
21	16	1
22	16	1
23	16	1
24	16	1
18	17	1
19	17	1
20	17	1
21	17	1
22	17	1
23	17	1
24	17	1
25	17	1
27	17	1
28	17	1
31	17	1
19	18	1
20	18	1
21	18	1
22	18	1
23	18	1
24	18	1
25	18	1
26	18	1
27	18	1
28	18	1
29	18	1
30	18	1
31	18	1
32	18	1
33	18	1
34	18	1
35	18	1
36	18	1
37	18	1
38	18	1
39	18	1
41	18	1
20	19	1
21	19	1
22	19	1
23	19	1
24	19	1
25	19	1
26	19	1
27	19	1
28	19	1
29	19	1
30	19	1
31	19	1
32	19	1
33	19	1
34	19	1
35	19	1
36	19	1
37	19	1
38	19	1
39	19	1
40	19	1
42	19	1
22	20	1
23	20	1
24	20	1
25	20	1
26	20	1
27	20	1
28	20	1
29	20	1
30	20	1
31	20	1
32	20	1
33	20	1
34	20	1
35	20	1
36	20	1
37	20	1
39	20	1
40	20	1
43	20	1
22	21	1
23	21	1
24	21	1
23	22	1
24	22	1
25	22	1
24	23	1
25	23	1
26	23	1
27	23	1
28	23	1
29	23	1
30	23	1
31	23	1
32	23	1
33	23	1
34	23	1
35	23	1
36	23	1
37	23	1
38	23	1
39	23	1
40	23	1
41	23	1
42	23	1
44	23	1
25	24	1
26	24	1
27	24	1
28	24	1
29	24	1
30	24	1
31	24	1
32	24	1
33	24	1
34	24	1
35	24	1
36	24	1
37	24	1
38	24	1
39	24	1
40	24	1
41	24	1
42	24	1
43	24	1
45	24	1
26	25	1
27	25	1
28	25	1
29	25	1
30	25	1
31	25	1
32	25	1
33	25	1
34	25	1
35	25	1
36	25	1
37	25	1
38	25	1
39	25	1
40	25	1
42	25	1
43	25	1
46	25	1
27	26	1
28	26	1
29	26	1
30	26	1
31	26	1
32	26	1
33	26	1
34	26	1
35	26	1
36	26	1
37	26	1
38	26	1
39	26	1
40	26	1
41	26	1
42	26	1
43	26	1
44	26	1
45	26	1
49	26	1
28	27	1
29	27	1
30	27	1
31	27	1
32	27	1
33	27	1
34	27	1
35	27	1
36	27	1
37	27	1
38	27	1
39	27	1
40	27	1
41	27	1
42	27	1
43	27	1
44	27	1
45	27	1
46	27	1
50	27	1
29	28	1
30	28	1
31	28	1
32	28	1
33	28	1
34	28	1
35	28	1
36	28	1
37	28	1
38	28	1
39	28	1
40	28	1
41	28	1
42	28	1
43	28	1
45	28	1
46	28	1
51	28	1
30	29	1
31	29	1
32	29	1
33	29	1
34	29	1
35	29	1
36	29	1
37	29	1
38	29	1
39	29	1
40	29	1
41	29	1
42	29	1
43	29	1
44	29	1
45	29	1
46	29	1
49	29	1
50	29	1
54	29	1
31	30	1
32	30	1
33	30	1
34	30	1
35	30	1
36	30	1
37	30	1
38	30	1
39	30	1
40	30	1
41	30	1
42	30	1
43	30	1
44	30	1
45	30	1
46	30	1
49	30	1
50	30	1
51	30	1
55	30	1
32	31	1
33	31	1
34	31	1
35	31	1
36	31	1
37	31	1
38	31	1
39	31	1
40	31	1
41	31	1
42	31	1
43	31	1
44	31	1
45	31	1
46	31	1
50	31	1
51	31	1
56	31	1
33	32	1
34	32	1
35	32	1
36	32	1
37	32	1
38	32	1
39	32	1
40	32	1
41	32	1
42	32	1
43	32	1
44	32	1
45	32	1
46	32	1
49	32	1
50	32	1
51	32	1
54	32	1
55	32	1
59	32	1
34	33	1
35	33	1
36	33	1
37	33	1
38	33	1
39	33	1
40	33	1
41	33	1
42	33	1
43	33	1
44	33	1
45	33	1
46	33	1
49	33	1
50	33	1
51	33	1
54	33	1
55	33	1
56	33	1
60	33	1
35	34	1
36	34	1
37	34	1
38	34	1
39	34	1
40	34	1
41	34	1
42	34	1
43	34	1
44	34	1
45	34	1
46	34	1
49	34	1
50	34	1
51	34	1
55	34	1
56	34	1
61	34	1
36	35	1
37	35	1
38	35	1
39	35	1
40	35	1
41	35	1
42	35	1
43	35	1
44	35	1
45	35	1
46	35	1
49	35	1
50	35	1
51	35	1
54	35	1
55	35	1
56	35	1
59	35	1
60	35	1
64	35	1
37	36	1
38	36	1
39	36	1
40	36	1
41	36	1
42	36	1
43	36	1
44	36	1
45	36	1
46	36	1
49	36	1
50	36	1
51	36	1
54	36	1
55	36	1
56	36	1
59	36	1
60	36	1
61	36	1
65	36	1
38	37	1
39	37	1
40	37	1
41	37	1
42	37	1
43	37	1
44	37	1
45	37	1
46	37	1
49	37	1
50	37	1
51	37	1
54	37	1
55	37	1
56	37	1
60	37	1
61	37	1
66	37	1
39	38	1
40	38	1
41	38	1
42	38	1
43	38	1
44	38	1
45	38	1
46	38	1
49	38	1
50	38	1
51	38	1
54	38	1
55	38	1
56	38	1
59	38	1
60	38	1
61	38	1
64	38	1
65	38	1
69	38	1
40	39	1
41	39	1
42	39	1
43	39	1
44	39	1
45	39	1
46	39	1
49	39	1
50	39	1
51	39	1
54	39	1
55	39	1
56	39	1
59	39	1
60	39	1
61	39	1
64	39	1
65	39	1
66	39	1
70	39	1
41	40	1
42	40	1
43	40	1
44	40	1
45	40	1
46	40	1
49	40	1
50	40	1
51	40	1
54	40	1
55	40	1
56	40	1
59	40	1
60	40	1
61	40	1
65	40	1
66	40	1
71	40	1
42	41	1
43	41	1
44	41	1
45	41	1
46	41	1
49	41	1
50	41	1
51	41	1
54	41	1
55	41	1
56	41	1
59	41	1
60	41	1
61	41	1
64	41	1
65	41	1
66	41	1
69	41	1
70	41	1
43	42	1
44	42	1
45	42	1
46	42	1
49	42	1
50	42	1
51	42	1
54	42	1
55	42	1
56	42	1
58	42	1
59	42	1
60	42	1
61	42	1
63	42	1
64	42	1
65	42	1
66	42	1
69	42	1
70	42	1
71	42	1
44	43	1
45	43	1
46	43	1
49	43	1
50	43	1
51	43	1
53	43	1
54	43	1
55	43	1
56	43	1
59	43	1
60	43	1
61	43	1
64	43	1
65	43	1
66	43	1
70	43	1
71	43	1
45	44	1
46	44	1
49	44	1
50	44	1
51	44	1
54	44	1
55	44	1
56	44	1
59	44	1
60	44	1
61	44	1
64	44	1
65	44	1
66	44	1
69	44	1
70	44	1
71	44	1
46	45	1
49	45	1
50	45	1
51	45	1
53	45	1
54	45	1
55	45	1
56	45	1
57	45	1
58	45	1
59	45	1
60	45	1
61	45	1
63	45	1
64	45	1
65	45	1
66	45	1
68	45	1
69	45	1
70	45	1
71	45	1
49	46	1
50	46	1
51	46	1
53	46	1
54	46	1
55	46	1
56	46	1
58	46	1
59	46	1
60	46	1
61	46	1
64	46	1
65	46	1
66	46	1
69	46	1
70	46	1
71	46	1
48	47	1
49	47	1
50	47	1
52	47	1
53	47	1
54	47	1
55	47	1
57	47	1
58	47	1
59	47	1
60	47	1
62	47	1
63	47	1
64	47	1
65	47	1
67	47	1
68	47	1
69	47	1
49	48	1
50	48	1
51	48	1
52	48	1
53	48	1
54	48	1
55	48	1
56	48	1
57	48	1
58	48	1
59	48	1
60	48	1
61	48	1
62	48	1
63	48	1
64	48	1
65	48	1
66	48	1
67	48	1
68	48	1
69	48	1
70	48	1
50	49	1
51	49	1
52	49	1
53	49	1
54	49	1
55	49	1
56	49	1
57	49	1
58	49	1
59	49	1
60	49	1
61	49	1
62	49	1
63	49	1
64	49	1
65	49	1
66	49	1
67	49	1
68	49	1
69	49	1
70	49	1
71	49	1
51	50	1
52	50	1
53	50	1
54	50	1
55	50	1
56	50	1
57	50	1
58	50	1
59	50	1
60	50	1
61	50	1
62	50	1
63	50	1
64	50	1
65	50	1
66	50	1
68	50	1
69	50	1
70	50	1
71	50	1
53	51	1
54	51	1
55	51	1
56	51	1
58	51	1
59	51	1
60	51	1
61	51	1
63	51	1
64	51	1
65	51	1
66	51	1
69	51	1
70	51	1
71	51	1
53	52	1
54	52	1
55	52	1
57	52	1
58	52	1
59	52	1
60	52	1
62	52	1
63	52	1
64	52	1
65	52	1
67	52	1
68	52	1
69	52	1
70	52	1
54	53	1
55	53	1
56	53	1
57	53	1
58	53	1
59	53	1
60	53	1
61	53	1
62	53	1
63	53	1
64	53	1
65	53	1
66	53	1
67	53	1
68	53	1
69	53	1
70	53	1
71	53	1
55	54	1
56	54	1
57	54	1
58	54	1
59	54	1
60	54	1
61	54	1
62	54	1
63	54	1
64	54	1
65	54	1
66	54	1
67	54	1
68	54	1
69	54	1
70	54	1
71	54	1
56	55	1
57	55	1
58	55	1
59	55	1
60	55	1
61	55	1
62	55	1
63	55	1
64	55	1
65	55	1
66	55	1
67	55	1
68	55	1
69	55	1
70	55	1
71	55	1
58	56	1
59	56	1
60	56	1
61	56	1
63	56	1
64	56	1
65	56	1
66	56	1
68	56	1
69	56	1
70	56	1
71	56	1
58	57	1
59	57	1
60	57	1
62	57	1
63	57	1
64	57	1
65	57	1
67	57	1
68	57	1
69	57	1
70	57	1
59	58	1
60	58	1
61	58	1
62	58	1
63	58	1
64	58	1
65	58	1
66	58	1
67	58	1
68	58	1
69	58	1
70	58	1
71	58	1
60	59	1
61	59	1
62	59	1
63	59	1
64	59	1
65	59	1
66	59	1
67	59	1
68	59	1
69	59	1
70	59	1
71	59	1
61	60	1
62	60	1
63	60	1
64	60	1
65	60	1
66	60	1
67	60	1
68	60	1
69	60	1
70	60	1
71	60	1
63	61	1
64	61	1
65	61	1
66	61	1
68	61	1
69	61	1
70	61	1
71	61	1
63	62	1
64	62	1
65	62	1
67	62	1
68	62	1
69	62	1
70	62	1
64	63	1
65	63	1
66	63	1
67	63	1
68	63	1
69	63	1
70	63	1
71	63	1
65	64	1
66	64	1
67	64	1
68	64	1
69	64	1
70	64	1
71	64	1
66	65	1
67	65	1
68	65	1
69	65	1
70	65	1
71	65	1
68	66	1
69	66	1
70	66	1
71	66	1
68	67	1
69	67	1
70	67	1
69	68	1
70	68	1
71	68	1
70	69	1
71	69	1
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
