#pragma once
#include "HashTable.h"
#include  <vector>
#include <string>
#include <climits>
/**
 * @brief SeparateChaining class
 * @param _table: vector of vectors of pairs
 * @param _capacity: capacity of the table
 * @param _size: max size of the table
 * @param _type: type of hash functionc (0: Modulo Hashing, 1: Hashing with the suffering of Prime Numbers, 2: Fibonacci Hashing)
 * @param _alfa: load factor
*/
template <typename K, typename V>
class SeparateChaining : public HashTable<K, V>{
    private:
        std::vector<std::vector<std::pair<K, V>>> _table;
        int _capacity;
        int _size;
        int _type;
        float _alfa = _capacity/_size;
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
 * @param capacity: capacity of the table
 * @param type: type of hash function
*/
        SeparateChaining(int size, int type);
/**
 * @brief Constructors
 * @param filename: name of the file
 * 
 * @param type: type of hash function
*/
        SeparateChaining(std::string filename);
/**
 * @brief Constructors
 * @param filename: name of the file
 * @param type: type of hash function
*/
        SeparateChaining(std::string filename, int type);
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
        ~SeparateChaining(){}
};
template <typename K, typename V>
int SeparateChaining<K, V>::hash(K key){
    double goldenRatio = 1.618033988749895; // Golden Ratio
    double fractionalPart = key * goldenRatio; // Get the fractional part of the number
    switch (_type){
    case 0:
        return abs(key % _size); // Modulo Hashing
    case 1:
        return abs((31*key) % _size); // Hashing with the suffering of Prime Numbers
    case 2: // Fibonacci Hashing
        fractionalPart -= static_cast<int>(fractionalPart);  // Get the fractional part of the number
        return abs(static_cast<unsigned int>(_size * fractionalPart)); // Multiply the fractional part with the table size
    default:
        return abs(key % _size); // Modulo Hashing
    }
}

template <typename K, typename V>
void SeparateChaining<K, V>::resizeUp(){
    if(_alfa >= 0.8){
        _size = _size * 2;
        std::vector<std::vector<std::pair<K, V>>> temp(_size);
        for(int i = 0; i < _size; i++){
            if(!_table[i].empty()){
                for(int j = 0; j < _table[i].size(); j++){
                    int index = hash(_table[i][j].first);
                    temp[index].push_back(_table[i][j]);
                }
            }
        }
        _table = temp;
    }
}

template <typename K, typename V>
void SeparateChaining<K, V>::resizeDown(){
    if(_alfa <= 0.2){
        _size = _size / 2;
        std::vector<std::vector<std::pair<K, V>>> temp(_size);
        for(int i = 0; i < _size; i++){
            if(!_table[i].empty()){
                for(int j = 0; j < _table[i].size(); j++){
                    int index = hash(_table[i][j].first);
                    temp[index].push_back(_table[i][j]);
                }
            }
        }
        _table = temp;
    }
}

template <typename K, typename V>
SeparateChaining<K, V>::SeparateChaining(){
    _capacity = 0;
    _size = 10;
    _type = 0;
    _table.resize(_size);
}

template <typename K, typename V>
SeparateChaining<K, V>::SeparateChaining(int size){
    _capacity = 0;
    _size = size;
    _type = 0;
    _table.resize(_size);
}

template <typename K, typename V>
SeparateChaining<K, V>::SeparateChaining(int size, int type){
    _capacity = 0;
    _size = size;
    _type = type;
    _table.resize(_size);
}

template <typename K, typename V>
SeparateChaining<K, V>::SeparateChaining(std::string filename){
    /**
     * @todo Implement this function
    */
}

template <typename K, typename V>
SeparateChaining<K, V>::SeparateChaining(std::string filename, int type){
    /**
     * @todo Implement this function
    */
}

template <typename K, typename V>
void SeparateChaining<K, V>::insert(K key, V value){
    int index = hash(key);
    _table[index].push_back(std::make_pair(key, value));
    _capacity++;
    _alfa = _capacity/_size;
    resizeUp();
    
}

template <typename K, typename V>
void SeparateChaining<K, V>::remove(K key){
    if(!exists(key) || empty()){
        return;
    }
    int index = hash(key);
    for(int i = 0; i < _table[index].size(); i++){
        if(_table[index][i].first == key){
            _table[index].erase(_table[index].begin() + i);
            _capacity--;
            _alfa = _capacity/_size;
            resizeDown();
            return;
        }
    }
}

template <typename K, typename V>
V SeparateChaining<K, V>::find(K key){
    int index = hash(key);
    for(int i = 0; i < _table[index].size(); i++){
        if(_table[index][i].first == key){
            return _table[index][i].second;
        }
    }
    return -1;
}

template <typename K, typename V>
bool SeparateChaining<K, V>::exists(K key){
    int index = hash(key);
    for(int i = 0; i < _table[index].size(); i++){
        if(_table[index][i].first == key){
            return true;
        }
    }
    return false;
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
        if(!_table[i].empty()){
            for(int j = 0; j < _table[i].size(); j++){
                std::cout << _table[i][j].first << std::endl;
            }
        }
    }   
}

template <typename K, typename V>
void SeparateChaining<K, V>::values(){
    for(int i = 0; i < _size; i++){
        if(!_table[i].empty()){
            for(int j = 0; j < _table[i].size(); j++){
                std::cout << _table[i][j].second << std::endl;
            }
        }
    }   
}

template <typename K, typename V>
void SeparateChaining<K, V>::print(){
    for(auto &bucket : _table){
        if(!bucket.empty()){
            for (auto& item :bucket) {
                std::cout << item.first << " " << item.second << std::endl;
            }
        }
    }    
}
