#include "../MicroSketch-CM.h"
const int M = 6e3, T = 1, H = 3, size_k = 4, log_base = 1;
const int TRACE_LEN = 16;
const int TUPLE_LEN = 4;
const int MAXINPUT = 1e7;
const int WIN = 1e6;
const int NUM = 3, LIM = 500;
uint32_t addr[MAXINPUT];
uint64_t timestamp[MAXINPUT];
pil strrep[MAXINPUT];
map<uint32_t, int> strmap;

uint64_t calc(uint32_t a, uint32_t b) {
	return 1ll * a * 1000000 + b;
}

void load_CAIDA(string data_path,int TUPLE_L,int TRACE_L) {
	char CAIDA[100];
	sprintf(CAIDA, data_path.c_str());
	ifstream fin(CAIDA, ios::binary);
    uint8_t key[TRACE_L];
    rep2 (pkt, 0, MAXINPUT) {
        fin.read((char *)key, TRACE_L);
        memcpy(addr+pkt, key+8, TUPLE_L);
//		timestamp[pkt] = calc(*((uint32_t *) key), *((uint32_t *) key+1));
		timestamp[pkt] = pkt;
    }
}
void load_IMC(string data_path,int TUPLE_L,int TRACE_L) {
	
	BOBHash32 * hash = new BOBHash32(uint8_t(rd() % MAX_PRIME32));
	char IMC[100];
	sprintf(IMC, data_path.c_str());
	ifstream fin(IMC, ios::binary);
    uint8_t key[TRACE_L];
    rep2 (pkt, 0, MAXINPUT) {
        fin.read((char *)key, TRACE_L);
        addr[pkt]= hash->run((char *) key, 13);
		timestamp[pkt] = pkt;
    }
}


void load_zipf(string data_path,int TUPLE_L,int TRACE_L) {
	int alpha=30;
	char zipf[100];
	sprintf(zipf, data_path.c_str());
	ifstream fin(zipf, ios::binary);
    uint8_t key[TRACE_L];
    rep2 (pkt, 0, MAXINPUT) {
        fin.read((char *)key, TRACE_L);
        memcpy(addr+pkt, key, TUPLE_L);
		timestamp[pkt] = pkt;
    }
}

int main(int argc, char* argv[]) {
    string data_path;
    int TUPLE_L,TRACE_L;
	if(string(argv[1])=="caida"){
        data_path = "/share/datasets/CAIDA2018/dataset/130000.dat";
        TUPLE_L = 4;
        TRACE_L = 16;
		load_CAIDA(data_path,TUPLE_L,TRACE_L);
    }else if(string(argv[1])=="imc"){
        data_path = "/root/work/data/20.dat";
        TUPLE_L = 4;
        TRACE_L = 26;
		load_IMC(data_path,TUPLE_L,TRACE_L);
    }else if(string(argv[1])=="webpage"){
        data_path = "/share/datasets/DataSet/webpage/webdocs_form04.dat";
        TUPLE_L = 4;
        TRACE_L = 4;
		load_zipf(data_path,TUPLE_L,TRACE_L);
    }else if(string(argv[1])=="zipf003"){
        data_path = "/share/datasets/DataSet/new_zipf/003.dat";
        TUPLE_L = 4;
        TRACE_L = 4;
		load_zipf(data_path,TUPLE_L,TRACE_L);
    }else{
        data_path = "/share/datasets/DataSet/new_zipf/030.dat";
        TUPLE_L = 4;
        TRACE_L = 4;
		load_zipf(data_path,TUPLE_L,TRACE_L);
    }
    // load_CAIDA();
	srand(time(0));
	string tm = string(argv[1])+"/speed_MicroCM.csv";
	fstream fout(tm.c_str(),ios::app);

	// freopen("output.csv", "w", stdout);
	// fout << "mem,aae," << endl;
	// cout << "MicroSketch-CU" << endl;
	int mm = atoi(argv[2]);
	for (int mem = mm; mem <= mm; mem += 100) {
	// for (int mem = 100; mem <= 500; mem += 100) {
		double mops = 0;
		for (int test = 1; test <= 40; test++) {
			strmap.clear();
			int M = mem * 1024 * 8 / H / (32-size_k + (T+2)*size_k + ceil(5 - log(log_base)/log(2)));
			auto sketch = new MicroSketch_CM<TUPLE_LEN>(M, WIN, T, size_k, H, log_base);
			double totaee = 0;
			int cnt = 0, pkt = 0, h = 0, t = 0;

			clock_t st = clock();

			while (pkt < NUM * WIN) {
				sketch->insert((uint8_t *)(addr + pkt), timestamp[pkt]);
				pkt++;
			}

			clock_t ed = clock();
			mops += 1. / (1. * (ed - st) / CLOCKS_PER_SEC / (NUM * WIN)) / 1e6;
		}
		cout << mops / 40 << endl;
		fout << mem<<","<<mops / 40 <<","<< endl;
	}
	fout.close();
    return 0;
}