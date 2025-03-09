#include "constants.hpp"

const string SCRIPTS_PATH_DOCKER = "/usr/src/app/scripts/sql/";
const string SCRIPTS_PATH_LOCAL = "../scripts/sql/";
const string CONFIG_PATH_DOCKER = "/usr/src/app/config.conf";
const string CONFIG_PATH_LOCAL = "../config.conf";

int totalRows;
int totalCols;

const int &TOTAL_ROWS = totalRows;
const int &TOTAL_COLS = totalCols;

const int MAX_RETRIES = 5;
const int DELAY_MS = 500;

bool RUNNING_IN_DOCKER = []() -> bool {
    if (const char *runningInDocker = std::getenv("RUNNING_IN_DOCKER"); runningInDocker != nullptr)
        return true;
    else
        return false;
}();

const string HTML_WRAPPER = R"(<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Task</title>
    <style>
        body {
            background-color: #121212;
            color: #ffffff;
            font-family: 'Courier New', monospace;
            font-size: 12px;
            line-height: 1.5;
            margin: 20px;
        }

        pre {
            background-color: #1e1e1e;
            padding: 10px;
            border-radius: 5px;
            overflow-x: auto;
        }

        code {
            font-family: 'Courier New', monospace;
            background-color: #333;
            padding: 2px 4px;
            border-radius: 3px;
        }

        strong {
            color: #ffcc00;
        }

        ul {
            padding-left: 20px;
        }

        li {
            margin-bottom: 5px;
        }
    </style>

    <script>
        function resizeAndCenterWindow() {
            const width = Math.round(window.screen.width * 0.65);
            const height = Math.round(window.screen.height * 0.60);
            const left = Math.round((window.screen.width - width) / 2);
            const top = Math.round((window.screen.height - height) / 2);

            window.resizeTo(width, height);
            window.moveTo(left, top);
        }

        window.onload = function () {
            resizeAndCenterWindow();

            const taskTitle = document.body.getAttribute("data-title");
            if (taskTitle) {
                document.title = taskTitle;
            }
        };

        document.addEventListener("keydown", function(event) {
            if (event.key === "q") {
                window.close();
            } else if (event.key === "j") {
                window.scrollBy(0, window.innerHeight * 0.2);
            } else if (event.key === "k") {
                window.scrollBy(0, -window.innerHeight * 0.2);
            }
        });
    </script>
</head>
)";