#pragma once
#include "HashTable.h"
#include  <vector>
#include <string>
#include <climits>
/**
 * @brief OpenAddressing class
 * @param _table: vector of pairs
 * @param _capacity: capacity of the table
 * @param _size: max size of the table
 * @param _type: type of hash functionc (0: Linear Probing, 1: Quadratic Probing, 2: Double Hashing)
 * @param _alfa: load factor
 * @param emptyKey: empty key
*/
template <typename K, typename V>
class OpenAddressing : public HashTable<K, V>{
    private:
        std::vector<std::pair<K, V>> _table;
        int _capacity;
        int _size = 10;
        int _type;
        float _alfa = _capacity/_size;
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
 * @param capacity: capacity of the table
*/
        OpenAddressing(int capacity);
/**
 * @brief Constructors
 * @param capacity: capacity of the table
 * @param type: type of hash function
*/
        OpenAddressing(int capacity, int type);
/**
 * @brief Constructors
 * @param filename: name of the file
*/
        OpenAddressing(std::string filename);
/**
 * @brief Constructors
 * @param filename: name of the file
 * @param type: type of hash function
*/
        OpenAddressing(std::string filename, int type);
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
        ~OpenAddressing(){}
};

template <typename K, typename V>
int OpenAddressing<K, V>::hash(K key, int i){
    int h1 = abs(key % _size);
    int h2 = abs(1 + (key % (_size - 1)));

    switch (_type){
    case 0:
        return (h1 + i) % _size; // Linear Probing
    case 1:
        return (h1 + i*i*3 + i) % _size; // Quadratic Probing
    case 2:
        return (h1 + i*h2) % _size; // Double Hashing
    default:
        return (h1 + i) % _size; // Linear Probing
    }
    
}

template <typename K, typename V>
void OpenAddressing<K, V>::resizeUp(){
    if(_alfa >= 0.8){
        std::vector<std::pair<K, V>> newTable(_size*2, {emptyKey,0});
        for(int i = 0; i < _size; i++){
            if(_table[i].first != emptyKey){
                int index = hash(_table[i].first, 0);
                for(int j = 0; j < _size; j++){
                    if(newTable[index].first == emptyKey){
                        newTable[index].first = _table[i].first;
                        newTable[index].second = _table[i].second;
                        break;
                    }
                    index = hash(_table[i].first, j);
                }
            }
        }
        _table = newTable;
        _size = _size*2;
    }
}

template <typename K, typename V>
void OpenAddressing<K, V>::resizeDown(){
    if(_alfa <= 0.2){
        std::vector<std::pair<K, V>> newTable(_size/2, {emptyKey,0});
        for(int i = 0; i < _size; i++){
            if(_table[i].first != emptyKey){
                int index = hash(_table[i].first, 0);
                for(int j = 0; j < _size; j++){
                    if(newTable[index].first == emptyKey){
                        newTable[index].first = _table[i].first;
                        newTable[index].second = _table[i].second;
                        break;
                    }
                    index = hash(_table[i].first, j);
                }
            }
        }
        _table = newTable;
        _capacity = _capacity/2;
    }
}

template <typename K, typename V>
OpenAddressing<K, V>::OpenAddressing(){
    _capacity = 0;
    _size = 10;
    _type = 0;
    _table.resize(_size);
    for(int i = 0; i < _size; i++){
        _table[i].first = emptyKey;
    }
}

template <typename K, typename V>
OpenAddressing<K, V>::OpenAddressing(int size){
    _capacity = 0;
    _size = size;
    _type = 0;
    _table.resize(_size);
    for(int i = 0; i < _size; i++){
        _table[i].first = emptyKey;
    }
}

template <typename K, typename V>
OpenAddressing<K, V>::OpenAddressing(int size, int type){
    _capacity = 0;
    _size = size;
    _type = type;
    _table.resize(_size);
    for(int i = 0; i < _size; i++){
        _table[i].first = emptyKey;
    }
}

template <typename K, typename V>
OpenAddressing<K, V>::OpenAddressing(std::string filename){
    /**
     * @todo Implement this function
    */
}

template <typename K, typename V>
OpenAddressing<K, V>::OpenAddressing(std::string filename, int type){
    /**
     * @todo Implement this function
    */
}

template <typename K, typename V>
void OpenAddressing<K, V>::insert(K key, V value){
    int index = hash(key, 0);
    for(int i = 0; i < _size; i++){
        if(_table[index].first == emptyKey){
            _table[index].first = key;
            _table[index].second = value;
            _capacity++;
            _alfa = _capacity/_size;
            resizeUp();
            return;
        }
        index = hash(key, i);
    }
    
}

template <typename K, typename V>   
void OpenAddressing<K, V>::remove(K key){
    if(!exists(key) || empty()){
        return;
    }
    int index = hash(key, 0);
    for(int i = 0; i < _size; i++){
        if(_table[index].first == key){
            _table[index].first = emptyKey;
            _capacity--;
            _alfa = _capacity/_size;
            resizeDown();
            return;
        }
        index = hash(key, i);
    }
}

template <typename K, typename V>
V OpenAddressing<K, V>::find(K key){
    int index = hash(key, 0);
    for(int i = 0; i < _size; i++){
        if(_table[index].first == key){
            return _table[index].second;
        }
        index = hash(key, i);
    }
    return -1;
}

template <typename K, typename V>
bool OpenAddressing<K, V>::exists(K key){
    int index = hash(key, 0);
    for(int i = 0; i < _size; i++){
        if(_table[index].first == key){
            return true;
        }
        index = hash(key, i);
    }
    return false;
}

template <typename K, typename V>
int OpenAddressing<K, V>::size(){
    return _capacity;
}

template <typename K, typename V>
bool OpenAddressing<K, V>::empty(){
    return _capacity == 0;
}

template <typename K, typename V>
void OpenAddressing<K, V>::keys(){
    for(auto& elem : _table){
        if(elem.first != emptyKey){
            std::cout << elem.first << std::endl;
        }
    }
}

template <typename K, typename V>
void OpenAddressing<K, V>::values(){
    for(auto& elem : _table){
        if(elem.first != emptyKey){
            std::cout << elem.first << std::endl;
        }
    }
}

template <typename K, typename V>
void OpenAddressing<K, V>::print(){
    for(auto& elem : _table){
        if(elem.first != emptyKey){
            std::cout << elem.first << " " << elem.second << std::endl;
        }
    }
}