#pragma once
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <ctime>

class Logger {
public:
    static void log(const std::string& message) {

        std::cout << message << std::endl;

        std::ofstream logfile("server.log", std::ios::app);
        if (logfile.is_open()) {
            logfile << message << std::endl;
            logfile.close();
        }
    }

    static std::string timestamp() {
        auto now = std::chrono::system_clock::now();
        auto time_t_now = std::chrono::system_clock::to_time_t(now);
        char buf[64];
        ctime_s(buf, sizeof(buf), &time_t_now);
        std::string ts(buf);
        ts.pop_back();
        return ts;
    }
};