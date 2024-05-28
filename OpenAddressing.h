#pragma once
#include "HashTable.h"
#include "Pair.h"
#include <string>
#include <climits>
#include <iostream>
#include <fstream>
/**
 * @brief OpenAddressing class
 * @param _table: array of pairs
 * @param _capacity: capacity of the table
 * @param _size: max size of the table
 * @param _alfa: load factor
 * @param emptyKey: empty key
*/
template <typename K, typename V>
class OpenAddressing : public HashTable<K, V>{
    private:
        Pair<K, V> * _table;
        int _capacity;
        int _size = 10;
        float _alfa;
        K emptyKey = INT_MIN;
/**
 * @brief Hash function
 * @param key: key to hash
 * @param i: index of the key
*/
        int hash(K key, int i);
/**
 * @brief Resize functions up
*/
        void resizeUp();
/**
 * @brief Resize functions down
*/
        void resizeDown();
    public:
/**
 * @brief Constructors
*/
        OpenAddressing();
/**
 * @brief Constructors
 * @param size: size of the table
*/
        OpenAddressing(int size);
/**
 * @brief Constructors
 * @param filename: name of the file
 * @param size: size of the table
*/
        OpenAddressing(std::string filename, int size);
/**
 * @brief Insert function
 * @param key: key to insert
 * @param value: value to insert
*/
        void insert(K key, V value);
/**
 * @brief Remove function
 * @param key: key to remove
*/
        void remove(K key);
/**
 * @brief Find function
 * @param key: key to find
 * @return value of the key
*/
        V find(K key);
/**
 * @brief Exists function
 * @param key: key to find
 * @return true if the key exists, false otherwise
*/
        bool exists(K key);
/**
 * @brief Size function
 * @return capacity of the table
*/
        int size();
/**
 * @brief Empty function
 * @return true if the table is empty, false otherwise
*/
        bool empty();
/**
 * @brief print keys function
*/
        void keys();
/**
 * @brief print values function
*/
        void values();
/**
 * @brief print function
*/
        void print();
/**
 * @brief Destructor
*/
        ~OpenAddressing(){
            delete[] _table;
        }
};

template <typename K, typename V>
int OpenAddressing<K, V>::hash(K key, int i){
    int span = 3; // pause between the following keys
    int h1 = abs(key*span % _size);
    return (h1 + i) % _size; // Linear Probing
}

template <typename K, typename V>
void OpenAddressing<K, V>::resizeUp(){
    _alfa = static_cast<float>(_capacity) / static_cast<float>(_size);
    if(_alfa > 0.80){
        Pair<K, V> * temp = new Pair<K, V>[_size*2];
        _size = _size*2;
        for(int i = 0; i < _size/2; i++){
            if(_table[i]._key != emptyKey){
                int j = 0;
                while(temp[hash(_table[i]._key, j)]._key != emptyKey){
                    j++;
                }
                temp[hash(_table[i]._key, j)] = _table[i];
            }
        }
        delete[] _table;
        _table = temp;
    }
}

template <typename K, typename V>
void OpenAddressing<K, V>::resizeDown(){
    _alfa = static_cast<float>(_capacity) / static_cast<float>(_size);
    if(_alfa <=0.20){
        _size = _size/2;
        Pair<K, V> * temp = new Pair<K, V>[_size/2];
        for(int i = 0; i < _size*2; i++){
            if(_table[i]._key != emptyKey){
                int j = 0;
                while(temp[hash(_table[i]._key, j)]._key != emptyKey){
                    j++;
                }
                temp[hash(_table[i]._key, j)] = _table[i];
            }
        }
        delete[] _table;
        _table = temp;
    }
}

template <typename K, typename V>
OpenAddressing<K, V>::OpenAddressing(){
    _capacity = 0;
    _size = 10;
    _table = new Pair<K, V>[_size];
}

template <typename K, typename V>
OpenAddressing<K, V>::OpenAddressing(int size){
    _capacity = 0;
    _size = size;
    _table = new Pair<K, V>[_size];
}

template <typename K, typename V>
OpenAddressing<K,V>::OpenAddressing(std::string filename, int size)  {
    _size = size*2;
    _table = new Pair<K, V>[_size*2];
    _capacity = 0;
    std::ifstream file(filename);
    if (file.is_open()) {
        K key;
        V value;
        while (file >> key >> value) {
            insert(key, value);
        }
        file.close();
    } else {
        std::cout << "File not found" << std::endl;
    }
}

template <typename K, typename V>
void OpenAddressing<K, V>::insert(K key, V value){
    int i = 0;
    while(_table[hash(key, i)]._key != emptyKey && _table[hash(key, i)]._key != key){
        i++;
    }
    if(_table[hash(key, i)]._key != key){
        _table[hash(key, i)] = Pair<K, V>(key, value);
        _capacity++;
        resizeUp();
    }
}

template <typename K, typename V>
void OpenAddressing<K, V>::remove(K key){
    int i = 0;
    while(_table[hash(key, i)]._key != key && i < _size-hash(key, 0)){
        i++;
    }
    _table[hash(key, i)] = Pair<K, V>();
    _capacity--;
    resizeDown();
}

template <typename K, typename V>
V OpenAddressing<K, V>::find(K key){
    int i = 0;
    while(_table[hash(key, i)]._key != key && i < _size-hash(key, 0)){
        i++;
    }
    if(_table[hash(key, i)]._key == key){
        return _table[hash(key, i)]._value;
    }else{
        return INT_MIN;
    }
}

template <typename K, typename V>
bool OpenAddressing<K, V>::exists(K key){
    int i = 0;
    while(_table[hash(key, i)]._key != key && i < _size){
        i++;
    }
    if(_table[hash(key, i)]._key == key){
        return true;
    }
    return false;
}

template <typename K, typename V>
int OpenAddressing<K, V>::size(){
    return _size;
}

template <typename K, typename V>
bool OpenAddressing<K, V>::empty(){
    if(_capacity == 0){
        return true;
    }
    return false;
}

template <typename K, typename V>
void OpenAddressing<K, V>::keys(){
    for(int i = 0; i < _size; i++){
        if(_table[i]._key != emptyKey){
            std::cout << _table[i]._key << std::endl;
        }
    }
}

template <typename K, typename V>
void OpenAddressing<K, V>::values(){
    for(int i = 0; i < _size; i++){
        if(_table[i]._key != emptyKey){
            std::cout << _table[i]._value << std::endl;
        }
    }
}

template <typename K, typename V>
void OpenAddressing<K, V>::print(){
    for(int i = 0; i < _size; i++){
        if(_table[i]._key != emptyKey){
            std::cout << _table[i]._key << "->" << _table[i]._value << std::endl;
        }
    }
}