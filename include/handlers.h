#pragma once

#include "crow_all.h"
#include <fstream>
#include <string>
#include <windows.h>

std::string getExecutablePath() {
    char buffer[MAX_PATH];
    GetModuleFileName(NULL, buffer, MAX_PATH);
    std::string path(buffer);
    return path.substr(0, path.find_last_of("\\/") + 1);
}

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

    CROW_ROUTE(app, "/files/<path>")
    ([](const std::string& path) {

        if (path.find("..") != std::string::npos) {
            return crow::response(403, "Forbidden");
        }

        std::string base_path = getExecutablePath() + "..\\..\\static\\";
	std::string full_path = base_path + path;
	std::cout << "Looking for file: " << full_path << std::endl;
        std::ifstream file(full_path, std::ios::binary);
        if (!file.is_open()) {
            return crow::response(404, "File not found");
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