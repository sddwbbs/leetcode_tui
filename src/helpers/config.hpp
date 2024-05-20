#pragma once

#include <string>

using std::string;

class Config {
    static inline string csrftoken;
    static inline string leetcodeSession;

public:
    static void setConfig(const string _csrftoken, const string _leetcodeSession) {
        csrftoken = _csrftoken;
        leetcodeSession = _leetcodeSession;
    }

    static const string& getCsrftoken() {
        return csrftoken;
    }

    static const string& getLeetcodeSession() {
        return leetcodeSession;
    }
};
