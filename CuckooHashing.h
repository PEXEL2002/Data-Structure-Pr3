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
    int new_size = _size * 2;
    Pair<K, V> * old_table1 = _table1;
    Pair<K, V> * old_table2 = _table2;
    _table1 = new Pair<K, V>[new_size/2];
    _table2 = new Pair<K, V>[new_size/2];
    for(int i = 0; i < _size/2; i++){
        if(old_table1[i]._key != emptyKey){
            insert(old_table1[i]._key, old_table1[i]._value);
        }
        if(old_table2[i]._key != emptyKey){
            insert(old_table2[i]._key, old_table2[i]._value);
        }
    }
    delete[] old_table1;
    delete[] old_table2;
    _size = new_size;
}

template <typename K, typename V>
void Cuckoo<K, V>::resizeDown(){
    int new_size = _size / 2;
    Pair<K, V> * old_table1 = _table1;
    Pair<K, V> * old_table2 = _table2;
    _table1 = new Pair<K, V>[new_size/2];
    _table2 = new Pair<K, V>[new_size/2];
    for(int i = 0; i < _size/2; i++){
        if(old_table1[i]._key != emptyKey){
            insert(old_table1[i]._key, old_table1[i]._value);
        }
        if(old_table2[i]._key != emptyKey){
            insert(old_table2[i]._key, old_table2[i]._value);
        }
    }
    delete[] old_table1;
    delete[] old_table2;
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
    int optimal_size = size/0.4;
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
    if (_capacity >= 0.5 * _size) {
        resizeUp();
    }
    Pair<K, V> entry(key, value);
    int pos1, pos2, limit = 64;

    for (int i = 0; i < limit; ++i) {
        pos1 = hash1(entry._key);
        std::swap(entry, _table1[pos1]);
        if (entry._key == emptyKey) return;

        pos2 = hash2(entry._key);
        std::swap(entry, _table2[pos2]);
        if (entry._key == emptyKey) return; 
    }
    // if limit is reached, resize the table
    resizeUp();
    insert(entry._key, entry._value);
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