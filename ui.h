#pragma once
#include "HashTable.h"
#include "Pair.h"
#include <string>
#include <climits>
/**
 * @brief generate random numbers and save them to file
 * @param n number of elements
 * @param filename name of the file
*/
void generateFileData(int n, std::string filename = "test");
/**
 * @brief generate random number
 * @param min minimal value
 * @param max maximal value
*/
int generateNumber(int min, int max);
/**
 * @brief tests funcion
*/
void tests();
/**
 * @brief insert element to hash table
 * @param table hash tables
 * @param haschName name of the hash table
*/
void uiInsert(HashTable<int, int> ** table, std::string* haschName);
/**
 * @brief remove element from hash table
 * @param table hash tables
 * @param haschName name of the hash table
*/
void uiRemove(HashTable<int, int> ** table, std::string* haschName);
/**
 * @brief find element in hash table
 * @param table hash tables
 * @param haschName name of the hash table
*/
void uiFind(HashTable<int, int> ** table, std::string* haschName);
/**
 * @brief print keys from hash table
 * @param table hash tables
 * @param haschName name of the hash table
*/
void uiPrintKeys(HashTable<int, int> ** table, std::string* haschName);
/**
 * @brief print values from hash table
 * @param table hash tables
 * @param haschName name of the hash table
*/
void uiPrintValues(HashTable<int, int> ** table, std::string* haschName);
/**
 * @brief print hash table
 * @param table hash tables
 * @param haschName name of the hash table
*/
void uiPrint(HashTable<int, int> ** table, std::string* haschName);
