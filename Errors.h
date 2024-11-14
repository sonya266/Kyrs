#include <string>
#include <chrono>
#include <ctime>
#include <iostream>
#include <fstream>
#include <iomanip>

using namespace std;


class Errors{
    public:
        string get_File_Log();
        void set_File_Log(string file);

        void error_recording(string flag, string info);

    private:
        string File_Log;
};
