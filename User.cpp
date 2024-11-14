
#include "User.h"


string User::get_ID(){
    return ID;
}


void User::set_ID(string ID1){
    ID = ID1;

}


string User::get_hash(){
    return hash;
}


void User::set_hash(string hash1){
    hash = hash1;
}


bool User::СheckLogin(vector<string> Db_ID){
    for (size_t i = 0; i < Db_ID.size(); ++i) {
        if (Db_ID[i] == ID){
            return 1;
        }
    }

    return 0;
    
}


bool User::CheckPassword(vector<string> Db_password, vector<string> Db_ID, string SALT, string SendHash){
    int t = 0;
    for (size_t i = 0; i < Db_ID.size(); ++i) {
        if (Db_ID[i] == ID){
            t = i;
        }
    }
    
    string hashMD5 = SALT + Db_password[t];
    cout << hashMD5 <<endl;
    string HASHMD5 = MD5_hash(hashMD5);
    cout << SendHash << "|" << HASHMD5 <<endl;
    if (HASHMD5 == SendHash){
        return 1;
    }
    else{
        return 0;
    }
}
