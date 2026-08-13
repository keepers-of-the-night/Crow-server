#pragma once
#include "crow_all.h"

template <typename AppType>
void setupRoutes(AppType& app) {
    CROW_ROUTE(app, "/")([]() {
        return "Hello, World!";
    });

    CROW_ROUTE(app, "/hello/<string>")
    ([](const std::string& name) {
        return "Hello, " + name + "!";
    });

    CROW_ROUTE(app, "/json")
    ([]() {
        crow::json::wvalue response;
        response["message"] = "Hello, JSON!";
        response["status"] = "success";
        return response;
    });

    CROW_ROUTE(app, "/add").methods("POST"_method)
    ([](const crow::request& req) {
        auto body = crow::json::load(req.body);
        if (!body) {
            return crow::response(400, "Invalid JSON");
        }
        int a = body["a"].i();
        int b = body["b"].i();
        int sum = a + b;

        crow::json::wvalue result;
        result["sum"] = sum;
        return crow::response(result);
    });
}