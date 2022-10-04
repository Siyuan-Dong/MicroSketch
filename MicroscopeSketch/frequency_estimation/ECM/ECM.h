#ifndef _ECM_SKETCH_H_
#define _ECM_SKETCH_H_

#include "EH.h"

const static int MAX_HASH_NUM = 3;

class ECM_Sketch
{
private:
	BOBHash32 *bobhash32[MAX_HASH_NUM];
	EH*** counter;
	int w, d;
	int k;
	int N;
	clock_t totaltime;

public:
	ECM_Sketch(int _w, int _d, int _N, int _k = 2): w(_w), d(_d), N(_N), k(_k){
		totaltime = 0;
		counter = new EH**[d];
		for(int i = 0; i < d; ++i)
		{
			counter[i] = new EH*[w];
			for(int j = 0; j < w; ++j)
				counter[i][j] = new EH(k, N);
			bobhash32[i] = new BOBHash32(100 + i);
		}
	}
	~ECM_Sketch(){
		for(int i = 0; i < d; ++i){
			for(int j = 0; j < w; ++j)
				delete counter[i][j];
			delete[] counter[i];
			delete bobhash32[i];
		}
		delete[] counter;
	}

	void insert(const char *str, int t)
	{
		clock_t st = clock();
		for(int i = 0; i < d; ++i){
			int idx = (bobhash32[i]->run(str, 4)) % w;
			counter[i][idx]->expire(t);
			counter[i][idx]->insert(t);
		}
		clock_t ed = clock();
		totaltime += ed-st;
	}

	int query(const char *str, int t)
	{
		int min_value = 0x3FFFFFFF;
		for(int i = 0; i < d; ++i){
			int idx = (bobhash32[i]->run(str, 4)) % w;
			counter[i][idx]->expire(t);
			int tmp = counter[i][idx]->query();
			if(tmp < min_value)
				min_value = tmp;
		}
		return min_value;
	}

	int memory(){
		EH *eh = new EH(k, N);
		int EH_mem = eh->memory();
		return EH_mem * w * d;
	}
	int time() {
		return totaltime;
	}
};


#endif