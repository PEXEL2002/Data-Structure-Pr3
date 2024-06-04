#pragma once
#include "Pair.h"
#include "Node.h"
template <typename K, typename V>
struct Node{
    Pair<K, V> _pair;
    Node* next;
    Node* prev;
    Node(){
        _pair = Pair<K, V>();
        next = nullptr;
        prev = nullptr;
    };
};