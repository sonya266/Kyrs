#include "DataReader.h"


string DataReader::get_FileReader(){
    return FileReader;
}


void DataReader::set_FileReader(string file){
    FileReader = file;
}


pair<vector<string>, vector<string>> DataReader::getClient(){
    vector<string> firstVector;
    vector<string> secondVector;

    ifstream inputFile(get_FileReader());

    if (!inputFile.is_open()){
        inputFile.close();
        Err.error_recording("критичная", "Fun: getClient. Файла с БД открыть невозможно.");
        
    }
    else if(inputFile.peek() == std::ifstream::traits_type::eof()) {
        inputFile.close();
        Err.error_recording("критичная", "Fun: getClient. Файл с БД пуст.");

    }
    
    else
    {
        std::string line;
        while (std::getline(inputFile, line)) {
            std::istringstream ss(line);
            std::string first, second;
            
            // Чтение двух строк, разделенных пробелом
            if (std::getline(ss, first, ':') && std::getline(ss, second, ':')) {
                firstVector.push_back(first);
                secondVector.push_back(second);
            }
        }

        inputFile.close();
    }
    
    return std::make_pair(firstVector, secondVector);
}
