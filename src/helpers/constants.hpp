#pragma once

#include <string>

using std::string;

const static string SCRIPTS_PATH_DOCKER = "/usr/src/app/scripts/sql/";
const static string SCRIPTS_PATH_LOCAL = "../scripts/sql/";
const static string CONFIG_PATH_DOCKER = "/usr/src/app/config.conf";
const static string CONFIG_PATH_LOCAL = "../config.conf";

static bool RUNNING_IN_DOCKER;