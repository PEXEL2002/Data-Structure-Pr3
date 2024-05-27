#include "HashTable.h"
#include "OpenAddressing.h"
#include "SeparateChaining.h"
#include "ui.h"
#include <string>
#include <climits>
#include <iostream>
using namespace std;

int main(){
    string haschName [3] = {"Adresowanie otwarte ", "Metoda łańcuchowa   ", "Metoda Cuckoo       "}; //nazwy metod mieszania
    int ifTests;
    do{
        cout << "Czy przeprowadzić testy(1 - Tak, 0 - Nie): ";
        cin >> ifTests;
    }while(ifTests != 0 && ifTests != 1);
    if(ifTests){
        tests();
    }else{
        int mainLoop = 1;
        while(mainLoop){
            int size;
            do{
                cout << "Podaj rozmiar tablicy: ";
                cin >> size;
            }while(size <= 0);
            generateFileData(size, "prezentacja");
            HashTable<int, int> ** tables = new HashTable<int, int>*[3];
            tables[0] = new OpenAddressing<int, int>("prezentacja.txt",size);
            tables[1] = new SeparateChaining<int, int>("prezentacja.txt",size);
            //tables[2] = new Cuckoo<int, int>("prezentacja.txt",size);
            int oper = 1;
            int oparation = 0;
            while(oper){
                cout << "Wybierz operacje:" << endl;
                cout << " 1 - Wstaw" << endl;
                cout << " 2 - Usun" << endl;
                cout << " 3 - Znajdz" << endl;
                cout << " 4 - Wyswietl klucze" << endl;
                cout << " 5 - Wyswietl wartości" << endl;
                cout << " 6 - Wyswietl tabele" << endl;
                cout << " 0 - Wyjscie lub generacja danych na nowo  " << endl;
                cin >> oparation;
                switch (oparation){
                    case 1:
                        uiInsert(tables, haschName );
                        break;
                    case 2:
                        uiRemove(tables, haschName);
                        break;
                    case 3:
                        uiFind(tables, haschName);
                        break;
                    case 4:
                        uiPrintKeys(tables, haschName);
                        break;
                    case 5:
                        uiPrintValues(tables, haschName);
                        break;
                    case 6:
                        uiPrint(tables, haschName);
                        break;  
                    case 0:
                        oper = 0;
                        break;
                    default:
                        break;
                }

            }
            cout << "Czy zakończyć działąnie programu(1 - Tak, 0 - Nie): ";
            cin >> mainLoop;
            for(int i = 0; i < 3; i++){
                delete tables[i];
            }
            delete[] tables;
            if(mainLoop){
                mainLoop = 0;
            }
        }
   }
}