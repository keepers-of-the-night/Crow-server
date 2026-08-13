#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include "crow_all.h"

struct Config {
    int port = 18080;
    std::string static_path = "static";
    std::string log_level = "info";

    static Config load(const std::string& filename = "config.json") {
        Config cfg;
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Warning: config.json not found, using defaults." << std::endl;
            return cfg;
        }

        std::stringstream buffer;
        buffer << file.rdbuf();
        auto json = crow::json::load(buffer.str());
        if (!json) {
            std::cerr << "Warning: invalid JSON in config.json, using defaults." << std::endl;
            return cfg;
        }

        if (json.has("port")) cfg.port = json["port"].i();
        if (json.has("static_path")) cfg.static_path = json["static_path"].s();
        if (json.has("log_level")) cfg.log_level = json["log_level"].s();

        return cfg;
    }
};