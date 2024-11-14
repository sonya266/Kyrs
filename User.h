#include <string>
#include  <vector>
#include <iostream>

#include "./md5/md5.h"

using namespace std;


class User{
    private:
        string ID;
        string hash;
    public:
        bool СheckLogin(vector<string> Db_ID);
        bool CheckPassword(vector<string> Db_password, vector<string> Db_ID, string SALT, string SendHash);
        
        string get_ID();
        void set_ID(string ID1);

        string get_hash();
        void set_hash(string hash1);

};
