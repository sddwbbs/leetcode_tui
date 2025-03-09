#pragma once

#include <string>

using std::string;

extern const string SCRIPTS_PATH_DOCKER;
extern const string SCRIPTS_PATH_LOCAL;
extern const string CONFIG_PATH_DOCKER;
extern const string CONFIG_PATH_LOCAL;

extern int totalRows;
extern int totalCols;

extern const int &TOTAL_ROWS;
extern const int &TOTAL_COLS;

extern const int MAX_RETRIES;
extern const int DELAY_MS;

extern bool RUNNING_IN_DOCKER;

extern const string HTML_WRAPPER;