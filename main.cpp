/** 
* @file main.cpp
* @brief Функция для предоставления пользователю справки по использованию программы
*/

#include "ServerClientInterface.h"
#include <boost/program_options.hpp>


using namespace std;

/**
* @brief Функция для получения справки по использованию программы
*/
namespace po = boost::program_options;

void help(const po::options_description& desc) {
    cout << desc <<endl;
}

int main(int argc, char *argv[]) {
    try {
        // Опции командной строки
        po::options_description desc("Допустимые параметры");
        desc.add_options()
            ("help,h", "Вывод справки")
            ("Port,p", po::value<int>()->default_value(33333), "Порт сервера")
            ("Db,d", po::value<std::string>()->default_value("/etc/vcalc.conf"), "Файл с базой данных клиентов")
            ("Log,l", po::value<std::string>()->default_value("/var/log/vcalc.log"), "Файл для ошибок");

        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);

        if (vm.count("help")) {
            help(desc);
            return 0;
        }

        int port = vm["Port"].as<int>();
        std::string database = vm["Db"].as<std::string>();
        std::string logFile = vm["Log"].as<std::string>();

        ServerClientInterface newServer;
        newServer.set_address("127.0.0.1");
        newServer.set_port(port);
        newServer.interaction(database, logFile);

    } catch (const std::exception& e) {
        std::cerr << "Критическая ошибка: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
