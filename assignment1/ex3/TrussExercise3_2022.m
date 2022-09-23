% Created with:       FlExtract v1.13
% Element type:       truss
% Number of nodes:    156
% Number of elements: 443

close all
clear all
clc

%SCALE = 1; % Ec = 0.8
%SCALE = 0.5; % Ec = 0.4
SCALE = 2; % Ec = 1.6

nincr  = 200;
i_max  = 1000;
eSTOP  = 10^(-9);
Pfinal = -0.004*SCALE;


% Node coordinates: x, y
X = [
0	0
0	5
0	10
0	15
0	20
0	25
0	30
0	35
0	40
0	45
0	50
0	55
0	60
0	65
0	70
0	75
0	80
0	85
0	90
0	95
0	100
5	0
5	5
5	10
5	15
5	20
5	25
5	30
5	35
5	40
5	45
5	50
5	55
5	60
5	65
5	70
5	75
5	80
5	85
5	90
5	95
5	100
10	0
10	5
10	10
10	90
10	95
10	100
15	0
15	5
15	10
15	90
15	95
15	100
20	0
20	5
20	10
20	90
20	95
20	100
25	0
25	5
25	10
25	90
25	95
25	100
30	0
30	5
30	10
30	90
30	95
30	100
35	0
35	5
35	10
35	90
35	95
35	100
40	0
40	5
40	10
40	90
40	95
40	100
45	0
45	5
45	10
45	90
45	95
45	100
50	0
50	5
50	10
50	90
50	95
50	100
55	0
55	5
55	10
55	90
55	95
55	100
60	0
60	5
60	10
60	90
60	95
60	100
65	0
65	5
65	10
65	90
65	95
65	100
70	0
70	5
70	10
70	90
70	95
70	100
75	0
75	5
75	10
75	90
75	95
75	100
80	0
80	5
80	10
80	90
80	95
80	100
85	0
85	5
85	10
85	90
85	95
85	100
90	0
90	5
90	10
90	90
90	95
90	100
95	0
95	5
95	10
95	90
95	95
95	100
100	0
100	5
100	10
100	90
100	95
100	100
];
% Element connectivity: node1_id, node2_id, material_id
IX = [
2	1	1
23	1	1
22	1	1
3	2	1
24	2	1
23	2	1
22	2	1
4	3	2
25	3	2
24	3	1
23	3	1
5	4	2
26	4	2
25	4	2
24	4	2
6	5	2
27	5	2
26	5	2
25	5	2
7	6	2
28	6	2
27	6	2
26	6	2
8	7	2
29	7	2
28	7	2
27	7	2
9	8	2
30	8	2
29	8	2
28	8	2
10	9	2
31	9	2
30	9	2
29	9	2
11	10	2
32	10	2
31	10	2
30	10	2
12	11	2
33	11	2
32	11	2
31	11	2
13	12	2
34	12	2
33	12	2
32	12	2
14	13	2
35	13	2
34	13	2
33	13	2
15	14	2
36	14	2
35	14	2
34	14	2
16	15	2
37	15	2
36	15	2
35	15	2
17	16	2
38	16	2
37	16	2
36	16	2
18	17	2
39	17	2
38	17	2
37	17	2
19	18	2
40	18	2
39	18	2
38	18	2
20	19	1
41	19	1
40	19	1
39	19	2
21	20	1
42	20	1
41	20	1
40	20	1
42	21	1
41	21	1
23	22	1
44	22	1
43	22	1
24	23	1
45	23	1
44	23	1
43	23	1
25	24	2
45	24	1
44	24	1
26	25	2
27	26	2
28	27	2
29	28	2
30	29	2
31	30	2
32	31	2
33	32	2
34	33	2
35	34	2
36	35	2
37	36	2
38	37	2
39	38	2
40	39	2
41	40	1
47	40	1
46	40	1
42	41	1
48	41	1
47	41	1
46	41	1
48	42	1
47	42	1
44	43	1
50	43	1
49	43	1
45	44	1
51	44	1
50	44	1
49	44	1
51	45	1
50	45	1
47	46	1
53	46	1
52	46	1
48	47	1
54	47	1
53	47	1
52	47	1
54	48	1
53	48	1
50	49	1
56	49	1
55	49	1
51	50	1
57	50	1
56	50	1
55	50	1
57	51	1
56	51	1
53	52	1
59	52	1
58	52	1
54	53	1
60	53	1
59	53	1
58	53	1
60	54	1
59	54	1
56	55	1
62	55	1
61	55	1
57	56	1
63	56	1
62	56	1
61	56	1
63	57	1
62	57	1
59	58	1
65	58	1
64	58	1
60	59	1
66	59	1
65	59	1
64	59	1
66	60	1
65	60	1
62	61	1
68	61	1
67	61	1
63	62	1
69	62	1
68	62	1
67	62	1
69	63	1
68	63	1
65	64	1
71	64	1
70	64	1
66	65	1
72	65	1
71	65	1
70	65	1
72	66	1
71	66	1
68	67	1
74	67	1
73	67	1
69	68	1
75	68	1
74	68	1
73	68	1
75	69	1
74	69	1
71	70	1
77	70	1
76	70	1
72	71	1
78	71	1
77	71	1
76	71	1
78	72	1
77	72	1
74	73	1
80	73	1
79	73	1
75	74	1
81	74	1
80	74	1
79	74	1
81	75	1
80	75	1
77	76	1
83	76	1
82	76	1
78	77	1
84	77	1
83	77	1
82	77	1
84	78	1
83	78	1
80	79	1
86	79	1
85	79	1
81	80	1
87	80	1
86	80	1
85	80	1
87	81	1
86	81	1
83	82	1
89	82	1
88	82	1
84	83	1
90	83	1
89	83	1
88	83	1
90	84	1
89	84	1
86	85	1
92	85	1
91	85	1
87	86	1
93	86	1
92	86	1
91	86	1
93	87	1
92	87	1
89	88	1
95	88	1
94	88	1
90	89	1
96	89	1
95	89	1
94	89	1
96	90	1
95	90	1
92	91	1
98	91	1
97	91	1
93	92	1
99	92	1
98	92	1
97	92	1
99	93	1
98	93	1
95	94	1
101	94	1
100	94	1
96	95	1
102	95	1
101	95	1
100	95	1
102	96	1
101	96	1
98	97	1
104	97	1
103	97	1
99	98	1
105	98	1
104	98	1
103	98	1
105	99	1
104	99	1
101	100	1
107	100	1
106	100	1
102	101	1
108	101	1
107	101	1
106	101	1
108	102	1
107	102	1
104	103	1
110	103	1
109	103	1
105	104	1
111	104	1
110	104	1
109	104	1
111	105	1
110	105	1
107	106	1
113	106	1
112	106	1
108	107	1
114	107	1
113	107	1
112	107	1
114	108	1
113	108	1
110	109	1
116	109	1
115	109	1
111	110	1
117	110	1
116	110	1
115	110	1
117	111	1
116	111	1
113	112	1
119	112	1
118	112	1
114	113	1
120	113	1
119	113	1
118	113	1
120	114	1
119	114	1
116	115	1
122	115	1
121	115	1
117	116	1
123	116	1
122	116	1
121	116	1
123	117	1
122	117	1
119	118	1
125	118	1
124	118	1
120	119	1
126	119	1
125	119	1
124	119	1
126	120	1
125	120	1
122	121	1
128	121	1
127	121	1
123	122	1
129	122	1
128	122	1
127	122	1
129	123	1
128	123	1
125	124	1
131	124	1
130	124	1
126	125	1
132	125	1
131	125	1
130	125	1
132	126	1
131	126	1
128	127	1
134	127	1
133	127	1
129	128	1
135	128	1
134	128	1
133	128	1
135	129	1
134	129	1
131	130	1
137	130	1
136	130	1
132	131	1
138	131	1
137	131	1
136	131	1
138	132	1
137	132	1
134	133	1
140	133	1
139	133	1
135	134	1
141	134	1
140	134	1
139	134	1
141	135	1
140	135	1
137	136	1
143	136	1
142	136	1
138	137	1
144	137	1
143	137	1
142	137	1
144	138	1
143	138	1
140	139	1
146	139	1
145	139	1
141	140	1
147	140	1
146	140	1
145	140	1
147	141	1
146	141	1
143	142	1
149	142	1
148	142	1
144	143	1
150	143	1
149	143	1
148	143	1
150	144	1
149	144	1
146	145	1
152	145	1
151	145	1
147	146	1
153	146	1
152	146	1
151	146	1
153	147	1
152	147	1
149	148	1
155	148	1
154	148	1
150	149	1
156	149	1
155	149	1
154	149	1
156	150	1
155	150	1
152	151	1
153	152	1
155	154	1
156	155	1
];
% Element properties: Young's modulus, area
mprop = [0.2        	0.1 % beam
         0.8*SCALE	    0.2]; % column
% Nodal diplacements: node_id, degree of freedom (1 - x, 2 - y), displacement
bound = [
151	1	0
151	2	0
152	1	0
152	2	0
153	1	0
153	2	0
154	1	0
154	2	0
155	1	0
155	2	0
156	1	0
156	2	0
];
% Nodal loads: node_id, degree of freedom (1 - x, 2 - y), load
loads = [
21	2	Pfinal
1	2	-Pfinal
];
% Control parameters
plotdof = 21;