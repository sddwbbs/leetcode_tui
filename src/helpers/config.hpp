#pragma once

#include <string>
#include <fstream>

using std::string;

class Config {
    static inline string csrftoken;
    static inline string leetcodeSession;
    static inline string dbname;
    static inline string user;
    static inline string password;
    static inline string hostaddr;
    static inline string port;

public:
    static void getConfig() {
        std::ifstream configFile("../config.conf");
        string param;

        while (std::getline(configFile, param)) {
            if (param.empty() || param[0] == '[')
                continue;

            switch(param[0]) {
                case 'd' : dbname = param.substr(param.find('=') + 1, param.length() - param.find('=')); break;
                case 'u' : user = param.substr(param.find('=') + 1, param.length() - param.find('=')); break;
                case 'p' : {
                    if (param[1] == 'a')
                        password = param.substr(param.find('=') + 1, param.length() - param.find('='));
                    else
                        port = param.substr(param.find('=') + 1, param.length() - param.find('='));
                } break;
                case 'h' : hostaddr = param.substr(param.find('=') + 1, param.length() - param.find('=')); break;
                case 'c' : csrftoken = param.substr(param.find('=')+ 1, param.length() - param.find('=')); break;
                case 'L' : leetcodeSession = param.substr(param.find('=') + 1, param.length() - param.find('='));
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
