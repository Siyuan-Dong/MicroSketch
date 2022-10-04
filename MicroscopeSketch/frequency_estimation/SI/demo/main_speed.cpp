#include <iostream>
#include <fstream>
#include <iomanip>
#include <time.h>
#include <unordered_map>
#include <unordered_set>
#include <cmath>
#include "../clock.cpp"
#include "../data.cpp"
#include "../hash_class.cpp"

using namespace std;
unordered_map<Data, int, My_Hash> mp;
unordered_set<Data, My_Hash> st;

//argv[1]:cycle
//argv[2]:hash_number
//argv[3]:Memory
//argv[4]:file
//argv[5]:input_num_max
//argv[6]:out_file
//argv[7]:out_model
//argv[8]:label_name
//argv[9]:field

#define tpTest 1

void Read_File(int argc,char*argv[]){
    int cycle = 1e6;
    int hash_number = 3;
    // cout << argv[0];
    double mymemory  = atof(argv[1]);
    cout << mymemory;
    int input_num_max = 1e6*3;
    int field_num = 3;
    int row_length = (mymemory * 1000 * 1024) / hash_number / (4 * field_num);
    Recent_Counter CM_Counter(cycle, hash_number * row_length, row_length, hash_number, field_num);
    Recent_Counter CU_Counter(cycle, hash_number * row_length, row_length, hash_number, field_num);
    Recent_Counter CO_Counter(2*cycle/3, hash_number * row_length, row_length, hash_number, field_num);

    Data *dat = new Data[cycle + 1];

    unsigned long long int num = 0;
    double CM_dif = 0, CU_dif = 0, CO_dif = 0;
    double CM_ae = 0,  CU_ae = 0,  CO_ae = 0;
    double CM_re = 0,  CU_re = 0,  CO_re = 0;
    double CM_AEE = 0, CU_AEE = 0;
    int totcnt = 0;
    int Test_num = 0;
    string data_path;
    int TUPLE_L,TRACE_L;
    if(string(argv[2])=="caida"){
        data_path = "/share/datasets/CAIDA2018/dataset/130000.dat";
        TUPLE_L = 4;
        TRACE_L = 16;
    }else if(string(argv[2])=="imc"){
        data_path = "/root/work/data/20.dat";
        TUPLE_L = 4;
        TRACE_L = 26;
    }else if(string(argv[2])=="webpage"){
        data_path = "/share/datasets/DataSet/webpage/webdocs_form04.dat";
        TUPLE_L = 4;
        TRACE_L = 4;
    }else if(string(argv[2])=="zipf003"){
        data_path = "/share/datasets/DataSet/new_zipf/003.dat";
        TUPLE_L = 4;
        TRACE_L = 4;
    }else{
        data_path = "/share/datasets/DataSet/new_zipf/030.dat";
        TUPLE_L = 4;
        TRACE_L = 4;
    }
    cout<<" "<<data_path<<endl;
    FILE* file = fopen(data_path.c_str(),"rb");
    // FILE* file = fopen(argv[2],"rb");
    Data packet;


    cout <<"Sl-CM,Sl-CU"<<endl;
    int clock_ = clock();
    unsigned char key[TRACE_L];
    double mopss = 0,cnt=0;
    string tm ;
#if !defined(tpTest) || tpTest==1
    tm = string(argv[2]) + "/speed_SI_CM.csv";
#endif
#if !defined(tpTest) || tpTest==2
    tm = string(argv[2]) + "/speed_SI_CU.csv";
#endif
    ofstream fout(tm.c_str(),ios::app);
    while(fread(key, TRACE_L, 1, file) > 0)
    {
        if(string(argv[2])!="imc")
            memmove(packet.str, key, TUPLE_L);
        else
            memmove(packet.str, key+8, TUPLE_L);
            
        packet.str[TUPLE_L] = '\0';

        if(num > input_num_max){
            break;
        }

#ifndef tpTest

        unsigned int pos = num % cycle;
        if(num >= cycle){
            mp[dat[pos]] -= 1;
        }


        dat[pos] = packet;
#endif
        string tm ;
        ofstream fout;
#if !defined(tpTest) || tpTest==1
        CM_Counter.CM_Init(packet.str, TUPLE_L, num);
#endif
#if !defined(tpTest) || tpTest==2
        CU_Counter.CU_Init(packet.str, TUPLE_L, num);
#endif
#if !defined(tpTest) || tpTest==3
        CO_Counter.CO_Init(packet.str, TUPLE_L, num);
#endif

#ifndef tpTest

        if(mp.find(packet) == mp.end())
            mp[packet] = 1;
        else
            mp[packet] += 1;

        if( num > cycle){

            if(num%(cycle/100) ==0){
                st.clear();


                clock_t stt = clock();
                for(int i=0;i<cycle;++i){
                    if(st.find(dat[i]) != st.end()) continue;
                    st.insert(dat[i]);
                    Test_num ++;
                    // CM_dif = fabs((double)CM_Counter.Query(dat[i].str,TUPLE_L) - mp[dat[i]]);
                    // CU_dif = fabs((double)CU_Counter.Query(dat[i].str,TUPLE_L) - mp[dat[i]]);
                    // CO_dif = fabs((double)CO_Counter.CO_Query(dat[i].str,TUPLE_L) - mp[dat[i]]);
                    // CM_ae += CM_dif; CU_ae += CU_dif; CO_ae += CO_dif;
                    // CM_re += CM_dif/mp[dat[i]]; CU_re += CU_dif/mp[dat[i]]; CO_re += CO_dif/mp[dat[i]];

                    // cerr << CM_dif << " "  << mp[dat[i]] <<endl;
                }
                clock_t ed = clock();
			    mopss += 1. / (1. * (ed - stt) / CLOCKS_PER_SEC / (cycle)) / 1e6;
                cnt+=1;
                // double tp = (double)num /1000000 / ((double)(clock()-clock_)/CLOCKS_PER_SEC);

                /*
                cout << "Sl-CM are" << "," << num << "," << CM_re / Test_num << endl;
                cout << "Sl-CU are" << "," << num << "," << CU_re / Test_num << endl;
                cout << "Sl-Count are" << "," << num << "," << CO_re / Test_num << endl;
                cout << "Sl-CM aae" << "," << num << "," << CM_ae / Test_num << endl;
                cout << "Sl-CU aae" << "," << num << "," << CU_ae / Test_num << endl;
                cout << "Sl-Count aae" << "," << num << "," << CO_ae / Test_num << endl;
                */
                CM_AEE += CM_ae / Test_num;
                CU_AEE += CU_ae / Test_num;
                totcnt++;
            }
        }

#endif

        num++;

        
    }
    
        
#ifndef tpTest
#else
    double mops = 1. / (1. * (clock()-clock_) / CLOCKS_PER_SEC / num) / 1e6;
    cout << mops << endl;
    fout << int(mymemory*1000)<<","<<mops<<"," << endl;
    // cout << ((double)(clock()-clock_)/CLOCKS_PER_SEC) / num << endl;

#endif
    fout.close();
}

int main(int argc, char* argv[]){
    Read_File(argc, argv);
    // printf("---\n");
    // Read_File(20);
    // printf("---\n");
    // Read_File(25);
    // printf("---\n");
    // Read_File(30);
    // printf("---\n");
    // Read_File(35);
    // printf("---\n");
    // Read_File(40);
}
