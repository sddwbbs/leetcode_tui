#pragma once

#include "constants.hpp"

#include <string>
#include <optional>

using std::string;

class Config {
    static inline string csrftoken;
    static inline string leetcodeSession;
    static inline string dbname;
    static inline string user;
    static inline string password;
    static inline string hostaddr;
    static inline string port;

    static std::optional<string> get_env(const char* env_var) {
        const char* value = std::getenv(env_var);
        return value ? std::optional<string>(value) : std::nullopt;
    }

public:
    static bool getConfig() {
        csrftoken       = get_env("LEETCODE_CSRFTOKEN").value_or("");
        leetcodeSession = get_env("LEETCODE_SESSION").value_or("");
        dbname          = get_env("LEETCODE_DBNAME").value_or("");
        user            = get_env("LEETCODE_USER").value_or("");
        password        = get_env("LEETCODE_PASSWORD").value_or("");
        hostaddr        = get_env("LEETCODE_HOSTADDR").value_or("");
        port            = get_env("LEETCODE_PORT").value_or("");

        if (csrftoken.empty()
            || leetcodeSession.empty()
            || dbname.empty()
            || user.empty()
            || password.empty()
            || hostaddr.empty()
            || port.empty()) return false;

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