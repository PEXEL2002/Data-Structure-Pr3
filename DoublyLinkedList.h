#pragma once
#include "Node.h"
#include "Pair.h"

template <typename K, typename V>
class DoublyLinkedList{
    Node<K, V>* head;
    Node<K, V>* tail;
    int size;
public:
    DoublyLinkedList();
    ~DoublyLinkedList();
    Node<K, V>* getHead(){
        return head;
    };
    Node<K, V>* exist(K key){
        Node<K, V>* temp = head;
        while(temp != nullptr){
            if(temp->_pair._key == key){
                return temp;
            }
            temp = temp->next;
        }
        return nullptr;
    };
    void insert(K key, V value);
    void remove(K key);
    V find(K key);
    void printKeys(){
        Node<K, V>* temp = head;
        while(temp != nullptr){
            std::cout << temp->_pair._key << " " << std::endl;
            temp = temp->next;
        }
    };
    void printValues(){
        Node<K, V>* temp = head;
        while(temp != nullptr){
            std::cout << temp->_pair._value << " " << std::endl;
            temp = temp->next;
        }
    };
    void print(){
        Node<K, V>* temp = head;
        while(temp != nullptr){
            std::cout << temp->_pair._key << "->" << temp->_pair._value << std::endl;
            temp = temp->next;
        }
    };
    int getSize(){
        return size;
    };
};

template <typename K, typename V>
DoublyLinkedList<K,V>::DoublyLinkedList(){
    head = nullptr;
    tail = nullptr;
    size = 0;
}

template <typename K, typename V>
DoublyLinkedList<K,V>::~DoublyLinkedList(){
    Node<K, V>* temp = head;
    while(temp != nullptr){
        Node<K, V>* next = temp->next;
        delete temp;
        temp = next;
    }
}


template <typename K, typename V>
void DoublyLinkedList<K,V>::insert(K key, V value){
    Node<K, V>* newNode = new Node<K, V>();
    newNode->_pair = Pair<K, V>(key, value);
    if(head == nullptr){
        head = newNode;
        tail = newNode;
    }else{
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}

template <typename K, typename V>
void DoublyLinkedList<K,V>::remove(K key){
    Node<K, V>* temp = head;
    while(temp != nullptr){
        if(temp->_pair._key == key){
            if(temp == head){
                head = temp->next;
                if(head != nullptr){
                    head->prev = nullptr;
                }
            }else if(temp == tail){
                tail = temp->prev;
                tail->next = nullptr;
            }else{
                temp->prev->next = temp->next;
                temp->next->prev = temp->prev;
            }
            delete temp;
            size--;
            return;
        }
        temp = temp->next;
    }
}

template <typename K, typename V>
V DoublyLinkedList<K,V>::find(K key){
    Node<K, V>* temp = head;
    while(temp != nullptr){
        if(temp->_pair._key == key){
            return temp->_pair._value;
        }
        temp = temp->next;
    }
    return V();
};