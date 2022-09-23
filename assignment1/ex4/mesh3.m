% Created with:       FlExtract v1.13
% Element type:       truss
% Number of nodes:    220
% Number of elements: 759

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
0	0.1
0	0.2
0	0.3
0	0.4
0	0.5
0	0.6
0	0.7
0	0.8
0	0.9
0	1
0.0833333	0
0.0833333	0.1
0.0833333	0.2
0.0833333	0.3
0.0833333	0.4
0.0833333	0.5
0.0833333	0.6
0.0833333	0.7
0.0833333	0.8
0.0833333	0.9
0.0833333	1
0.166667	0
0.166667	0.1
0.166667	0.2
0.166667	0.3
0.166667	0.4
0.166667	0.5
0.166667	0.6
0.166667	0.7
0.166667	0.8
0.166667	0.9
0.166667	1
0.25	0
0.25	0.1
0.25	0.2
0.25	0.3
0.25	0.4
0.25	0.5
0.25	0.6
0.25	0.7
0.25	0.8
0.25	0.9
0.25	1
0.333333	0
0.333333	0.1
0.333333	0.2
0.333333	0.3
0.333333	0.4
0.333333	0.5
0.333333	0.6
0.333333	0.7
0.333333	0.8
0.333333	0.9
0.333333	1
0.416667	0
0.416667	0.1
0.416667	0.2
0.416667	0.3
0.416667	0.4
0.416667	0.5
0.416667	0.6
0.416667	0.7
0.416667	0.8
0.416667	0.9
0.416667	1
0.5	0
0.5	0.1
0.5	0.2
0.5	0.3
0.5	0.4
0.5	0.5
0.5	0.6
0.5	0.7
0.5	0.8
0.5	0.9
0.5	1
0.583333	0.5
0.583333	0.6
0.583333	0.7
0.583333	0.8
0.583333	0.9
0.583333	1
0.666667	0.5
0.666667	0.6
0.666667	0.7
0.666667	0.8
0.666667	0.9
0.666667	1
0.75	0.5
0.75	0.6
0.75	0.7
0.75	0.8
0.75	0.9
0.75	1
0.833333	0.5
0.833333	0.6
0.833333	0.7
0.833333	0.8
0.833333	0.9
0.833333	1
0.916667	0.5
0.916667	0.6
0.916667	0.7
0.916667	0.8
0.916667	0.9
0.916667	1
1	0.5
1	0.6
1	0.7
1	0.8
1	0.9
1	1
1.08333	0.5
1.08333	0.6
1.08333	0.7
1.08333	0.8
1.08333	0.9
1.08333	1
1.16667	0.5
1.16667	0.6
1.16667	0.7
1.16667	0.8
1.16667	0.9
1.16667	1
1.25	0.5
1.25	0.6
1.25	0.7
1.25	0.8
1.25	0.9
1.25	1
1.33333	0.5
1.33333	0.6
1.33333	0.7
1.33333	0.8
1.33333	0.9
1.33333	1
1.41667	0.5
1.41667	0.6
1.41667	0.7
1.41667	0.8
1.41667	0.9
1.41667	1
1.5	0
1.5	0.1
1.5	0.2
1.5	0.3
1.5	0.4
1.5	0.5
1.5	0.6
1.5	0.7
1.5	0.8
1.5	0.9
1.5	1
1.58333	0
1.58333	0.1
1.58333	0.2
1.58333	0.3
1.58333	0.4
1.58333	0.5
1.58333	0.6
1.58333	0.7
1.58333	0.8
1.58333	0.9
1.58333	1
1.66667	0
1.66667	0.1
1.66667	0.2
1.66667	0.3
1.66667	0.4
1.66667	0.5
1.66667	0.6
1.66667	0.7
1.66667	0.8
1.66667	0.9
1.66667	1
1.75	0
1.75	0.1
1.75	0.2
1.75	0.3
1.75	0.4
1.75	0.5
1.75	0.6
1.75	0.7
1.75	0.8
1.75	0.9
1.75	1
1.83333	0
1.83333	0.1
1.83333	0.2
1.83333	0.3
1.83333	0.4
1.83333	0.5
1.83333	0.6
1.83333	0.7
1.83333	0.8
1.83333	0.9
1.83333	1
1.91667	0
1.91667	0.1
1.91667	0.2
1.91667	0.3
1.91667	0.4
1.91667	0.5
1.91667	0.6
1.91667	0.7
1.91667	0.8
1.91667	0.9
1.91667	1
2	0
2	0.1
2	0.2
2	0.3
2	0.4
2	0.5
2	0.6
2	0.7
2	0.8
2	0.9
2	1
];
% Element connectivity: node1_id, node2_id, material_id
IX = [
2	1	1
13	1	1
12	1	1
3	2	1
14	2	1
13	2	1
12	2	1
4	3	1
15	3	1
14	3	1
13	3	1
5	4	1
16	4	1
15	4	1
14	4	1
6	5	1
17	5	1
16	5	1
15	5	1
7	6	1
18	6	1
17	6	1
16	6	1
8	7	1
19	7	1
18	7	1
17	7	1
9	8	1
20	8	1
19	8	1
18	8	1
10	9	1
21	9	1
20	9	1
19	9	1
11	10	1
22	10	1
21	10	1
20	10	1
22	11	1
21	11	1
13	12	1
24	12	1
23	12	1
14	13	1
25	13	1
24	13	1
23	13	1
15	14	1
26	14	1
25	14	1
24	14	1
16	15	1
27	15	1
26	15	1
25	15	1
17	16	1
28	16	1
27	16	1
26	16	1
18	17	1
29	17	1
28	17	1
27	17	1
19	18	1
30	18	1
29	18	1
28	18	1
20	19	1
31	19	1
30	19	1
29	19	1
21	20	1
32	20	1
31	20	1
30	20	1
22	21	1
33	21	1
32	21	1
31	21	1
33	22	1
32	22	1
24	23	1
35	23	1
34	23	1
25	24	1
36	24	1
35	24	1
34	24	1
26	25	1
37	25	1
36	25	1
35	25	1
27	26	1
38	26	1
37	26	1
36	26	1
28	27	1
39	27	1
38	27	1
37	27	1
29	28	1
40	28	1
39	28	1
38	28	1
30	29	1
41	29	1
40	29	1
39	29	1
31	30	1
42	30	1
41	30	1
40	30	1
32	31	1
43	31	1
42	31	1
41	31	1
33	32	1
44	32	1
43	32	1
42	32	1
44	33	1
43	33	1
35	34	1
46	34	1
45	34	1
36	35	1
47	35	1
46	35	1
45	35	1
37	36	1
48	36	1
47	36	1
46	36	1
38	37	1
49	37	1
48	37	1
47	37	1
39	38	1
50	38	1
49	38	1
48	38	1
40	39	1
51	39	1
50	39	1
49	39	1
41	40	1
52	40	1
51	40	1
50	40	1
42	41	1
53	41	1
52	41	1
51	41	1
43	42	1
54	42	1
53	42	1
52	42	1
44	43	1
55	43	1
54	43	1
53	43	1
55	44	1
54	44	1
46	45	1
57	45	1
56	45	1
47	46	1
58	46	1
57	46	1
56	46	1
48	47	1
59	47	1
58	47	1
57	47	1
49	48	1
60	48	1
59	48	1
58	48	1
50	49	1
61	49	1
60	49	1
59	49	1
51	50	1
62	50	1
61	50	1
60	50	1
52	51	1
63	51	1
62	51	1
61	51	1
53	52	1
64	52	1
63	52	1
62	52	1
54	53	1
65	53	1
64	53	1
63	53	1
55	54	1
66	54	1
65	54	1
64	54	1
66	55	1
65	55	1
57	56	1
68	56	1
67	56	1
58	57	1
69	57	1
68	57	1
67	57	1
59	58	1
70	58	1
69	58	1
68	58	1
60	59	1
71	59	1
70	59	1
69	59	1
61	60	1
72	60	1
71	60	1
70	60	1
62	61	1
73	61	1
72	61	1
71	61	1
63	62	1
74	62	1
73	62	1
72	62	1
64	63	1
75	63	1
74	63	1
73	63	1
65	64	1
76	64	1
75	64	1
74	64	1
66	65	1
77	65	1
76	65	1
75	65	1
77	66	1
76	66	1
68	67	1
69	68	1
70	69	1
71	70	1
72	71	1
73	72	1
79	72	1
78	72	1
74	73	1
80	73	1
79	73	1
78	73	1
75	74	1
81	74	1
80	74	1
79	74	1
76	75	1
82	75	1
81	75	1
80	75	1
77	76	1
83	76	1
82	76	1
81	76	1
83	77	1
82	77	1
79	78	1
85	78	1
84	78	1
80	79	1
86	79	1
85	79	1
84	79	1
81	80	1
87	80	1
86	80	1
85	80	1
82	81	1
88	81	1
87	81	1
86	81	1
83	82	1
89	82	1
88	82	1
87	82	1
89	83	1
88	83	1
85	84	1
91	84	1
90	84	1
86	85	1
92	85	1
91	85	1
90	85	1
87	86	1
93	86	1
92	86	1
91	86	1
88	87	1
94	87	1
93	87	1
92	87	1
89	88	1
95	88	1
94	88	1
93	88	1
95	89	1
94	89	1
91	90	1
97	90	1
96	90	1
92	91	1
98	91	1
97	91	1
96	91	1
93	92	1
99	92	1
98	92	1
97	92	1
94	93	1
100	93	1
99	93	1
98	93	1
95	94	1
101	94	1
100	94	1
99	94	1
101	95	1
100	95	1
97	96	1
103	96	1
102	96	1
98	97	1
104	97	1
103	97	1
102	97	1
99	98	1
105	98	1
104	98	1
103	98	1
100	99	1
106	99	1
105	99	1
104	99	1
101	100	1
107	100	1
106	100	1
105	100	1
107	101	1
106	101	1
103	102	1
109	102	1
108	102	1
104	103	1
110	103	1
109	103	1
108	103	1
105	104	1
111	104	1
110	104	1
109	104	1
106	105	1
112	105	1
111	105	1
110	105	1
107	106	1
113	106	1
112	106	1
111	106	1
113	107	1
112	107	1
109	108	1
115	108	1
114	108	1
110	109	1
116	109	1
115	109	1
114	109	1
111	110	1
117	110	1
116	110	1
115	110	1
112	111	1
118	111	1
117	111	1
116	111	1
113	112	1
119	112	1
118	112	1
117	112	1
119	113	1
118	113	1
115	114	1
121	114	1
120	114	1
116	115	1
122	115	1
121	115	1
120	115	1
117	116	1
123	116	1
122	116	1
121	116	1
118	117	1
124	117	1
123	117	1
122	117	1
119	118	1
125	118	1
124	118	1
123	118	1
125	119	1
124	119	1
121	120	1
127	120	1
126	120	1
122	121	1
128	121	1
127	121	1
126	121	1
123	122	1
129	122	1
128	122	1
127	122	1
124	123	1
130	123	1
129	123	1
128	123	1
125	124	1
131	124	1
130	124	1
129	124	1
131	125	1
130	125	1
127	126	1
133	126	1
132	126	1
128	127	1
134	127	1
133	127	1
132	127	1
129	128	1
135	128	1
134	128	1
133	128	1
130	129	1
136	129	1
135	129	1
134	129	1
131	130	1
137	130	1
136	130	1
135	130	1
137	131	1
136	131	1
133	132	1
139	132	1
138	132	1
134	133	1
140	133	1
139	133	1
138	133	1
135	134	1
141	134	1
140	134	1
139	134	1
136	135	1
142	135	1
141	135	1
140	135	1
137	136	1
143	136	1
142	136	1
141	136	1
143	137	1
142	137	1
139	138	1
150	138	1
149	138	1
140	139	1
151	139	1
150	139	1
149	139	1
141	140	1
152	140	1
151	140	1
150	140	1
142	141	1
153	141	1
152	141	1
151	141	1
143	142	1
154	142	1
153	142	1
152	142	1
154	143	1
153	143	1
145	144	1
156	144	1
155	144	1
146	145	1
157	145	1
156	145	1
155	145	1
147	146	1
158	146	1
157	146	1
156	146	1
148	147	1
159	147	1
158	147	1
157	147	1
149	148	1
160	148	1
159	148	1
158	148	1
150	149	1
161	149	1
160	149	1
159	149	1
151	150	1
162	150	1
161	150	1
160	150	1
152	151	1
163	151	1
162	151	1
161	151	1
153	152	1
164	152	1
163	152	1
162	152	1
154	153	1
165	153	1
164	153	1
163	153	1
165	154	1
164	154	1
156	155	1
167	155	1
166	155	1
157	156	1
168	156	1
167	156	1
166	156	1
158	157	1
169	157	1
168	157	1
167	157	1
159	158	1
170	158	1
169	158	1
168	158	1
160	159	1
171	159	1
170	159	1
169	159	1
161	160	1
172	160	1
171	160	1
170	160	1
162	161	1
173	161	1
172	161	1
171	161	1
163	162	1
174	162	1
173	162	1
172	162	1
164	163	1
175	163	1
174	163	1
173	163	1
165	164	1
176	164	1
175	164	1
174	164	1
176	165	1
175	165	1
167	166	1
178	166	1
177	166	1
168	167	1
179	167	1
178	167	1
177	167	1
169	168	1
180	168	1
179	168	1
178	168	1
170	169	1
181	169	1
180	169	1
179	169	1
171	170	1
182	170	1
181	170	1
180	170	1
172	171	1
183	171	1
182	171	1
181	171	1
173	172	1
184	172	1
183	172	1
182	172	1
174	173	1
185	173	1
184	173	1
183	173	1
175	174	1
186	174	1
185	174	1
184	174	1
176	175	1
187	175	1
186	175	1
185	175	1
187	176	1
186	176	1
178	177	1
189	177	1
188	177	1
179	178	1
190	178	1
189	178	1
188	178	1
180	179	1
191	179	1
190	179	1
189	179	1
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
198	187	1
197	187	1
189	188	1
200	188	1
199	188	1
190	189	1
201	189	1
200	189	1
199	189	1
191	190	1
202	190	1
201	190	1
200	190	1
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
209	198	1
208	198	1
200	199	1
211	199	1
210	199	1
201	200	1
212	200	1
211	200	1
210	200	1
202	201	1
213	201	1
212	201	1
211	201	1
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
220	209	1
219	209	1
211	210	1
212	211	1
213	212	1
214	213	1
215	214	1
216	215	1
217	216	1
218	217	1
219	218	1
220	219	1
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
12	1	0
12	2	0
23	1	0
23	2	0
34	1	0
34	2	0
45	1	0
45	2	0
56	1	0
56	2	0
67	1	0
67	2	0
144	1	0
144	2	0
155	1	0
155	2	0
166	1	0
166	2	0
177	1	0
177	2	0
188	1	0
188	2	0
199	1	0
199	2	0
210	1	0
210	2	0
];
% Nodal loads: node_id, degree of freedom (1 - x, 2 - y), load
loads = [
90	2	-0.01
];
% Control parameters
plotdof = 2;