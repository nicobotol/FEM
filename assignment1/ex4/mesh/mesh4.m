% Created with:       FlExtract v1.13
% Element type:       truss
% Number of nodes:    543
% Number of elements: 1982

%clear all

eta = 0.67;
p = 1.5;
V_star = 6;
iopt_max = 100;
rho_min = 1e-6;
epsilon = 1e-5;

% Node coordinates: x, y
X = [
0	0
0	0.05
0	0.1
0	0.15
0	0.2
0	0.25
0	0.3
0	0.35
0	0.4
0	0.45
0	0.5
0	0.55
0	0.6
0	0.65
0	0.7
0	0.75
0	0.8
0	0.85
0	0.9
0	0.95
0	1
0.0625	0
0.0625	0.05
0.0625	0.1
0.0625	0.15
0.0625	0.2
0.0625	0.25
0.0625	0.3
0.0625	0.35
0.0625	0.4
0.0625	0.45
0.0625	0.5
0.0625	0.55
0.0625	0.6
0.0625	0.65
0.0625	0.7
0.0625	0.75
0.0625	0.8
0.0625	0.85
0.0625	0.9
0.0625	0.95
0.0625	1
0.125	0
0.125	0.05
0.125	0.1
0.125	0.15
0.125	0.2
0.125	0.25
0.125	0.3
0.125	0.35
0.125	0.4
0.125	0.45
0.125	0.5
0.125	0.55
0.125	0.6
0.125	0.65
0.125	0.7
0.125	0.75
0.125	0.8
0.125	0.85
0.125	0.9
0.125	0.95
0.125	1
0.1875	0
0.1875	0.05
0.1875	0.1
0.1875	0.15
0.1875	0.2
0.1875	0.25
0.1875	0.3
0.1875	0.35
0.1875	0.4
0.1875	0.45
0.1875	0.5
0.1875	0.55
0.1875	0.6
0.1875	0.65
0.1875	0.7
0.1875	0.75
0.1875	0.8
0.1875	0.85
0.1875	0.9
0.1875	0.95
0.1875	1
0.25	0
0.25	0.05
0.25	0.1
0.25	0.15
0.25	0.2
0.25	0.25
0.25	0.3
0.25	0.35
0.25	0.4
0.25	0.45
0.25	0.5
0.25	0.55
0.25	0.6
0.25	0.65
0.25	0.7
0.25	0.75
0.25	0.8
0.25	0.85
0.25	0.9
0.25	0.95
0.25	1
0.3125	0
0.3125	0.05
0.3125	0.1
0.3125	0.15
0.3125	0.2
0.3125	0.25
0.3125	0.3
0.3125	0.35
0.3125	0.4
0.3125	0.45
0.3125	0.5
0.3125	0.55
0.3125	0.6
0.3125	0.65
0.3125	0.7
0.3125	0.75
0.3125	0.8
0.3125	0.85
0.3125	0.9
0.3125	0.95
0.3125	1
0.375	0
0.375	0.05
0.375	0.1
0.375	0.15
0.375	0.2
0.375	0.25
0.375	0.3
0.375	0.35
0.375	0.4
0.375	0.45
0.375	0.5
0.375	0.55
0.375	0.6
0.375	0.65
0.375	0.7
0.375	0.75
0.375	0.8
0.375	0.85
0.375	0.9
0.375	0.95
0.375	1
0.4375	0
0.4375	0.05
0.4375	0.1
0.4375	0.15
0.4375	0.2
0.4375	0.25
0.4375	0.3
0.4375	0.35
0.4375	0.4
0.4375	0.45
0.4375	0.5
0.4375	0.55
0.4375	0.6
0.4375	0.65
0.4375	0.7
0.4375	0.75
0.4375	0.8
0.4375	0.85
0.4375	0.9
0.4375	0.95
0.4375	1
0.5	0
0.5	0.05
0.5	0.1
0.5	0.15
0.5	0.2
0.5	0.25
0.5	0.3
0.5	0.35
0.5	0.4
0.5	0.45
0.5	0.5
0.5	0.55
0.5	0.6
0.5	0.65
0.5	0.7
0.5	0.75
0.5	0.8
0.5	0.85
0.5	0.9
0.5	0.95
0.5	1
0.5625	0.5
0.5625	0.55
0.5625	0.6
0.5625	0.65
0.5625	0.7
0.5625	0.75
0.5625	0.8
0.5625	0.85
0.5625	0.9
0.5625	0.95
0.5625	1
0.625	0.5
0.625	0.55
0.625	0.6
0.625	0.65
0.625	0.7
0.625	0.75
0.625	0.8
0.625	0.85
0.625	0.9
0.625	0.95
0.625	1
0.6875	0.5
0.6875	0.55
0.6875	0.6
0.6875	0.65
0.6875	0.7
0.6875	0.75
0.6875	0.8
0.6875	0.85
0.6875	0.9
0.6875	0.95
0.6875	1
0.75	0.5
0.75	0.55
0.75	0.6
0.75	0.65
0.75	0.7
0.75	0.75
0.75	0.8
0.75	0.85
0.75	0.9
0.75	0.95
0.75	1
0.8125	0.5
0.8125	0.55
0.8125	0.6
0.8125	0.65
0.8125	0.7
0.8125	0.75
0.8125	0.8
0.8125	0.85
0.8125	0.9
0.8125	0.95
0.8125	1
0.875	0.5
0.875	0.55
0.875	0.6
0.875	0.65
0.875	0.7
0.875	0.75
0.875	0.8
0.875	0.85
0.875	0.9
0.875	0.95
0.875	1
0.9375	0.5
0.9375	0.55
0.9375	0.6
0.9375	0.65
0.9375	0.7
0.9375	0.75
0.9375	0.8
0.9375	0.85
0.9375	0.9
0.9375	0.95
0.9375	1
1	0.5
1	0.55
1	0.6
1	0.65
1	0.7
1	0.75
1	0.8
1	0.85
1	0.9
1	0.95
1	1
1.0625	0.5
1.0625	0.55
1.0625	0.6
1.0625	0.65
1.0625	0.7
1.0625	0.75
1.0625	0.8
1.0625	0.85
1.0625	0.9
1.0625	0.95
1.0625	1
1.125	0.5
1.125	0.55
1.125	0.6
1.125	0.65
1.125	0.7
1.125	0.75
1.125	0.8
1.125	0.85
1.125	0.9
1.125	0.95
1.125	1
1.1875	0.5
1.1875	0.55
1.1875	0.6
1.1875	0.65
1.1875	0.7
1.1875	0.75
1.1875	0.8
1.1875	0.85
1.1875	0.9
1.1875	0.95
1.1875	1
1.25	0.5
1.25	0.55
1.25	0.6
1.25	0.65
1.25	0.7
1.25	0.75
1.25	0.8
1.25	0.85
1.25	0.9
1.25	0.95
1.25	1
1.3125	0.5
1.3125	0.55
1.3125	0.6
1.3125	0.65
1.3125	0.7
1.3125	0.75
1.3125	0.8
1.3125	0.85
1.3125	0.9
1.3125	0.95
1.3125	1
1.375	0.5
1.375	0.55
1.375	0.6
1.375	0.65
1.375	0.7
1.375	0.75
1.375	0.8
1.375	0.85
1.375	0.9
1.375	0.95
1.375	1
1.4375	0.5
1.4375	0.55
1.4375	0.6
1.4375	0.65
1.4375	0.7
1.4375	0.75
1.4375	0.8
1.4375	0.85
1.4375	0.9
1.4375	0.95
1.4375	1
1.5	0
1.5	0.05
1.5	0.1
1.5	0.15
1.5	0.2
1.5	0.25
1.5	0.3
1.5	0.35
1.5	0.4
1.5	0.45
1.5	0.5
1.5	0.55
1.5	0.6
1.5	0.65
1.5	0.7
1.5	0.75
1.5	0.8
1.5	0.85
1.5	0.9
1.5	0.95
1.5	1
1.5625	0
1.5625	0.05
1.5625	0.1
1.5625	0.15
1.5625	0.2
1.5625	0.25
1.5625	0.3
1.5625	0.35
1.5625	0.4
1.5625	0.45
1.5625	0.5
1.5625	0.55
1.5625	0.6
1.5625	0.65
1.5625	0.7
1.5625	0.75
1.5625	0.8
1.5625	0.85
1.5625	0.9
1.5625	0.95
1.5625	1
1.625	0
1.625	0.05
1.625	0.1
1.625	0.15
1.625	0.2
1.625	0.25
1.625	0.3
1.625	0.35
1.625	0.4
1.625	0.45
1.625	0.5
1.625	0.55
1.625	0.6
1.625	0.65
1.625	0.7
1.625	0.75
1.625	0.8
1.625	0.85
1.625	0.9
1.625	0.95
1.625	1
1.6875	0
1.6875	0.05
1.6875	0.1
1.6875	0.15
1.6875	0.2
1.6875	0.25
1.6875	0.3
1.6875	0.35
1.6875	0.4
1.6875	0.45
1.6875	0.5
1.6875	0.55
1.6875	0.6
1.6875	0.65
1.6875	0.7
1.6875	0.75
1.6875	0.8
1.6875	0.85
1.6875	0.9
1.6875	0.95
1.6875	1
1.75	0
1.75	0.05
1.75	0.1
1.75	0.15
1.75	0.2
1.75	0.25
1.75	0.3
1.75	0.35
1.75	0.4
1.75	0.45
1.75	0.5
1.75	0.55
1.75	0.6
1.75	0.65
1.75	0.7
1.75	0.75
1.75	0.8
1.75	0.85
1.75	0.9
1.75	0.95
1.75	1
1.8125	0
1.8125	0.05
1.8125	0.1
1.8125	0.15
1.8125	0.2
1.8125	0.25
1.8125	0.3
1.8125	0.35
1.8125	0.4
1.8125	0.45
1.8125	0.5
1.8125	0.55
1.8125	0.6
1.8125	0.65
1.8125	0.7
1.8125	0.75
1.8125	0.8
1.8125	0.85
1.8125	0.9
1.8125	0.95
1.8125	1
1.875	0
1.875	0.05
1.875	0.1
1.875	0.15
1.875	0.2
1.875	0.25
1.875	0.3
1.875	0.35
1.875	0.4
1.875	0.45
1.875	0.5
1.875	0.55
1.875	0.6
1.875	0.65
1.875	0.7
1.875	0.75
1.875	0.8
1.875	0.85
1.875	0.9
1.875	0.95
1.875	1
1.9375	0
1.9375	0.05
1.9375	0.1
1.9375	0.15
1.9375	0.2
1.9375	0.25
1.9375	0.3
1.9375	0.35
1.9375	0.4
1.9375	0.45
1.9375	0.5
1.9375	0.55
1.9375	0.6
1.9375	0.65
1.9375	0.7
1.9375	0.75
1.9375	0.8
1.9375	0.85
1.9375	0.9
1.9375	0.95
1.9375	1
2	0
2	0.05
2	0.1
2	0.15
2	0.2
2	0.25
2	0.3
2	0.35
2	0.4
2	0.45
2	0.5
2	0.55
2	0.6
2	0.65
2	0.7
2	0.75
2	0.8
2	0.85
2	0.9
2	0.95
2	1
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
4	3	1
25	3	1
24	3	1
23	3	1
5	4	1
26	4	1
25	4	1
24	4	1
6	5	1
27	5	1
26	5	1
25	5	1
7	6	1
28	6	1
27	6	1
26	6	1
8	7	1
29	7	1
28	7	1
27	7	1
9	8	1
30	8	1
29	8	1
28	8	1
10	9	1
31	9	1
30	9	1
29	9	1
11	10	1
32	10	1
31	10	1
30	10	1
12	11	1
33	11	1
32	11	1
31	11	1
13	12	1
34	12	1
33	12	1
32	12	1
14	13	1
35	13	1
34	13	1
33	13	1
15	14	1
36	14	1
35	14	1
34	14	1
16	15	1
37	15	1
36	15	1
35	15	1
17	16	1
38	16	1
37	16	1
36	16	1
18	17	1
39	17	1
38	17	1
37	17	1
19	18	1
40	18	1
39	18	1
38	18	1
20	19	1
41	19	1
40	19	1
39	19	1
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
25	24	1
46	24	1
45	24	1
44	24	1
26	25	1
47	25	1
46	25	1
45	25	1
27	26	1
48	26	1
47	26	1
46	26	1
28	27	1
49	27	1
48	27	1
47	27	1
29	28	1
50	28	1
49	28	1
48	28	1
30	29	1
51	29	1
50	29	1
49	29	1
31	30	1
52	30	1
51	30	1
50	30	1
32	31	1
53	31	1
52	31	1
51	31	1
33	32	1
54	32	1
53	32	1
52	32	1
34	33	1
55	33	1
54	33	1
53	33	1
35	34	1
56	34	1
55	34	1
54	34	1
36	35	1
57	35	1
56	35	1
55	35	1
37	36	1
58	36	1
57	36	1
56	36	1
38	37	1
59	37	1
58	37	1
57	37	1
39	38	1
60	38	1
59	38	1
58	38	1
40	39	1
61	39	1
60	39	1
59	39	1
41	40	1
62	40	1
61	40	1
60	40	1
42	41	1
63	41	1
62	41	1
61	41	1
63	42	1
62	42	1
44	43	1
65	43	1
64	43	1
45	44	1
66	44	1
65	44	1
64	44	1
46	45	1
67	45	1
66	45	1
65	45	1
47	46	1
68	46	1
67	46	1
66	46	1
48	47	1
69	47	1
68	47	1
67	47	1
49	48	1
70	48	1
69	48	1
68	48	1
50	49	1
71	49	1
70	49	1
69	49	1
51	50	1
72	50	1
71	50	1
70	50	1
52	51	1
73	51	1
72	51	1
71	51	1
53	52	1
74	52	1
73	52	1
72	52	1
54	53	1
75	53	1
74	53	1
73	53	1
55	54	1
76	54	1
75	54	1
74	54	1
56	55	1
77	55	1
76	55	1
75	55	1
57	56	1
78	56	1
77	56	1
76	56	1
58	57	1
79	57	1
78	57	1
77	57	1
59	58	1
80	58	1
79	58	1
78	58	1
60	59	1
81	59	1
80	59	1
79	59	1
61	60	1
82	60	1
81	60	1
80	60	1
62	61	1
83	61	1
82	61	1
81	61	1
63	62	1
84	62	1
83	62	1
82	62	1
84	63	1
83	63	1
65	64	1
86	64	1
85	64	1
66	65	1
87	65	1
86	65	1
85	65	1
67	66	1
88	66	1
87	66	1
86	66	1
68	67	1
89	67	1
88	67	1
87	67	1
69	68	1
90	68	1
89	68	1
88	68	1
70	69	1
91	69	1
90	69	1
89	69	1
71	70	1
92	70	1
91	70	1
90	70	1
72	71	1
93	71	1
92	71	1
91	71	1
73	72	1
94	72	1
93	72	1
92	72	1
74	73	1
95	73	1
94	73	1
93	73	1
75	74	1
96	74	1
95	74	1
94	74	1
76	75	1
97	75	1
96	75	1
95	75	1
77	76	1
98	76	1
97	76	1
96	76	1
78	77	1
99	77	1
98	77	1
97	77	1
79	78	1
100	78	1
99	78	1
98	78	1
80	79	1
101	79	1
100	79	1
99	79	1
81	80	1
102	80	1
101	80	1
100	80	1
82	81	1
103	81	1
102	81	1
101	81	1
83	82	1
104	82	1
103	82	1
102	82	1
84	83	1
105	83	1
104	83	1
103	83	1
105	84	1
104	84	1
86	85	1
107	85	1
106	85	1
87	86	1
108	86	1
107	86	1
106	86	1
88	87	1
109	87	1
108	87	1
107	87	1
89	88	1
110	88	1
109	88	1
108	88	1
90	89	1
111	89	1
110	89	1
109	89	1
91	90	1
112	90	1
111	90	1
110	90	1
92	91	1
113	91	1
112	91	1
111	91	1
93	92	1
114	92	1
113	92	1
112	92	1
94	93	1
115	93	1
114	93	1
113	93	1
95	94	1
116	94	1
115	94	1
114	94	1
96	95	1
117	95	1
116	95	1
115	95	1
97	96	1
118	96	1
117	96	1
116	96	1
98	97	1
119	97	1
118	97	1
117	97	1
99	98	1
120	98	1
119	98	1
118	98	1
100	99	1
121	99	1
120	99	1
119	99	1
101	100	1
122	100	1
121	100	1
120	100	1
102	101	1
123	101	1
122	101	1
121	101	1
103	102	1
124	102	1
123	102	1
122	102	1
104	103	1
125	103	1
124	103	1
123	103	1
105	104	1
126	104	1
125	104	1
124	104	1
126	105	1
125	105	1
107	106	1
128	106	1
127	106	1
108	107	1
129	107	1
128	107	1
127	107	1
109	108	1
130	108	1
129	108	1
128	108	1
110	109	1
131	109	1
130	109	1
129	109	1
111	110	1
132	110	1
131	110	1
130	110	1
112	111	1
133	111	1
132	111	1
131	111	1
113	112	1
134	112	1
133	112	1
132	112	1
114	113	1
135	113	1
134	113	1
133	113	1
115	114	1
136	114	1
135	114	1
134	114	1
116	115	1
137	115	1
136	115	1
135	115	1
117	116	1
138	116	1
137	116	1
136	116	1
118	117	1
139	117	1
138	117	1
137	117	1
119	118	1
140	118	1
139	118	1
138	118	1
120	119	1
141	119	1
140	119	1
139	119	1
121	120	1
142	120	1
141	120	1
140	120	1
122	121	1
143	121	1
142	121	1
141	121	1
123	122	1
144	122	1
143	122	1
142	122	1
124	123	1
145	123	1
144	123	1
143	123	1
125	124	1
146	124	1
145	124	1
144	124	1
126	125	1
147	125	1
146	125	1
145	125	1
147	126	1
146	126	1
128	127	1
149	127	1
148	127	1
129	128	1
150	128	1
149	128	1
148	128	1
130	129	1
151	129	1
150	129	1
149	129	1
131	130	1
152	130	1
151	130	1
150	130	1
132	131	1
153	131	1
152	131	1
151	131	1
133	132	1
154	132	1
153	132	1
152	132	1
134	133	1
155	133	1
154	133	1
153	133	1
135	134	1
156	134	1
155	134	1
154	134	1
136	135	1
157	135	1
156	135	1
155	135	1
137	136	1
158	136	1
157	136	1
156	136	1
138	137	1
159	137	1
158	137	1
157	137	1
139	138	1
160	138	1
159	138	1
158	138	1
140	139	1
161	139	1
160	139	1
159	139	1
141	140	1
162	140	1
161	140	1
160	140	1
142	141	1
163	141	1
162	141	1
161	141	1
143	142	1
164	142	1
163	142	1
162	142	1
144	143	1
165	143	1
164	143	1
163	143	1
145	144	1
166	144	1
165	144	1
164	144	1
146	145	1
167	145	1
166	145	1
165	145	1
147	146	1
168	146	1
167	146	1
166	146	1
168	147	1
167	147	1
149	148	1
170	148	1
169	148	1
150	149	1
171	149	1
170	149	1
169	149	1
151	150	1
172	150	1
171	150	1
170	150	1
152	151	1
173	151	1
172	151	1
171	151	1
153	152	1
174	152	1
173	152	1
172	152	1
154	153	1
175	153	1
174	153	1
173	153	1
155	154	1
176	154	1
175	154	1
174	154	1
156	155	1
177	155	1
176	155	1
175	155	1
157	156	1
178	156	1
177	156	1
176	156	1
158	157	1
179	157	1
178	157	1
177	157	1
159	158	1
180	158	1
179	158	1
178	158	1
160	159	1
181	159	1
180	159	1
179	159	1
161	160	1
182	160	1
181	160	1
180	160	1
162	161	1
183	161	1
182	161	1
181	161	1
163	162	1
184	162	1
183	162	1
182	162	1
164	163	1
185	163	1
184	163	1
183	163	1
165	164	1
186	164	1
185	164	1
184	164	1
166	165	1
187	165	1
186	165	1
185	165	1
167	166	1
188	166	1
187	166	1
186	166	1
168	167	1
189	167	1
188	167	1
187	167	1
189	168	1
188	168	1
170	169	1
171	170	1
172	171	1
173	172	1
174	173	1
175	174	1
176	175	1
177	176	1
178	177	1
179	178	1
180	179	1
191	179	1
190	179	1
181	180	1
192	180	1
191	180	1
190	180	1
182	181	1
193	181	1
192	181	1
191	181	1
183	182	1
194	182	1
193	182	1
192	182	1
184	183	1
195	183	1
194	183	1
193	183	1
185	184	1
196	184	1
195	184	1
194	184	1
186	185	1
197	185	1
196	185	1
195	185	1
187	186	1
198	186	1
197	186	1
196	186	1
188	187	1
199	187	1
198	187	1
197	187	1
189	188	1
200	188	1
199	188	1
198	188	1
200	189	1
199	189	1
191	190	1
202	190	1
201	190	1
192	191	1
203	191	1
202	191	1
201	191	1
193	192	1
204	192	1
203	192	1
202	192	1
194	193	1
205	193	1
204	193	1
203	193	1
195	194	1
206	194	1
205	194	1
204	194	1
196	195	1
207	195	1
206	195	1
205	195	1
197	196	1
208	196	1
207	196	1
206	196	1
198	197	1
209	197	1
208	197	1
207	197	1
199	198	1
210	198	1
209	198	1
208	198	1
200	199	1
211	199	1
210	199	1
209	199	1
211	200	1
210	200	1
202	201	1
213	201	1
212	201	1
203	202	1
214	202	1
213	202	1
212	202	1
204	203	1
215	203	1
214	203	1
213	203	1
205	204	1
216	204	1
215	204	1
214	204	1
206	205	1
217	205	1
216	205	1
215	205	1
207	206	1
218	206	1
217	206	1
216	206	1
208	207	1
219	207	1
218	207	1
217	207	1
209	208	1
220	208	1
219	208	1
218	208	1
210	209	1
221	209	1
220	209	1
219	209	1
211	210	1
222	210	1
221	210	1
220	210	1
222	211	1
221	211	1
213	212	1
224	212	1
223	212	1
214	213	1
225	213	1
224	213	1
223	213	1
215	214	1
226	214	1
225	214	1
224	214	1
216	215	1
227	215	1
226	215	1
225	215	1
217	216	1
228	216	1
227	216	1
226	216	1
218	217	1
229	217	1
228	217	1
227	217	1
219	218	1
230	218	1
229	218	1
228	218	1
220	219	1
231	219	1
230	219	1
229	219	1
221	220	1
232	220	1
231	220	1
230	220	1
222	221	1
233	221	1
232	221	1
231	221	1
233	222	1
232	222	1
224	223	1
235	223	1
234	223	1
225	224	1
236	224	1
235	224	1
234	224	1
226	225	1
237	225	1
236	225	1
235	225	1
227	226	1
238	226	1
237	226	1
236	226	1
228	227	1
239	227	1
238	227	1
237	227	1
229	228	1
240	228	1
239	228	1
238	228	1
230	229	1
241	229	1
240	229	1
239	229	1
231	230	1
242	230	1
241	230	1
240	230	1
232	231	1
243	231	1
242	231	1
241	231	1
233	232	1
244	232	1
243	232	1
242	232	1
244	233	1
243	233	1
235	234	1
246	234	1
245	234	1
236	235	1
247	235	1
246	235	1
245	235	1
237	236	1
248	236	1
247	236	1
246	236	1
238	237	1
249	237	1
248	237	1
247	237	1
239	238	1
250	238	1
249	238	1
248	238	1
240	239	1
251	239	1
250	239	1
249	239	1
241	240	1
252	240	1
251	240	1
250	240	1
242	241	1
253	241	1
252	241	1
251	241	1
243	242	1
254	242	1
253	242	1
252	242	1
244	243	1
255	243	1
254	243	1
253	243	1
255	244	1
254	244	1
246	245	1
257	245	1
256	245	1
247	246	1
258	246	1
257	246	1
256	246	1
248	247	1
259	247	1
258	247	1
257	247	1
249	248	1
260	248	1
259	248	1
258	248	1
250	249	1
261	249	1
260	249	1
259	249	1
251	250	1
262	250	1
261	250	1
260	250	1
252	251	1
263	251	1
262	251	1
261	251	1
253	252	1
264	252	1
263	252	1
262	252	1
254	253	1
265	253	1
264	253	1
263	253	1
255	254	1
266	254	1
265	254	1
264	254	1
266	255	1
265	255	1
257	256	1
268	256	1
267	256	1
258	257	1
269	257	1
268	257	1
267	257	1
259	258	1
270	258	1
269	258	1
268	258	1
260	259	1
271	259	1
270	259	1
269	259	1
261	260	1
272	260	1
271	260	1
270	260	1
262	261	1
273	261	1
272	261	1
271	261	1
263	262	1
274	262	1
273	262	1
272	262	1
264	263	1
275	263	1
274	263	1
273	263	1
265	264	1
276	264	1
275	264	1
274	264	1
266	265	1
277	265	1
276	265	1
275	265	1
277	266	1
276	266	1
268	267	1
279	267	1
278	267	1
269	268	1
280	268	1
279	268	1
278	268	1
270	269	1
281	269	1
280	269	1
279	269	1
271	270	1
282	270	1
281	270	1
280	270	1
272	271	1
283	271	1
282	271	1
281	271	1
273	272	1
284	272	1
283	272	1
282	272	1
274	273	1
285	273	1
284	273	1
283	273	1
275	274	1
286	274	1
285	274	1
284	274	1
276	275	1
287	275	1
286	275	1
285	275	1
277	276	1
288	276	1
287	276	1
286	276	1
288	277	1
287	277	1
279	278	1
290	278	1
289	278	1
280	279	1
291	279	1
290	279	1
289	279	1
281	280	1
292	280	1
291	280	1
290	280	1
282	281	1
293	281	1
292	281	1
291	281	1
283	282	1
294	282	1
293	282	1
292	282	1
284	283	1
295	283	1
294	283	1
293	283	1
285	284	1
296	284	1
295	284	1
294	284	1
286	285	1
297	285	1
296	285	1
295	285	1
287	286	1
298	286	1
297	286	1
296	286	1
288	287	1
299	287	1
298	287	1
297	287	1
299	288	1
298	288	1
290	289	1
301	289	1
300	289	1
291	290	1
302	290	1
301	290	1
300	290	1
292	291	1
303	291	1
302	291	1
301	291	1
293	292	1
304	292	1
303	292	1
302	292	1
294	293	1
305	293	1
304	293	1
303	293	1
295	294	1
306	294	1
305	294	1
304	294	1
296	295	1
307	295	1
306	295	1
305	295	1
297	296	1
308	296	1
307	296	1
306	296	1
298	297	1
309	297	1
308	297	1
307	297	1
299	298	1
310	298	1
309	298	1
308	298	1
310	299	1
309	299	1
301	300	1
312	300	1
311	300	1
302	301	1
313	301	1
312	301	1
311	301	1
303	302	1
314	302	1
313	302	1
312	302	1
304	303	1
315	303	1
314	303	1
313	303	1
305	304	1
316	304	1
315	304	1
314	304	1
306	305	1
317	305	1
316	305	1
315	305	1
307	306	1
318	306	1
317	306	1
316	306	1
308	307	1
319	307	1
318	307	1
317	307	1
309	308	1
320	308	1
319	308	1
318	308	1
310	309	1
321	309	1
320	309	1
319	309	1
321	310	1
320	310	1
312	311	1
323	311	1
322	311	1
313	312	1
324	312	1
323	312	1
322	312	1
314	313	1
325	313	1
324	313	1
323	313	1
315	314	1
326	314	1
325	314	1
324	314	1
316	315	1
327	315	1
326	315	1
325	315	1
317	316	1
328	316	1
327	316	1
326	316	1
318	317	1
329	317	1
328	317	1
327	317	1
319	318	1
330	318	1
329	318	1
328	318	1
320	319	1
331	319	1
330	319	1
329	319	1
321	320	1
332	320	1
331	320	1
330	320	1
332	321	1
331	321	1
323	322	1
334	322	1
333	322	1
324	323	1
335	323	1
334	323	1
333	323	1
325	324	1
336	324	1
335	324	1
334	324	1
326	325	1
337	325	1
336	325	1
335	325	1
327	326	1
338	326	1
337	326	1
336	326	1
328	327	1
339	327	1
338	327	1
337	327	1
329	328	1
340	328	1
339	328	1
338	328	1
330	329	1
341	329	1
340	329	1
339	329	1
331	330	1
342	330	1
341	330	1
340	330	1
332	331	1
343	331	1
342	331	1
341	331	1
343	332	1
342	332	1
334	333	1
345	333	1
344	333	1
335	334	1
346	334	1
345	334	1
344	334	1
336	335	1
347	335	1
346	335	1
345	335	1
337	336	1
348	336	1
347	336	1
346	336	1
338	337	1
349	337	1
348	337	1
347	337	1
339	338	1
350	338	1
349	338	1
348	338	1
340	339	1
351	339	1
350	339	1
349	339	1
341	340	1
352	340	1
351	340	1
350	340	1
342	341	1
353	341	1
352	341	1
351	341	1
343	342	1
354	342	1
353	342	1
352	342	1
354	343	1
353	343	1
345	344	1
366	344	1
365	344	1
346	345	1
367	345	1
366	345	1
365	345	1
347	346	1
368	346	1
367	346	1
366	346	1
348	347	1
369	347	1
368	347	1
367	347	1
349	348	1
370	348	1
369	348	1
368	348	1
350	349	1
371	349	1
370	349	1
369	349	1
351	350	1
372	350	1
371	350	1
370	350	1
352	351	1
373	351	1
372	351	1
371	351	1
353	352	1
374	352	1
373	352	1
372	352	1
354	353	1
375	353	1
374	353	1
373	353	1
375	354	1
374	354	1
356	355	1
377	355	1
376	355	1
357	356	1
378	356	1
377	356	1
376	356	1
358	357	1
379	357	1
378	357	1
377	357	1
359	358	1
380	358	1
379	358	1
378	358	1
360	359	1
381	359	1
380	359	1
379	359	1
361	360	1
382	360	1
381	360	1
380	360	1
362	361	1
383	361	1
382	361	1
381	361	1
363	362	1
384	362	1
383	362	1
382	362	1
364	363	1
385	363	1
384	363	1
383	363	1
365	364	1
386	364	1
385	364	1
384	364	1
366	365	1
387	365	1
386	365	1
385	365	1
367	366	1
388	366	1
387	366	1
386	366	1
368	367	1
389	367	1
388	367	1
387	367	1
369	368	1
390	368	1
389	368	1
388	368	1
370	369	1
391	369	1
390	369	1
389	369	1
371	370	1
392	370	1
391	370	1
390	370	1
372	371	1
393	371	1
392	371	1
391	371	1
373	372	1
394	372	1
393	372	1
392	372	1
374	373	1
395	373	1
394	373	1
393	373	1
375	374	1
396	374	1
395	374	1
394	374	1
396	375	1
395	375	1
377	376	1
398	376	1
397	376	1
378	377	1
399	377	1
398	377	1
397	377	1
379	378	1
400	378	1
399	378	1
398	378	1
380	379	1
401	379	1
400	379	1
399	379	1
381	380	1
402	380	1
401	380	1
400	380	1
382	381	1
403	381	1
402	381	1
401	381	1
383	382	1
404	382	1
403	382	1
402	382	1
384	383	1
405	383	1
404	383	1
403	383	1
385	384	1
406	384	1
405	384	1
404	384	1
386	385	1
407	385	1
406	385	1
405	385	1
387	386	1
408	386	1
407	386	1
406	386	1
388	387	1
409	387	1
408	387	1
407	387	1
389	388	1
410	388	1
409	388	1
408	388	1
390	389	1
411	389	1
410	389	1
409	389	1
391	390	1
412	390	1
411	390	1
410	390	1
392	391	1
413	391	1
412	391	1
411	391	1
393	392	1
414	392	1
413	392	1
412	392	1
394	393	1
415	393	1
414	393	1
413	393	1
395	394	1
416	394	1
415	394	1
414	394	1
396	395	1
417	395	1
416	395	1
415	395	1
417	396	1
416	396	1
398	397	1
419	397	1
418	397	1
399	398	1
420	398	1
419	398	1
418	398	1
400	399	1
421	399	1
420	399	1
419	399	1
401	400	1
422	400	1
421	400	1
420	400	1
402	401	1
423	401	1
422	401	1
421	401	1
403	402	1
424	402	1
423	402	1
422	402	1
404	403	1
425	403	1
424	403	1
423	403	1
405	404	1
426	404	1
425	404	1
424	404	1
406	405	1
427	405	1
426	405	1
425	405	1
407	406	1
428	406	1
427	406	1
426	406	1
408	407	1
429	407	1
428	407	1
427	407	1
409	408	1
430	408	1
429	408	1
428	408	1
410	409	1
431	409	1
430	409	1
429	409	1
411	410	1
432	410	1
431	410	1
430	410	1
412	411	1
433	411	1
432	411	1
431	411	1
413	412	1
434	412	1
433	412	1
432	412	1
414	413	1
435	413	1
434	413	1
433	413	1
415	414	1
436	414	1
435	414	1
434	414	1
416	415	1
437	415	1
436	415	1
435	415	1
417	416	1
438	416	1
437	416	1
436	416	1
438	417	1
437	417	1
419	418	1
440	418	1
439	418	1
420	419	1
441	419	1
440	419	1
439	419	1
421	420	1
442	420	1
441	420	1
440	420	1
422	421	1
443	421	1
442	421	1
441	421	1
423	422	1
444	422	1
443	422	1
442	422	1
424	423	1
445	423	1
444	423	1
443	423	1
425	424	1
446	424	1
445	424	1
444	424	1
426	425	1
447	425	1
446	425	1
445	425	1
427	426	1
448	426	1
447	426	1
446	426	1
428	427	1
449	427	1
448	427	1
447	427	1
429	428	1
450	428	1
449	428	1
448	428	1
430	429	1
451	429	1
450	429	1
449	429	1
431	430	1
452	430	1
451	430	1
450	430	1
432	431	1
453	431	1
452	431	1
451	431	1
433	432	1
454	432	1
453	432	1
452	432	1
434	433	1
455	433	1
454	433	1
453	433	1
435	434	1
456	434	1
455	434	1
454	434	1
436	435	1
457	435	1
456	435	1
455	435	1
437	436	1
458	436	1
457	436	1
456	436	1
438	437	1
459	437	1
458	437	1
457	437	1
459	438	1
458	438	1
440	439	1
461	439	1
460	439	1
441	440	1
462	440	1
461	440	1
460	440	1
442	441	1
463	441	1
462	441	1
461	441	1
443	442	1
464	442	1
463	442	1
462	442	1
444	443	1
465	443	1
464	443	1
463	443	1
445	444	1
466	444	1
465	444	1
464	444	1
446	445	1
467	445	1
466	445	1
465	445	1
447	446	1
468	446	1
467	446	1
466	446	1
448	447	1
469	447	1
468	447	1
467	447	1
449	448	1
470	448	1
469	448	1
468	448	1
450	449	1
471	449	1
470	449	1
469	449	1
451	450	1
472	450	1
471	450	1
470	450	1
452	451	1
473	451	1
472	451	1
471	451	1
453	452	1
474	452	1
473	452	1
472	452	1
454	453	1
475	453	1
474	453	1
473	453	1
455	454	1
476	454	1
475	454	1
474	454	1
456	455	1
477	455	1
476	455	1
475	455	1
457	456	1
478	456	1
477	456	1
476	456	1
458	457	1
479	457	1
478	457	1
477	457	1
459	458	1
480	458	1
479	458	1
478	458	1
480	459	1
479	459	1
461	460	1
482	460	1
481	460	1
462	461	1
483	461	1
482	461	1
481	461	1
463	462	1
484	462	1
483	462	1
482	462	1
464	463	1
485	463	1
484	463	1
483	463	1
465	464	1
486	464	1
485	464	1
484	464	1
466	465	1
487	465	1
486	465	1
485	465	1
467	466	1
488	466	1
487	466	1
486	466	1
468	467	1
489	467	1
488	467	1
487	467	1
469	468	1
490	468	1
489	468	1
488	468	1
470	469	1
491	469	1
490	469	1
489	469	1
471	470	1
492	470	1
491	470	1
490	470	1
472	471	1
493	471	1
492	471	1
491	471	1
473	472	1
494	472	1
493	472	1
492	472	1
474	473	1
495	473	1
494	473	1
493	473	1
475	474	1
496	474	1
495	474	1
494	474	1
476	475	1
497	475	1
496	475	1
495	475	1
477	476	1
498	476	1
497	476	1
496	476	1
478	477	1
499	477	1
498	477	1
497	477	1
479	478	1
500	478	1
499	478	1
498	478	1
480	479	1
501	479	1
500	479	1
499	479	1
501	480	1
500	480	1
482	481	1
503	481	1
502	481	1
483	482	1
504	482	1
503	482	1
502	482	1
484	483	1
505	483	1
504	483	1
503	483	1
485	484	1
506	484	1
505	484	1
504	484	1
486	485	1
507	485	1
506	485	1
505	485	1
487	486	1
508	486	1
507	486	1
506	486	1
488	487	1
509	487	1
508	487	1
507	487	1
489	488	1
510	488	1
509	488	1
508	488	1
490	489	1
511	489	1
510	489	1
509	489	1
491	490	1
512	490	1
511	490	1
510	490	1
492	491	1
513	491	1
512	491	1
511	491	1
493	492	1
514	492	1
513	492	1
512	492	1
494	493	1
515	493	1
514	493	1
513	493	1
495	494	1
516	494	1
515	494	1
514	494	1
496	495	1
517	495	1
516	495	1
515	495	1
497	496	1
518	496	1
517	496	1
516	496	1
498	497	1
519	497	1
518	497	1
517	497	1
499	498	1
520	498	1
519	498	1
518	498	1
500	499	1
521	499	1
520	499	1
519	499	1
501	500	1
522	500	1
521	500	1
520	500	1
522	501	1
521	501	1
503	502	1
524	502	1
523	502	1
504	503	1
525	503	1
524	503	1
523	503	1
505	504	1
526	504	1
525	504	1
524	504	1
506	505	1
527	505	1
526	505	1
525	505	1
507	506	1
528	506	1
527	506	1
526	506	1
508	507	1
529	507	1
528	507	1
527	507	1
509	508	1
530	508	1
529	508	1
528	508	1
510	509	1
531	509	1
530	509	1
529	509	1
511	510	1
532	510	1
531	510	1
530	510	1
512	511	1
533	511	1
532	511	1
531	511	1
513	512	1
534	512	1
533	512	1
532	512	1
514	513	1
535	513	1
534	513	1
533	513	1
515	514	1
536	514	1
535	514	1
534	514	1
516	515	1
537	515	1
536	515	1
535	515	1
517	516	1
538	516	1
537	516	1
536	516	1
518	517	1
539	517	1
538	517	1
537	517	1
519	518	1
540	518	1
539	518	1
538	518	1
520	519	1
541	519	1
540	519	1
539	519	1
521	520	1
542	520	1
541	520	1
540	520	1
522	521	1
543	521	1
542	521	1
541	521	1
543	522	1
542	522	1
524	523	1
525	524	1
526	525	1
527	526	1
528	527	1
529	528	1
530	529	1
531	530	1
532	531	1
533	532	1
534	533	1
535	534	1
536	535	1
537	536	1
538	537	1
539	538	1
540	539	1
541	540	1
542	541	1
543	542	1
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
22	1	0
22	2	0
43	1	0
43	2	0
64	1	0
64	2	0
85	1	0
85	2	0
106	1	0
106	2	0
127	1	0
127	2	0
148	1	0
148	2	0
169	1	0
169	2	0
355	1	0
355	2	0
376	1	0
376	2	0
397	1	0
397	2	0
418	1	0
418	2	0
439	1	0
439	2	0
460	1	0
460	2	0
481	1	0
481	2	0
502	1	0
502	2	0
523	1	0
523	2	0
];
% Nodal loads: node_id, degree of freedom (1 - x, 2 - y), load
loads = [
223	2	-0.01
];
% Control parameters
plotdof = 2;
