#pragma once
#include "DoublyLinkedList.h"
#include "Pair.h"
#include "HashTable.h"
#include <climits>
#include <string>
#include <fstream>
/**
 * @brief SeparateChaining class
 * @param table: array of doubly linked lists
 * @param _size: size of the table  
 * @param _capacity: capacity of the table
 * @param _alfa: load factor
*/
template <typename K, typename V>
class SeparateChaining: public HashTable<K, V>{
    DoublyLinkedList<K, V>* table;
    int _size;
    int _capacity;
    float _alfa;
/**
 * @brief Hash function
 * @param key: key to hash
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
 * @param size: size of the table
*/
    SeparateChaining(int size);
/**
 * @brief Constructors
 * @param filename: name of the file
 * @param size: size of the table
*/
    SeparateChaining(std::string fileName, int size);
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
        delete[] table;
    };
};

template <typename K, typename V>
int SeparateChaining<K, V>::hash(K key){
    return key % _size;
};

template <typename K, typename V>
void SeparateChaining<K, V>::resizeUp(){
    _alfa = static_cast<float>(_capacity) / static_cast<float>(_size);
    if(_alfa >= 3.0){
        _size = _size*2;
        DoublyLinkedList<K, V>* temp = new DoublyLinkedList<K, V>[_size];
        for(int i = 0; i < _size/2; i++){
            Node<K, V>* tempNode = table[i].getHead();
            while(tempNode != nullptr){
                int index = hash(tempNode->_pair._key);
                temp[index].insert(tempNode->_pair._key, tempNode->_pair._value);
                tempNode = tempNode->next;
            }
        }
        delete[] table;
        table = temp;
    }
};

template <typename K, typename V>
void SeparateChaining<K, V>::resizeDown(){
    _alfa = static_cast<float>(_capacity) / static_cast<float>(_size);
    if(_alfa <= 1){
        _size = _size/2;
        DoublyLinkedList<K, V>* temp = new DoublyLinkedList<K, V>[_size];
        for(int i = 0; i < _size*2; i++){
            Node<K, V>* tempNode = table[i].getHead();
            while(tempNode != nullptr){
                int index = hash(tempNode->_pair._key);
                temp[index].insert(tempNode->_pair._key, tempNode->_pair._value);
                tempNode = tempNode->next;
            }
        }
        delete[] table;
        table = temp;
    }
};

template <typename K, typename V>
SeparateChaining<K, V>::SeparateChaining(){
    table = new DoublyLinkedList<K, V>[10];
    _size = 10;
    _capacity = 0;
};

template <typename K, typename V>
SeparateChaining<K, V>::SeparateChaining(int size){
    table = new DoublyLinkedList<K, V>[size];
    _size = size;
    _capacity = 0;
};

template <typename K, typename V>
SeparateChaining<K, V>::SeparateChaining(std::string fileName, int size){
    int optimal_size = size/2;
    _size = optimal_size;
    table = new DoublyLinkedList<K, V>[_size];
    _capacity = 0;
    std::ifstream file(fileName);
    if (file.is_open()) {
        K key;
        V value;
        while (file >> key >> value) {
            insert(key, value);
        }
    }
}

template <typename K, typename V>
void SeparateChaining<K, V>::insert(K key, V value){
    int index = hash(key);
    Node<K, V>* node = table[index].exist(key);
    if(node != nullptr){
        node->_pair._value = value;
        return;
    }else{
        table[index].insert(key, value);
        _capacity++;
        resizeUp();
    }
}

template <typename K, typename V>
void SeparateChaining<K, V>::remove(K key){
    int index = hash(key);
    table[index].remove(key);
    _capacity--;
    resizeDown();
}

template <typename K, typename V>
V SeparateChaining<K, V>::find(K key){
    int index = hash(key);
    Node<K, V>* node = table[index].exist(key);
    if(node != nullptr){
        return node->_pair._value;
    }
    return INT_MIN;
};

template <typename K, typename V>
bool SeparateChaining<K, V>::exists(K key){
    int index = hash(key);
    Node<K, V>* node = table[index].exist(key);
    if(node != nullptr){
        return true;
    }
    return false;
};

template <typename K, typename V>
int SeparateChaining<K, V>::size(){
    return _capacity;
};

template <typename K, typename V>
bool SeparateChaining<K, V>::empty(){
    return _capacity == 0;
};

template <typename K, typename V>
void SeparateChaining<K, V>::keys(){
    for(int i = 0; i < _size; i++){
        table[i].printKeys();
    }
};

template <typename K, typename V>
void SeparateChaining<K, V>::values(){
    for(int i = 0; i < _size; i++){
        table[i].printValues();
    }
};

template <typename K, typename V>
void SeparateChaining<K, V>::print(){
    for(int i = 0; i < _size; i++){
        table[i].print();
    }
};
