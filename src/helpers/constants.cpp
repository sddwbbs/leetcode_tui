#include "constants.hpp"

const string SCRIPTS_PATH_DOCKER = "/usr/src/app/scripts/sql/";
const string SCRIPTS_PATH_LOCAL = "../scripts/sql/";
const string CONFIG_PATH_DOCKER = "/usr/src/app/config.conf";
const string CONFIG_PATH_LOCAL = "../config.conf";

int totalRows;
int totalCols;

const int &TOTAL_ROWS = totalRows;
const int &TOTAL_COLS = totalCols;

bool RUNNING_IN_DOCKER = []() -> bool {
    if (const char *runningInDocker = std::getenv("RUNNING_IN_DOCKER"); runningInDocker != nullptr)
        return true;
    else
        return false;
}();