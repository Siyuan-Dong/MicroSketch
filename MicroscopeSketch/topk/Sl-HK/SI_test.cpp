#include <bits/stdc++.h>
#include "BOBHash32.h"
#include "heavykeeper.h"
using namespace std;
#define fi first
#define se second
#define mp make_pair
#define pb push_back
#define SZ(x) (int((x).size()))
#define All(x) (x).begin(), (x).end()
#define rep2(i,a,b) for(int (i)=(a);(i)<(b);(i)++)
typedef pair<int,int> pii;
typedef pair<string, int> psi;
const int QK = 500;
const int TRACE_LEN = 16;
const int TUPLE_LEN = 4;
const int WIN = 1000000;
const int NUM = 3, LIM = 500;
uint32_t addr[NUM * WIN];
uint32_t strrep[WIN];
map<uint32_t, int> strmap;

double cnt = 0, sum = 0;

bool cmp(const pii & A, const pii & B) {
    return A.se > B.se;
}

void load_CAIDA() {
	char CAIDA[100];
	sprintf(CAIDA, "E:\\DataSet\\CAIDA\\formatted00.dat");
	ifstream fin(CAIDA, ios::binary);
    uint8_t key[TRACE_LEN];
    rep2 (pkt, 0, NUM * WIN) {
        fin.read((char *)key, TRACE_LEN);
        memcpy(addr+pkt, key+8, TUPLE_LEN);
    }
}

int main() {
    load_CAIDA();
	srand(time(0));
	freopen("output.csv", "w", stdout);
	cout << "ARE,RR" << endl;
	for (int mem = 80; mem <= 640; mem <<= 1) {
		strmap.clear();
		int field_num = 2;
		int single_size = 8 + (4 * (field_num + 1)); // bytes
		int hk_M = mem * 1024 / single_size;
		heavykeeper *sketch = new heavykeeper(hk_M, WIN, field_num);
		sketch->clear();
		
		double totare = 0, totrecall = 0;
		int cnt = 0;

		clock_t st = clock();

		rep2 (pkt, 0, NUM * WIN) {
			uint32_t tuple = addr[pkt];
			if (pkt >= WIN) strmap[strrep[pkt % WIN]] -= 1;
			strrep[pkt % WIN] = tuple;
			strmap[tuple] += 1;
	
			sketch->Insert((uint8_t *)(addr + pkt), TUPLE_LEN, pkt);

	       
			if ((pkt + 1) >= WIN && (pkt + 1) % (WIN / 100) == 0) {
				vector<pii> topk1, topk2;
				for (auto &it: strmap) {
					topk1.push_back(it);
					topk2.push_back(mp(it.fi, sketch->num_query((uint8_t *) &it.fi, TUPLE_LEN)));
				}
				sort(All(topk1), cmp);
				sort(All(topk2), cmp);
				topk2.resize(QK);
				double same = 0, aee = 0, are = 0;
				rep2 (i, 0, QK) rep2 (j, 0, SZ(topk2)) {
					if (topk1[i].fi == topk2[j].fi) {
						same++;
						aee += abs(topk1[i].se - topk2[j].se);
						are += 1.0 * abs(topk1[i].se - topk2[j].se) / topk1[i].se;
						break;
					}
				}
				aee /= same; are /= same;
				double recall = same / QK;
				totare += are;
				totrecall += recall;
				cnt++;
			}
		}

		clock_t ed = clock();

		cout << totare / cnt << "," << totrecall / cnt << endl;

//		cout << 1. * (ed - st) / CLOCKS_PER_SEC / (NUM * WIN) << endl;
	
	}
	   
    return 0;
}