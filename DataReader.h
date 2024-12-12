/** 
* @file DataReader.h
* @author Дьякова С.М.
* @version 1.0
* @date 12.12.2023
* @copyright ИБСТ ПГУ
* @brief Заголовочный файл для модуля DataReader
*/
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Errors.h"

using namespace std;

/** 
* @brief Класс для получения БД клиентов
* @param FileReader путь к файлу с БД клиентов
* @param Err объект класса Errors обработки ошибок
*/
class DataReader{
    public:

        string get_FileReader();
        void set_FileReader(string file);

        pair<vector<string>, vector<string>> getClient();
    private:
        string FileReader;
        Errors Err;
};
