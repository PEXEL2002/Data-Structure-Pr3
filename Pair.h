#pragma once
#include <climits>

template <typename K, typename V>
struct Pair{
    K _key;
    V _value;
    Pair(int key, int value){
        _key = key;
        _value = value;
    }
    Pair(){
        _key = INT_MIN;
        _value = 0;
    }
    ~Pair(){}
};
