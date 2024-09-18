#pragma once

#include "constants.hpp"

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
    static bool getConfig() {
        string param;
        std::ifstream configFile;

        if (RUNNING_IN_DOCKER)
            configFile.open(CONFIG_PATH_DOCKER);
        else
            configFile.open(CONFIG_PATH_LOCAL);

        if (configFile.is_open()) {
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

            if (dbname.empty() ||
                user.empty() ||
                password.empty() ||
                port.empty() ||
                hostaddr.empty() ||
                csrftoken.empty() ||
                leetcodeSession.empty()) {
                return false;
            }
        } else {
            return false;
        }

        return true;
    }

    static const string &getCsrftoken() { return csrftoken; }

    static const string &getLeetcodeSession() { return leetcodeSession; }

    static const string &getDbname() { return dbname; }

    static const string &getUser() { return user; }

    static const string &getPassword() { return password; }

    static const string &getHostaddr() { return hostaddr; }

    static const string &getPort() { return port; }
};