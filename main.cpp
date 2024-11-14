#include <getopt.h>
#include "ServerClientInterface.h"


using namespace std;


void help(){
    std::cout << "Эта программа представляет собой сервер для взаимодействия с клиентом.\n" << std::endl;
    std::cout << "Для корректной работы требуется указать параметры командной строки." << std::endl;
    std::cout << "Параметр -i или --Ip представляет собой сетевой адрес сервера и является обязательным." << std::endl;
    std::cout << "Параметр -p или --Port определяет порт сервера и является необязательным." << std::endl;
    std::cout << "Параметр -d или --Db указывает на файл с базой данных клиентов." << std::endl;
    std::cout << "Параметр -l или --Log определяет файл для аутентификации и является необязательным." << std::endl;
    std::cout << "Параметр -h или --Help для получения справки по использованию программу.\n" << std::endl;
}


int main (int argc, char *argv[]){

	// Опции командной строки
    const struct option long_options[] = {
        {"Ip", optional_argument, nullptr, 'i'},
        {"Port", optional_argument, nullptr, 'p'},
        {"Db", optional_argument, nullptr, 'd'},
        {"Log", optional_argument, nullptr, 'l'},
        {"Help", no_argument, nullptr, 'h'},
        {nullptr, 0, nullptr, 0}
    };

    // Класс для взаимодействия с клиент-сервером
    string address = "";
    int port = 0;
    string database  = "";
    string logFile = "";

    int opt;
    int option_index = 0;
    while ((opt = getopt_long(argc, argv, "i::p::d::l::h::", long_options, &option_index)) != -1) {

        switch (opt) {

        case 'i':
            if (optarg != nullptr) {
                address = string(optarg);
            }
            else{
                address = string("127.0.0.1");
            }
            break;
            
        case 'p':
            if (optarg != nullptr) {
                port = stoi(optarg);
            }
            else{
                port = stoi("33333");
            }
            break;
        
        case 'd':
            if (optarg != nullptr) {
                database = string(optarg);
            }
            else{
                database = string("DB.conf");
            }
            break;
        
        case 'l':
            if (optarg != nullptr) {
                logFile = string(optarg);
            }
            else{
                logFile = string("Log.conf");
            }
            break;
            
        case 'h':
            help();
            
            return 1;
        
        default:
            // Некорректная опция
            cout << "Error: Invalid option\n";
            help();
            return 2;
        }
    }

    if(address == "" or port == 0 or database == "" or logFile == ""){
        cout << "Не указаны параметры" << endl;
        help();
        return 0;
    }

	ServerClientInterface newServer;
    newServer.set_address(address);
    newServer.set_port(port);
    newServer.interaction(database, logFile);

    // ./server -i127.0.0.1 -p33333 -dDB.conf -llog.conf
	return 0;
}
