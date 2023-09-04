#ifndef TINYLANGUAGEPARSER_LOGGER_H
#define TINYLANGUAGEPARSER_LOGGER_H
#include <fstream>
#include <iostream>
#include <ctime>


class logger {
private:
    static std::ofstream logFile;

public:
    static void initialize() {
        logFile.open("log.txt", std::ios::out | std::ios::app);
        if (!logFile) {
            std::cerr << "Failed to open log file." << std::endl;
        } else {
            logFile << "[LOG]: Parsing..." << getTime() << std::endl;
            logFile << "[LOG]: script.txt..." << getTime() << std::endl;
            logFile << "[LOG]: " << getTime() << std::endl;
        }
    }

    static void close() {
        if (logFile) {
            logFile << "--------------- DONE ---------------- " << std::endl;
            logFile.close();
        }
    }

    static void log(const std::string& message) {
        if (logFile) {
            logFile << "[LOG]: " << message << std::endl;
        }
        //std::cout << "[LOG]: " << message << std::endl;
    }

    static void error(const std::string& message) {
        if (logFile) {
            logFile << "[ERROR]: " << message << std::endl;
        }
        //std::cerr << "[ERROR]: " << message << std::endl;
    }

    static std::string getTime()
    {
        time_t now = time(0);
        char* dt = ctime(&now);
        return dt;
    }
};

std::ofstream logger::logFile;

#endif //TINYLANGUAGEPARSER_LOGGER_H
