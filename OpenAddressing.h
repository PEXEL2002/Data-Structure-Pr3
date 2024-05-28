#pragma once
#include "HashTable.h"
#include "Pair.h"
#include <iostream>
#include <string>
#include <climits>
/**
 * @brief SeparateChaining class
 * @param _table: array of buckets of pairs
 * @param _bucketSize: size of the bucket
 * @param _capacity: capacity of the table
 * @param _size: max size of the table
 * @param _alfa: load factor
*/
template <typename K, typename V>
class SeparateChaining : public HashTable<K, V>{
    private:
        Pair<K, V> ** _table;
        int _bucketSize = 50;
        int _capacity;
        int _size;
        int _sizetemp;
        float _alfa;
/**
 * @brief Hash function
 * @param key: key to hash
 * @param i: index of the key
*/
        int hash(K key);
/**
 * @brief Resize functions up
*/
        void resizeUp();
/**
 * @brief Resize functions down
*/
        void resizeDown();
/**
 * @brief Change the bucket so there are no interruptions
 * @param index: index of the bucket
*/
        void refactorBucket(int index);
    public:
/**
 * @brief Constructors
*/
        SeparateChaining();
/**
 * @brief Constructors
 * @param capacity: capacity of the table
*/
        SeparateChaining(int size);
/**
 * @brief Constructors
 * @param filename: name of the file
 * @param size: size of the table
*/
        SeparateChaining(std::string filename, int size);
/**
 * @brief Insert function
 * @param key: key to insert
 * @param value: value to insert
*/
        void insert(K key, V value) override;
/**
 * @brief Remove function
 * @param key: key to remove
*/
        void remove(K key) override;
/**
 * @brief Find function
 * @param key: key to find
 * @return value of the key
*/
        V find(K key) override;
/**
 * @brief Exists function
 * @param key: key to find
 * @return true if the key exists, false otherwise
*/
        bool exists(K key) override;
/**
 * @brief Size function
 * @return capacity of the table
*/
        int size() override;
/**
 * @brief Empty function
 * @return true if the table is empty, false otherwise
*/
        bool empty() override;
/**
 * @brief print keys function
*/
        void keys() override;
/**
 * @brief print values function
*/
        void values() override;
/**
 * @brief print function
*/
        void print() override;
/**
 * @brief Destructor
*/
        ~SeparateChaining(){
            for(int i = 0; i < _size; i++){
                delete[] _table[i];
            }
            delete[] _table;
        }
};
template <typename K, typename V>
int SeparateChaining<K, V>::hash(K key){
    return abs(key % _size); // Modulo Hashing
}

template <typename K, typename V>
void SeparateChaining<K, V>::resizeUp(){
    int new_size = _size * 2;
    Pair<K, V> **new_table = new Pair<K, V>*[new_size];
    for (int i = 0; i < new_size; i++) {
        new_table[i] = new Pair<K, V>[_bucketSize];
    }
    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _bucketSize; j++) {
            if (_table[i][j]._key != INT_MIN) {
                int new_index = hash(_table[i][j]._key);
                int k = 0;
                while (new_table[new_index][k]._key != INT_MIN && k < _bucketSize) {
                    k++;
                    if (k == _bucketSize) break;
                }
                if (k < _bucketSize) {
                    new_table[new_index][k] = _table[i][j];
                }
            }
        }
    }
    for (int i = 0; i < _size; i++) {
        delete[] _table[i];
    }
    delete[] _table;
    _table = new_table;
    _size = new_size;
}

template <typename K, typename V>
void SeparateChaining<K, V>::resizeDown() {
    int new_size = _size / 2;
    Pair<K, V> ** new_table = new Pair<K, V>*[new_size];
    for (int i = 0; i < new_size; i++) {
        new_table[i] = new Pair<K, V>[_bucketSize];
    }
    for (int i = 0; i < _size; i++) {
        for (int j = 0; j < _bucketSize; j++) {
            if (_table[i][j]._key != INT_MIN) {
                int new_index = hash(_table[i][j]._key) % new_size;
                int k = 0;
                while (new_table[new_index][k]._key != INT_MIN && k < _bucketSize) {
                    k++;
                    if (k == _bucketSize) break;
                }
                if (k < _bucketSize) {
                    new_table[new_index][k] = _table[i][j];
                }
            }
        }
    }
    for (int i = 0; i < _size; i++) {
        delete[] _table[i];
    }
    delete[] _table;
    _table = new_table;
    _size = new_size;
}


template <typename K, typename V>
void SeparateChaining<K, V>::refactorBucket(int index){
    Pair<K, V> * temp = new Pair<K, V>[_bucketSize];
    int j=0;
    for(int i = 0; i < _bucketSize; i++){
        if(_table[index][i]._key != INT_MIN){
            temp[j] = _table[index][i];
            j++;
        }
        if(_table[index][i]._key == INT_MIN && _table[index][i+1]._key == INT_MIN){
            break;
        }
    }
    delete[] _table[index];
    _table[index] = temp;

}

template <typename K, typename V>
SeparateChaining<K, V>::SeparateChaining(){
    _size = 10;
    _table = new Pair<K, V>*[_size];
    for(int i = 0; i < _size; i++){
        _table[i] = new Pair<K, V>[_bucketSize];
    }
    _capacity = 0;
}

template <typename K, typename V>
SeparateChaining<K, V>::SeparateChaining(int size){
    _size = size;
    _table = new Pair<K, V>*[_size];
    for(int i = 0; i < _size; i++){
        _table[i] = new Pair<K, V>[_bucketSize];
    }
    _capacity = 0;

}

template <typename K, typename V>
SeparateChaining<K, V>::SeparateChaining(std::string filename, int size){
    _size = size;
    _table = new Pair<K, V>*[_size];
    for(int i = 0; i < _size; i++){
        _table[i] = new Pair<K, V>[_bucketSize];
    }
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
void SeparateChaining<K, V>::insert(K key, V value){
    int index = hash(key);
    int i = 0;
    while (i < _bucketSize && _table[index][i]._key != INT_MIN && _table[index][i]._key != key) {
        i++;
    }
    if (i < _bucketSize) {
        if (_table[index][i]._key != key) {
            _table[index][i] = Pair<K, V>(key, value);
            _capacity++;
            _alfa = static_cast<float>(_capacity) / static_cast<float>(_size);
            if(_alfa >= 3){
                resizeUp();
            }
        }
    } else {
        resizeUp();
    }
}

template <typename K, typename V>
void SeparateChaining<K, V>::remove(K key) {
    int index = hash(key);
    bool found = false;
    int i;
    for (i = 0; i < _bucketSize; i++) {
        if (_table[index][i]._key == key) {
            found = true;
            break;
        }
        if (_table[index][i]._key == INT_MIN) {
            break;
        }
    }
    if (found) {
        _table[index][i] = Pair<K, V>();
        _capacity--;
        refactorBucket(index);
        if (_capacity > 0 && static_cast<float>(_capacity) / (_size) <= 1) {
            resizeDown();
        }
    }
}

template <typename K, typename V>
V SeparateChaining<K, V>::find(K key){
    int i = 0;
    while(_table[hash(key)][i]._key != key && i < _bucketSize){
        i++;
    }
    if(_table[hash(key)][i]._key == key){
        return _table[hash(key)][i]._value;
    }else{
        return INT_MIN;
    }
}

template <typename K, typename V>
bool SeparateChaining<K, V>::exists(K key){
    int i = 0;
    while(_table[hash(key)][i]._key != key && i < _bucketSize){
        i++;
    }
    if(_table[hash(key)][i]._key == key){
        return true;
    }else{
        return false;
    }
}

template <typename K, typename V>
int SeparateChaining<K, V>::size(){
    return _capacity;
}

template <typename K, typename V>
bool SeparateChaining<K, V>::empty(){
    return _capacity == 0;
}

template <typename K, typename V>
void SeparateChaining<K, V>::keys(){
    for(int i = 0; i < _size; i++){
        for(int j = 0; j < _bucketSize; j++){
            if(_table[i][j]._key != INT_MIN){
                std::cout << _table[i][j]._key << std::endl;
            }
        }
    }
    std::cout << std::endl;
}

template <typename K, typename V>
void SeparateChaining<K, V>::values(){
    for(int i = 0; i < _size; i++){
        for(int j = 0; j < _bucketSize; j++){
            if(_table[i][j]._key != INT_MIN){
                std::cout << _table[i][j]._value << std::endl;
            }
        }
    }
    std::cout << std::endl;
}

template <typename K, typename V>
void SeparateChaining<K, V>::print(){
    for(int i = 0; i < _size; i++){
        for(int j = 0; j < _bucketSize; j++){
            if(_table[i][j]._key != INT_MIN){
                std::cout << _table[i][j]._key << "->" << _table[i][j]._value << std::endl;
            }
        }
    }
    std::cout << std::endl;
}
