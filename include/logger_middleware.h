#pragma once
#include "crow_all.h"
#include "logger.h"
#include <chrono>
#include <string>

struct LoggingMiddleware {
    struct context {
        std::chrono::steady_clock::time_point start_time;
        std::string url;
    };

    void before_handle(crow::request& req, crow::response& /*res*/, context& ctx) {
        ctx.start_time = std::chrono::steady_clock::now();
        ctx.url = req.url;
    }

    void after_handle(crow::request& req, crow::response& res, context& ctx) {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - ctx.start_time).count();

        std::string method_str;
        #ifdef CROW_ENABLE_LOGGING
        method_str = crow::method_name(req.method);
        #else
        method_str = std::to_string(static_cast<int>(req.method));
        #endif

        std::string log_msg = "[" + Logger::timestamp() + "] " +
                              method_str + " " +
                              ctx.url + " -> " +
                              std::to_string(res.code) +
                              " (" + std::to_string(elapsed) + " ms)";
        Logger::log(log_msg);
    }
};