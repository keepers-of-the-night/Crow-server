#pragma once

#include "crow_all.h"
#include <fstream>
#include <string>
#include <regex>

static std::string g_static_path = "static";

void setStaticPath(const std::string& path) {
    g_static_path = path;
}

crow::response make_error_response(int code, const std::string& message) {
    crow::json::wvalue resp;
    resp["error"] = true;
    resp["message"] = message;
    return crow::response(code, resp);
}

template <typename AppType>
void setupRoutes(AppType& app) {

    CROW_ROUTE(app, "/")([]() {
        crow::json::wvalue response;
        response["service"] = "Crow HTTP Server";
        response["version"] = "1.0";

        crow::json::wvalue::list endpoints;
        endpoints.push_back({{"method", "GET"}, {"path", "/"}});
        endpoints.push_back({{"method", "GET"}, {"path", "/hello/<string>"}});
        endpoints.push_back({{"method", "GET"}, {"path", "/json"}});
        endpoints.push_back({{"method", "POST"}, {"path", "/add"}});
        endpoints.push_back({{"method", "GET"}, {"path", "/files/<path>"}});

        response["endpoints"] = std::move(endpoints);
        return response;
    });

    CROW_ROUTE(app, "/hello/<string>")
    .name("hello")
    ([](const std::string& name) {
        if (name.empty()) {
            return make_error_response(400, "Name cannot be empty");
        }
        std::regex valid_pattern("^[a-zA-Zа-яА-Я0-9\\s\\-']+$");
        if (!std::regex_match(name, valid_pattern)) {
            return make_error_response(400, "Name contains invalid characters");
        }
        return crow::response("Hello, " + name + "!");
    });

    CROW_ROUTE(app, "/json")
    .name("json")
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
            return make_error_response(400, "Invalid JSON format");
        }

        if (!body.has("a") || !body.has("b")) {
            return make_error_response(400, "Missing fields: a and b are required");
        }

        int a = body["a"].i();
        int b = body["b"].i();
        int sum = a + b;

        crow::json::wvalue result;
        result["sum"] = sum;
        return crow::response(result);
    });

    CROW_ROUTE(app, "/files/<path>")
    .name("files")
    ([](const std::string& path) {
        if (path.find("..") != std::string::npos) {
            return make_error_response(403, "Forbidden");
        }

	std::string full_path = g_static_path + "/" + path;
        std::ifstream file(full_path, std::ios::binary);
        if (!file.is_open()) {
            return make_error_response(404, "File not found");
        }

        std::string ext = path.substr(path.find_last_of('.') + 1);
        std::string content_type;
        if (ext == "html") content_type = "text/html";
        else if (ext == "css") content_type = "text/css";
        else if (ext == "js") content_type = "application/javascript";
        else if (ext == "png") content_type = "image/png";
        else if (ext == "jpg" || ext == "jpeg") content_type = "image/jpeg";
        else if (ext == "svg") content_type = "image/svg+xml";
        else content_type = "application/octet-stream";

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

        crow::response res;
        res.set_header("Content-Type", content_type);
        res.code = 200;
        res.body = content;
        return res;
    });
}