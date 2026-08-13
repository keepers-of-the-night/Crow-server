#include <iostream>
#include "crow_all.h"   // или путь к вашему файлу

int main() {
    crow::SimpleApp app;

    CROW_ROUTE(app, "/")([]() {
        return "Hello, World!";
    });

    app.port(18080).multithreaded().run();

    return 0;
}