#pragma once

#include <string>
#include <fstream>

using std::string;

class Config {
    static string csrftoken;
    static string leetcodeSession;
    static string dbname;
    static string user;
    static string password;
    static string hostaddr;
    static string port;

public:
    static void getConfig() {
        std::ifstream configFile("../config.conf");
        string param;

        while (std::getline(configFile, param)) {
            if (param.empty() || param[0] == '[')
                continue;

            switch(param[0]) {
                case 'd' : dbname = param.substr(param.find('='), param.length() - param.find('='));
                case 'u' : user = param.substr(param.find('='), param.length() - param.find('='));
                case 'p' : {
                    if (param[1] == 'a')
                        password = param.substr(param.find('='), param.length() - param.find('='));
                    else
                        port = param.substr(param.find('='), param.length() - param.find('='));
                }
                case 'h' : hostaddr = param.substr(param.find('='), param.length() - param.find('='));
                case 'c' : csrftoken = param.substr(param.find('='), param.length() - param.find('='));
                case 'L' : leetcodeSession = param.substr(param.find('='), param.length() - param.find('='));
            }
        }
    }

    static const string &getCsrftoken() { return csrftoken; }

    static const string &getLeetcodeSession() { return leetcodeSession; }

    static const string &getDbname() { return dbname; }

    static const string &getUser() { return user; }

    static const string &getPassword() { return password; }

    static const string &getHostaddr() { return hostaddr; }

    static const string &getPort() { return port; }
};
