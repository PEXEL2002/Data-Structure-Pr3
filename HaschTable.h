#pragma once
template <typename K, typename V>
class HashTable{
    virtual void insert(K key, V value) = 0;
    virtual void remove(K key) = 0;
    virtual int findKey(K key) = 0;
    virtual void print() = 0;
    virtual ~HashTable() = default;
};
