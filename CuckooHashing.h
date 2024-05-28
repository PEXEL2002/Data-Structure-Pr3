#pragma once
#include "HashTable.h"
#include "Pair.h"
#include <iostream>
#include <string>
#include <climits>


/**
 * @brief Cuckoo class
 * @param _table1: array of pairs
 * @param _table2: array of pairs
 * @param _capacity: capacity of the table
 * @param _size: max size of the table
 * @param _alfa: load factor
 * @param emptyKey: empty key
*/

template <typename K, typename V>
class Cuckoo : public HashTable<K, V>{
    private:
        Pair<K, V> * _table1, * _table2;
        int _capacity;
        int _size = 10;
        float _alfa;
        K emptyKey = INT_MIN;
/**
 * @brief Hash function
 * @param key: key to hash
*/
        int hash1(K key);
        int hash2(K key);
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
        Cuckoo();
/**
 * @brief Constructors
 * @param size: size of the table
*/
        Cuckoo(int size);
/**
 * @brief Constructors
 * @param filename: name of the file
 * @param size: size of the table
*/
        Cuckoo(std::string filename, int size);
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
        ~Cuckoo(){
            delete[] _table1;
            delete[] _table2;
        };
};

template <typename K, typename V>
int Cuckoo<K, V>::hash1(K key){
    return key%(_size/2);
}

template <typename K, typename V>
int Cuckoo<K, V>::hash2(K key){
    return (key * 31 + 1) % (_size / 2);
}

template <typename K, typename V>
void Cuckoo<K, V>::resizeUp(){
    std::cout << "Resize table";
    int new_size = _size * 2;
    Pair<K, V> * new_table1 = new Pair<K, V>[new_size/2];
    Pair<K, V> * new_table2 = new Pair<K, V>[new_size/2];
    int index;
    for(int i = 0; i < _size/2; i++){
        if(_table1[i]._key != emptyKey){
            index = hash1(_table1[i]._key);
            new_table1[index] = _table1[i];
        }
        if(_table2[i]._key != emptyKey){
            index = hash2(_table2[i]._key);
            new_table2[index] = _table2[i];
        }
    }
    delete[] _table1;
    delete[] _table2;
    _table1 = new_table1;
    _table2 = new_table2;
    _size = new_size;
}

template <typename K, typename V>
void Cuckoo<K, V>::resizeDown(){
    int new_size = _size / 2;
    Pair<K, V> * new_table1 = new Pair<K, V>[new_size/2];
    Pair<K, V> * new_table2 = new Pair<K, V>[new_size/2];
    int index;
    for(int i = 0; i < _size/2; i++){
        if(_table1[i]._key != emptyKey){
            index = hash1(_table1[i]._key);
            new_table1[index] = _table1[i];
        }
        if(_table2[i]._key != emptyKey){
            index = hash2(_table2[i]._key);
            new_table2[index] = _table2[i];
        }
    }
    delete[] _table1;
    delete[] _table2;
    _table1 = new_table1;
    _table2 = new_table2;
    _size = new_size;
}

template <typename K, typename V>
Cuckoo<K, V>::Cuckoo(){
    _size = 10;
    _table1 = new Pair<K, V>[_size/2];
    _table2 = new Pair<K, V>[_size/2];
    _alfa = 0.0;
}

template <typename K, typename V>
Cuckoo<K, V>::Cuckoo(int size){
    if(size%2 == 1) size++;
    _size = size;
    _table1 = new Pair<K, V>[_size/2];
    _table2 = new Pair<K, V>[_size/2];
    _alfa = 0.0;
}

template <typename K, typename V>
Cuckoo<K, V>::Cuckoo(std::string filename, int size){
    int optimal_size = size/0.45;
    if(optimal_size%2 == 1) optimal_size++;
    _size = optimal_size;
    _table1 = new Pair<K, V>[_size/2];
    _table2 = new Pair<K, V>[_size/2];
    _alfa = 0.0;
    std::ifstream file;
    file.open(filename);
    K key;
    V value;
    while(file >> key >> value){
        insert(key, value);
    }
    file.close();
}

template <typename K, typename V>
void Cuckoo<K, V>::insert(K key, V value) {
    int limit = 64; // limit for the number of attempts to insert a key
    int index1, index2;
    for (int attempt = 0; attempt < limit; ++attempt) {
        int index1 = hash1(key);
        if (_table1[index1]._key == INT_MIN) {
            _table1[index1] = Pair<K, V>(key, value);
            _capacity++;
            _alfa = static_cast<float>(_capacity) / static_cast<float>(_size);
            if(_alfa > 0.75) resizeUp();
            return;
        }
        std::swap(_table1[index1]._key, key);
        std::swap(_table1[index1]._value, value);
        int index2 = hash2(key);
        if (_table2[index2]._key == INT_MIN) {
            _table2[index2] = Pair<K, V>(key, value);
            _capacity++;
            _alfa = static_cast<float>(_capacity) / static_cast<float>(_size);
            if(_alfa > 0.75) resizeUp();
            return;
        }
        std::swap(_table2[index2]._key, key);
        std::swap(_table2[index2]._value, value);
    }
    // if we are here, then we have a cycle
    resizeUp();
    insert(key, value); // try again to insert the key
}

template <typename K, typename V>
void Cuckoo<K, V>::remove(K key){
    int index1 = hash1(key);
    int index2 = hash2(key);
    if(_table1[index1]._key == key){
        _table1[index1] = Pair<K, V>();
        _capacity--;
    }else{
        if(_table2[index2]._key == key){
            _table2[index2] = Pair<K, V>();
            _capacity--;
        }
    }
    _alfa = static_cast<float>(_capacity) / static_cast<float>(_size);
    if(_alfa <= 0.20) resizeDown();
}

template <typename K, typename V>
V Cuckoo<K, V>::find(K key){
    int index1 = hash1(key);
    int index2 = hash2(key);
    if(_table1[index1]._key == key){
        return _table1[index1]._value;
    }else{
        if(_table2[index2]._key == key){
            return _table2[index2]._value;
        }
    }
    return INT_MIN;
}

template <typename K, typename V>
bool Cuckoo<K, V>::exists(K key){
    int index1 = hash1(key);
    int index2 = hash2(key);
    if(_table1[index1]._key == key || _table2[index2]._key == key){
        return true;
    }
    return false;
}

template <typename K, typename V>
int Cuckoo<K, V>::size(){
    return _capacity;
}

template <typename K, typename V>
bool Cuckoo<K, V>::empty(){
    return _capacity == 0;
}

template <typename K, typename V>
void Cuckoo<K, V>::keys(){
    for(int i = 0; i < _size/2; i++){
        if(_table1[i]._key != emptyKey){
            std::cout << _table1[i]._key << " ";
        }
        if(_table2[i]._key != emptyKey){
            std::cout << _table2[i]._key << " ";
        }
    }
    std::cout << std::endl;
}

template <typename K, typename V>
void Cuckoo<K, V>::values(){
    for(int i = 0; i < _size/2; i++){
        if(_table1[i]._key != emptyKey){
            std::cout << _table1[i]._value << " ";
        }
        if(_table2[i]._key != emptyKey){
            std::cout << _table2[i]._value << " ";
        }
    }
    std::cout << std::endl;
}

template <typename K, typename V>
void Cuckoo<K, V>::print(){
    for(int i = 0; i < _size/2; i++){
        if(_table1[i]._key != emptyKey){
            std::cout << _table1[i]._key << "->" << _table1[i]._value << std::endl;
        }
        if(_table2[i]._key != emptyKey){
            std::cout << _table2[i]._key << "->" << _table2[i]._value << std::endl;
        }
    }
}