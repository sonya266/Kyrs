#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>

#include "Errors.h"

using namespace std;


class DataReader{
    public:

        string get_FileReader();
        void set_FileReader(string file);

        pair<vector<string>, vector<string>> getClient();
    private:
        string FileReader;
        Errors Err;
};
