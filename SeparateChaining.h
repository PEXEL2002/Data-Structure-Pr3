#pragma once
#include "HashTable.h"
#include "Pair.h"
#include <string>
#include <climits>
/**
 * @brief SeparateChaining class
 * @param _table: array of buckets of pairs
 * @param _bucketSize: size of the bucket
 * @param _capacity: capacity of the table
 * @param _size: max size of the table
 * @param _type: type of hash functionc (0: Modulo Hashing, 1: Hashing with the suffering of Prime Numbers, 2: Fibonacci Hashing)
 * @param _alfa: load factor
*/
template <typename K, typename V>
class SeparateChaining : public HashTable<K, V>{
    private:
        Pair<K, V> ** _table;
        int _bucketSize = 50;
        int _capacity;
        int _size;
        int _type;
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
 * @brief Constructors
 * @param filename: name of the file
 * @param type: type of hash function
 * @param size: size of the table
*/
        SeparateChaining(std::string filename, int type, int size);
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
    _alfa = static_cast<float>(_capacity) / static_cast<float>(_size);
    if(_alfa >= 0.8){
        _size = _size * 2;
        Pair<K, V> ** tempTable = new Pair<K, V>*[_size];
        for(int i = 0; i < _size; i++){
            tempTable[i] = new Pair<K, V>[_bucketSize];
        }
        for(int i = 0; i < _size/2; i++){
            for(int j = 0; j < _bucketSize; j++){
                tempTable[hash(_table[i][j]._key)][j] = _table[i][j];
            }
        }
        for(int i = 0; i < _size/2; i++){
            delete[] _table[i];
        }
        delete[] _table;
        _table = tempTable;
    }
}

template <typename K, typename V>
void SeparateChaining<K, V>::resizeDown(){
    _alfa = static_cast<float>(_capacity) / static_cast<float>(_size);
    if(_alfa <= 0.2){
        _size = _size / 2;
        Pair<K, V> ** tempTable = new Pair<K, V>*[_size];
        for(int i = 0; i < _size; i++){
            tempTable[i] = new Pair<K, V>[_bucketSize];
        }
        for(int i = 0; i < _size*2; i++){
            for(int j = 0; j < _bucketSize; j++){
                tempTable[hash(_table[i][j]._key)][j] = _table[i][j];
            }
        }
        for(int i = 0; i < _size*2; i++){
            delete[] _table[i];
        }
        delete[] _table;
        _table = tempTable;
    }
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
    _type = 0;
}

template <typename K, typename V>
SeparateChaining<K, V>::SeparateChaining(int size){
    _size = size;
    _table = new Pair<K, V>*[_size];
    for(int i = 0; i < _size; i++){
        _table[i] = new Pair<K, V>[_bucketSize];
    }
    _capacity = 0;
    _type = 0;

}

template <typename K, typename V>
SeparateChaining<K, V>::SeparateChaining(int size, int type){
    _size = size;
    _table = new Pair<K, V>*[_size];
    for(int i = 0; i < _size; i++){
        _table[i] = new Pair<K, V>[_bucketSize];
    }
    _capacity = 0;
    _type = type;
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
SeparateChaining<K, V>::SeparateChaining(std::string filename, int type, int size){
    /**
     * @todo Implement this function
    */
}

template <typename K, typename V>
void SeparateChaining<K, V>::insert(K key, V value){
    int i = 0;
    while(_table[hash(key)][i]._key != INT_MIN && _table[hash(key)][i]._key != key){
        i++;
    }
    if(_table[hash(key)][i]._key != key){
        _table[hash(key)][i] = Pair<K, V>(key, value);
        _capacity++;
        resizeUp();
    }
}

template <typename K, typename V>
void SeparateChaining<K, V>::remove(K key){
    int i = 0;
    while(_table[hash(key)][i]._key != key){
        i++;
    }
    _table[hash(key)][i] = Pair<K, V>();
    _capacity--;
    refactorBucket(hash(key));
    resizeDown();
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
                std::cout << _table[i][j]._key << " " << _table[i][j]._value << std::endl;
            }
        }
    }
    std::cout << std::endl;
}
