#pragma once

#include "crow_all.h"
#include "jwt-cpp/jwt.h"
#include <string>

static const std::string JWT_SECRET = "your-very-secret-key";

struct JWTAuthMiddleware {
    struct context {};

    void before_handle(crow::request& req, crow::response& res, context& /*ctx*/) {

        if (req.url == "/login" || req.url == "/") {
            return;
        }

        auto auth_header = req.get_header_value("Authorization");
        if (auth_header.empty()) {
            res.code = 401;
            res.body = R"({"error":"Missing Authorization header"})";
            res.set_header("Content-Type", "application/json");
            res.end();
            return;
        }

        std::string prefix = "Bearer ";
        if (auth_header.compare(0, prefix.size(), prefix) != 0) {
            res.code = 401;
            res.body = R"({"error":"Invalid Authorization format, expected Bearer token"})";
            res.set_header("Content-Type", "application/json");
            res.end();
            return;
        }

        std::string token = auth_header.substr(prefix.size());
        try {
            auto decoded = jwt::decode(token);
            auto verifier = jwt::verify()
                .allow_algorithm(jwt::algorithm::hs256{JWT_SECRET})
                .with_issuer("my-crow-server");

            verifier.verify(decoded);
        } catch (const std::exception& e) {
            res.code = 401;
            res.body = R"({"error":"Invalid or expired token"})";
            res.set_header("Content-Type", "application/json");
            res.end();
        }
    }

    void after_handle(crow::request& /*req*/, crow::response& /*res*/, context& /*ctx*/) { }
};