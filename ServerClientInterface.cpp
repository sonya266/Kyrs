/**
* @file ServerClientInterface.cpp
*/
#include "ServerClientInterface.h"

/**
* @brief Геттер для атрибута address
*/
string ServerClientInterface::get_address(){
    return address;
}


struct Result {
    std::string hash;
    std::string salt;
    std::string login;
};

Result splitString(const std::string& input) {
    Result result;


    if (input.length() < 48) {
        throw std::invalid_argument("Input string is too short.");
    }
    result.hash = input.substr(input.length() - 32, 32);
    result.salt = input.substr(input.length() - 48, 16);
    result.login = input.substr(0, input.length() - 48);
    return result;
}

/**
* @brief Сеттер для атрибута address
*/
void ServerClientInterface::set_address(string address1){
    const vector<string> list_add = {"127.0.0.1"};
    int fl = 0;
    for(auto it:list_add){
        if(it == address1){
            fl = 1;
            address = address1;
        }
    }
    if(fl != 1){
        Err.error_recording("критичная", "Fun: set_address. Адрес сервера не соответствует разрешенным.");
    }
}

/**
* @brief Геттер для атрибута port
*/
int ServerClientInterface::get_port(){
    return port;
}

/**
* @brief Сеттер для атрибута port
*/
void ServerClientInterface::set_port(int port1){
    if (port1 < 1023){
        Err.error_recording("критичная", "Fun: set_port. Порт сервера должен быть больше 1024.");
    }

    port = port1;
}

/**
* @brief Сеттер для атрибута vec
*/
vector<uint64_t> ServerClientInterface::get_vec(){
    return vec;
}

/**
* @brief Геттер для атрибута vec
*/
void ServerClientInterface::set_vec(vector<uint64_t> v){
    vec = v;
}

/**
* @brief Геттер для атрибута salt
*/
string ServerClientInterface::get_salt(){
    return salt;
}

/**
* @brief Сеттер для атрибута salt
*/
void ServerClientInterface::set_salt(){
    // Символы, которые могут встречаться в строке SALT
    const string saltCharacters = "0123456789ABCDEF";
    const int saltLength = 16;

    // Генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dis(0, saltCharacters.length() - 1);

    std::string Salt;
    Salt.reserve(saltLength);

    for (int i = 0; i < saltLength; ++i) {
        int randomIndex = dis(gen);
        Salt += saltCharacters[randomIndex];
    }

    salt = Salt;
}

/**
* @brief Геттер для атрибута DB_clients
*/
pair<vector<string>, vector<string>> ServerClientInterface::get_DB_clients(){
    return DB_clients;
}

/**
* @brief Сеттер для атрибута DB_clients
*/
void ServerClientInterface::set_DB_clients(vector<string> login, vector<string> password){
    DB_clients = std::make_pair(login,password);
}

/**
* @brief Функция вычисления суммы векторов
* @return Возвращает сумму векторов атрибута vec
*/
uint64_t ServerClientInterface::sum_vec(){
    uint64_t sum = 0;
    for (uint64_t value : vec) {
        // Проверка на переполнение
        if (sum > 9223372036854775807 - value) {
            // Обработка переполнения (в данном случае, просто возвращаем максимальное значение)
            return 9223372036854775807;
        }
        
        sum += value;
    }
    return sum;
}


int ServerClientInterface::interaction(string database, string logFile){
    
    // Получение пути к файлу для записи ошибок
    Errors Err;
    Err.set_File_Log(logFile);

    // Получение пути к файлу с БД клиентов
    DataReader DB;
    DB.set_FileReader(database);
    auto db = DB.getClient();

    vector<std::string> str1 = db.first;
    vector<std::string> str2 = db.second;

    set_DB_clients(str1, str2);

    // Создание сокета
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
       Err.error_recording("критичная", "Fun: interaction. Ошибка при создании сокета.");
    }

    // Настраиваем адрес сервера
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(get_address().c_str());
    serverAddress.sin_port = htons(get_port());

    // Привязка сокета к адресу
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        close(serverSocket);
        Err.error_recording("критичная", "Fun: interaction. Ошибка при привязке сокета к адресу.");
    }

    // Прослушивание порта
    if (listen(serverSocket, 5) == -1) {
        close(serverSocket);
        Err.error_recording("критичная", "Fun: interaction. Ошибка при начале прослушивания порта.");
    }

    std::cout << "\nСервер запущен на адресе " << get_address();
    std::cout << " и порту " << get_port() << std::endl;

     while (true) {
        // Принимаем соединение от клиента
        struct sockaddr_in clientAddress;
        socklen_t clientAddressLength = sizeof(clientAddress);
        int clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket == -1) {
            close(serverSocket);
            Err.error_recording("некритичная", "Fun: interaction. Ошибка при принятии соединения.");
        }

        cout << "ПОДКЛЮЧЕНИЕ - YES" << endl;


        char buf[1024] = {0};

        // Получение от клиента логина+соль+хеш
        ssize_t bytes_read = recv(clientSocket, buf, sizeof(buf), 0);
        if (bytes_read <= 0) {
        close(clientSocket);
        Err.error_recording("некритичная", "Fun: interaction. Ошибка при получении данных.");
        continue;
        }
        /*for(ssize_t i = 0; i < bytes_read; ++i) {
        if(buf[i] == '\n') {
           buf[i] = '\0';
            break;
        }
    }*/
        cout << "ПОДКЛЮЧЕНИЕ2 - YES" << endl;
        Result result = splitString(string(buf));
        cout << buf <<endl;
        cout << result.login << "|"<<endl;
        cout << result.hash << "|"<<endl;
        cout << result.salt <<"|" <<endl;
        cout << "ПОДКЛЮЧЕНИЕ3 - YES" << endl;
        // Проверка наличия логина в базе данных
        User u;
        u.set_ID(result.login);
        string s1;
        cout << result.login << endl;
      
        if(!u.СheckLogin(DB_clients.first)){
            
            s1 = "ERR";
            cout << "АУТЕНТИФИКАЦИЯ - NO" << endl;
            cout << "ЗАКРЫТИЕ СОКЕТА" << endl;
            strcpy(buf,s1.c_str());
            send(clientSocket, buf, s1.length(), 0);
            close(clientSocket);
            Err.error_recording("некритичная", "Fun: interaction. Ошибка аутентификации");
            continue;
            } else {
            cout << "Логин найден" << endl;
                   }
        
        cout << "ПОДКЛЮЧЕНИЕ4 - YES" << endl;
        
        if (!u.CheckPassword(DB_clients.second, DB_clients.first, result.salt, result.hash)) {
            string response = "ERR";
            cout << "АУТЕНТИФИКАЦИЯ - NO" << endl;
            cout << "ЗАКРЫТИЕ СОКЕТА" << endl;
            send(clientSocket, response.c_str(), response.length(), 0);
            close(clientSocket);
            Err.error_recording("некритичная", "Fun: interaction. Ошибка аутентификации: неверный пароль.");
            continue;
        }


        string s4 = "OK";
        cout << "АУТЕНТИФИКАЦИЯ - YES" << endl;
        strcpy(buf,s4.c_str());
        send(clientSocket, buf, s4.length(), 0);


        // Получение веторов

        // Получение количества векторов
        uint32_t col = 0;
        recv(clientSocket, &col, sizeof(col), 0);
        cout << "КОЛИЧЕСТВО ВЕКТОРОВ: " << col << endl;

        for(auto i = 0; i < col; i++){

            // Получение длины вектора
            uint32_t vec_len = 0;
            recv(clientSocket, &vec_len, sizeof(vec_len), 0);
            cout << "ДЛИНА ВЕКТОРА: " << vec_len << endl;

            // Получение вектора
            uint64_t arr[vec_len] = {0};

            recv(clientSocket, &arr, sizeof(arr), 0);

            vector<uint64_t> vec1;
            for (size_t j = 0; j < vec_len; j++) {
            vec1.push_back(arr[j]);
            cout << vec1[j] << " ";
            }
            cout << "\n";

            // Подсчет результатов
            set_vec(vec1);
            auto res = sum_vec();
            cout << "РЕЗУЛЬТАТ ВЫЧИСЛЕНИЙ: " << res << endl;

            // Отправка результата
            send(clientSocket, &res, sizeof(res), 0);


        }
        // Закрываем соединение с клиентом
        cout << "ЗАКРЫТИЕ СОКЕТА" << endl;
        close(clientSocket);
    }

    // Закрываем серверный сокет
    close(serverSocket);

    return 0;
}

