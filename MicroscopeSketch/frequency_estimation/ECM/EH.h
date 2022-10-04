#ifndef SLIDING_TOPK_EH_H
#define SLIDING_TOPK_EH_H
#include "../inc.h"
const int CellSize = 6;
class Cell {
public:
    int t;              // 4 bytes
    int exp;            // 2 bytes
    Cell() :t(-1), exp(-1) {}
    void clear() {
        t = -1;
        exp = -1;
    }
    void init(int _t) {
        t = _t;
        exp = 0;
    }
    void copyFrom(const Cell& c) {
        t = c.t;
        exp = c.exp;
    }
};


class EH {
    int k;      // k=2
    int N;      // N is window size
    int m;      // number of cells (based on k and N)

    int c;      // the number of cells used
    int c_beg;  // the index of the oldest cell

    Cell* cell;

public:
    EH(int _k, int _N) : k(_k), N(_N) {
        m = ceil(1.0 + k * 0.5) * (2.0 + log(2.0 * N / k) / log(2.0)) + 1;
//        printf("m = %d\n", m);
        c = 0;
        c_beg = 0;
        cell = new Cell[m];
    }
    ~EH() {
        delete[] cell;
    }

    void expire(int t) {     // t is the current time
        if (c == 0)
            return;

        int expired_cnt = 0;
        for (int i = 0; i < c; ++i) {
            int c_i = (c_beg + i) % m;
            if (cell[c_i].t <= t - N) {
                cell[c_i].clear();
                expired_cnt++;
            }
            else break;
        }

        c -= expired_cnt;
        c_beg = (c_beg + expired_cnt) % m;
    }

    void insert(int t) {
        /* if there is no cell */
        if (c == 0) {
            cell[c_beg].init(t);
            c++;
            return;
        }

        /* add a new cell in the end */
        cell[(c_beg + c) % m].init(t);
        c++;

        /* if needed, merge */
        merge();
    }

private:
    void merge() {
        if (c <= k + 1)
            return;

        /* count 1 */
        int cnt = 0;
        for (int i = c - 1; i >= 0; i--) {
            int c_i = (c_beg + i) % m;
            if (cell[c_i].exp != 0)
                break;
            cnt++;
        }
        if (cnt <= k + 1)
            return;
        cell[(c_beg + c - cnt + m) % m].exp = 1;
        cell[(c_beg + c - cnt + m) % m].t = cell[(c_beg + c - cnt + 1 + m) % m].t;
        for (int i = cnt - 1; i > 1; i--)
            cell[(c_beg + c - i + m) % m].copyFrom(cell[(c_beg + c - i + 1 + m) % m]);
        cell[(c_beg + c - 1 + m) % m].clear();



        /* count 1~max_val */
        int max_val = cell[c_beg].exp;
        int c_end = (c_beg + c - cnt + m) % m;
        int val = cell[c_end].exp;
        c--;

        while (val <= max_val) {
            cnt = 0;
            for (int i = c_end; cell[i].exp == val; i = (i - 1 + m) % m)
                cnt++;
            if (cnt <= k / 2 + 1)
                return;

            cell[(c_end - cnt + 1 + m) % m].exp = val + 1;
            cell[(c_end - cnt + 1 + m) % m].t = cell[(c_end - cnt + 1 + m + 1) % m].t;
            for (int i = (c_end - cnt + 2 + m) % m; i != (c_beg + c - 1) % m; i = (i + 1) % m)
                cell[i].copyFrom(cell[(i + 1) % m]);
            cell[(c_beg + c - 1) % m].clear();

            c_end = (c_end - cnt + 1 + m) % m;
            val = val + 1;
            c--;
        }
    }

public:
    int query() {
        int ret = 0;
        for (int i = 0; i < c; ++i) {
            ret += pow(2, cell[(c_beg + i) % m].exp);
        }
        return ret;
    }

public:
    int memory() {
        return CellSize * m;
    }
    static int memory(int k, int N) {
        return CellSize * (ceil(1.0 + k * 0.5) * (2.0 + log(2.0 * N / k) / log(2.0)) + 1);
    } 
};


#endif //SLIDING_TOPK_EH_H
