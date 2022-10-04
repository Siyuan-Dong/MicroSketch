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
	cout << "Speed" << endl;
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
			sketch->Insert((uint8_t *)(addr + pkt), TUPLE_LEN, pkt);
		}

		clock_t ed = clock();
		cout << 1. * (ed - st) / CLOCKS_PER_SEC / (NUM * WIN) << endl;
	
	}
	   
    return 0;
}