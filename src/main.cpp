#include <iostream>
#include "handlers.h"
#include "logger_middleware.h"

int main() {
    try {
        crow::App<LoggingMiddleware> app;

        setupRoutes(app);

        std::cout << "Server starting on http://localhost:18080" << std::endl;
        app.port(18080).multithreaded().run();

    } catch (const std::exception& e) {
        std::cerr << "ERROR: " << e.what() << std::endl;
        std::cin.get();
        return 1;
    } catch (...) {
        std::cerr << "Unknown error!" << std::endl;
        std::cin.get();
        return 1;
    }

    return 0;
}