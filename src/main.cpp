#include <iostream>
#include "crow_all.h"

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "Hello, World!";
    });

    CROW_ROUTE(app, "/hello/<string>")
    ([](const std::string& name) {
        return "Hello, " + name + "!";
    });

    CROW_ROUTE(app, "/square/<int>")
    ([](int number) {
        int result = number * number;
        return "Square of " + std::to_string(number) + " is " + std::to_string(result);
    });

    app.port(18080).multithreaded().run();
    return 0;
}