#ifndef DATA_H
#define DATA_H

#include "definition.h"
#include "hash_class.h"

class Data{
public:
    unsigned char str[TUPLE_LEN];
    Data& operator = (Data an);
};

bool operator < (Data bn, Data an);
bool operator == (Data bn, Data an);

class My_Hash{
public:
    size_t operator()(const Data dat) const{
        return RSHash(dat.str, TUPLE_LEN);
    }
};

#endif // DATA_H
