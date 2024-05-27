#include "ui.h"
#include <iostream>
#include <string>
#include <climits>
#include <fstream>
#include <chrono>
#include <random>
#include "SeparateChaining.h"
#include "OpenAddressing.h"
//#include "Cuckoo.h"

void generateFileData(int n, std::string filename){
    std::ofstream file;
    file.open(filename + ".txt");
    for(int i = 0; i < n; i++){
        file << i << " " << generateNumber(0, INT_MAX) << std::endl;
    }
    file.close();
}

int generateNumber(int min, int max){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

void tests(){
    int sizes[8] = {1000, 2000, 4000, 8000, 16000, 32000, 64000, 128000};
    //int sizes[4] = {1000, 2000, 4000, 8000};
    int sets = 1;
    int iterations = 100;
    OpenAddressing<int, int> * openAddressing;
    SeparateChaining<int, int> * separateChaining;
    std::string filename = "test_";
    std::fstream file("wyniki.csv", std::ios::out);
    if(!file.is_open()){
        std::cout << "Nie udało się otworzyć pliku wyniki.txt" << std::endl;
        return;
    }
    long long int time; 
    file << "NameOfTableAndType;Size;Operation;Time";
    for(int s = 0; s < sets; s++){
        time = 0;
        for(auto& size: sizes){
            filename = "test_";
            filename = filename + std::to_string(size)+"_"+std::to_string(s);
            generateFileData(size, filename);
            // OpenAddressing linear
            for(int i = 0; i < iterations; i++){
                openAddressing = new OpenAddressing<int, int>(filename + ".txt", size*2);
                auto start = std::chrono::high_resolution_clock::now();
                openAddressing->insert(generateNumber(0, INT_MAX), generateNumber(0, INT_MAX));
                auto end = std::chrono::high_resolution_clock::now();
                time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                delete openAddressing;
            }
            file << "OpenAddressing; " << size << "; Insert; " << time/iterations << "\n";
            std::cout << "OpenAddressing; " << size << "; Insert; " << time/iterations << "\n";
            time = 0;
            
            for(int i = 0; i < iterations; i++){
                openAddressing = new OpenAddressing<int, int>(filename + ".txt", size*2);
                auto start = std::chrono::high_resolution_clock::now();
                openAddressing->remove(generateNumber(0, size));
                auto end = std::chrono::high_resolution_clock::now();
                time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                delete openAddressing;
            }
            file << "OpenAddressing; " << size << "; Remove; " << time/iterations << "\n";
            std::cout << "OpenAddressing; " << size << "; Remove; " << time/iterations << "\n";
            time = 0;
            for(int i = 0; i < iterations; i++){
                openAddressing = new OpenAddressing<int, int>(filename + ".txt", size*2);
                auto start = std::chrono::high_resolution_clock::now();
                openAddressing->find(generateNumber(0, size));
                auto end = std::chrono::high_resolution_clock::now();
                time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                delete openAddressing;
            }
            file << "OpenAddressing; " << size << "; Find; " << time/iterations << "\n";
            std::cout << "OpenAddressing; " << size << "; Find; " << time/iterations << "\n";
            time = 0;

            //SeparateChaining modulo
            for(int i = 0; i < iterations; i++){
                separateChaining = new SeparateChaining<int, int>(filename + ".txt", size*2);
                auto start = std::chrono::high_resolution_clock::now();
                separateChaining->insert(generateNumber(0, INT_MAX), generateNumber(0, INT_MAX));
                auto end = std::chrono::high_resolution_clock::now();
                time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                delete separateChaining;
            }
            file << "SeparateChaining; " << size << "; Insert; " << time/iterations << "\n";
            std::cout << "SeparateChaining; " << size << "; Insert; " << time/iterations << "\n";
            time = 0;
            for(int i = 0; i < iterations; i++){
                separateChaining = new SeparateChaining<int, int>(filename + ".txt", size*2);
                auto start = std::chrono::high_resolution_clock::now();
                separateChaining->remove(generateNumber(0, size));
                auto end = std::chrono::high_resolution_clock::now();
                time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                delete separateChaining;
            }
            file << "SeparateChaining; " << size << "; Remove; " << time/iterations << "\n";
            std::cout << "SeparateChaining; " << size << "; Remove; " << time/iterations << "\n";
            time = 0;
            for(int i = 0; i < iterations; i++){
                separateChaining = new SeparateChaining<int, int>(filename + ".txt", size*2);
                auto start = std::chrono::high_resolution_clock::now();
                separateChaining->find(generateNumber(0, size));
                auto end = std::chrono::high_resolution_clock::now();
                time += std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
                delete separateChaining;
            }
            file << "SeparateChaining; " << size << "; Find; " << time/iterations << "\n";
            std::cout << "SeparateChaining; " << size << "; Find; " << time/iterations << "\n";
            time = 0;
            
        }
    }
    file.close();
}

void uiInsert(HashTable<int, int> ** table, std::string* haschName){
    long long int time[3];
    int key, value;
    std::cout << "Podaj klucz: ";
    std::cin >> key;
    std::cout << "Podaj wartosc: ";
    std::cin >> value;
    for(int i = 0; i < 2; i++){
        auto start = std::chrono::high_resolution_clock::now();
        table[i]->insert(key, value);
        auto end = std::chrono::high_resolution_clock::now();
        time[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }
    std::cout << "Wstawiono element o kluczu:" << key << " i wartości: " << value << std::endl;
    for(int i = 0; i < 2; i++){
        std::cout << haschName[i] << "Czas: " << time[i] << std::endl;
    }
}

void uiRemove(HashTable<int, int> ** table, std::string* haschName){
    long long int time[3];
    int key;
    std::cout << "Podaj klucz: ";
    std::cin >> key;
    for(int i = 0; i < 2; i++){
        auto start = std::chrono::high_resolution_clock::now();
        table[i]->remove(key);
        auto end = std::chrono::high_resolution_clock::now();
        time[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }
    std::cout << "Usunieto element o kluczu:" << key << std::endl;
    for(int i = 0; i < 2; i++){
        std::cout << haschName[i] << "Czas: " << time[i] << std::endl;
    }
}

void uiFind(HashTable<int, int> ** table, std::string* haschName){
    long long int time[3];
    int key;
    std::cout << "Podaj klucz: ";
    std::cin >> key;
    int value;
    for(int i = 0; i < 2; i++){
        auto start = std::chrono::high_resolution_clock::now();
        value = table[i]->find(key);
        auto end = std::chrono::high_resolution_clock::now();
        time[i] = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
    }
    std::cout << "Znaleziono element o kluczu:" << key << std::endl;
    std::cout << "Wartość: " << value << std::endl;
    for(int i = 0; i < 2; i++){
        std::cout << haschName[i] << "Czas: " << time[i] << std::endl;
    }
}

void uiPrintKeys(HashTable<int, int> ** table, std::string* haschName){
    int typeOfTable;
    std::cout << "Wybierz tabele: " << std::endl;
    std::cout << " 1 - OpenAddressing" << std::endl;
    std::cout << " 2 - SeparateChaining" << std::endl;
    std::cout << " 3 - Cuckoo" << std::endl;
    std::cin >> typeOfTable;
    switch (typeOfTable){
        case 1:
            table[0]->keys();
            break;
        case 2:
            table[1]->keys();
            break;
        case 3:
            table[2]->keys();
            break;
        default:
            break;
    }
}

void uiPrintValues(HashTable<int, int> ** table, std::string* haschName){
    int typeOfTable;
    std::cout << "Wybierz tabele: " << std::endl;
    std::cout << " 1 - OpenAddressing" << std::endl;
    std::cout << " 2 - SeparateChaining" << std::endl;
    std::cout << " 3 - Cuckoo" << std::endl;
    std::cin >> typeOfTable;
    switch (typeOfTable){
        case 1:
            table[0]->values();
            break;
        case 2:
            table[1]->values();
            break;
        case 3:
            table[2]->values();
            break;
        default:
            break;
    }

}

void uiPrint(HashTable<int, int> ** table, std::string* haschName){
    int typeOfTable;
    std::cout << "Wybierz tabele: " << std::endl;
    std::cout << " 1 - OpenAddressing" << std::endl;
    std::cout << " 2 - SeparateChaining" << std::endl;
    std::cout << " 3 - Cuckoo" << std::endl;
    std::cin >> typeOfTable;
    switch (typeOfTable){
        case 1:
            table[0]->print();
            break;
        case 2:
            table[1]->print();
            break;
        case 3:
            table[2]->print();
            break;
        default:
            break;
    }
}
