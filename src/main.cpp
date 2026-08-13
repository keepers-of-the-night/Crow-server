#include <iostream>
#include "handlers.h"
#include "logger_middleware.h"
#include "config.h"

int main() {
    try {
    	Config config = Config::load("config.json");

    	std::cout << "Starting server on port " << config.port << std::endl;
    	std::cout << "Static files path: " << config.static_path << std::endl;

    	crow::App<LoggingMiddleware> app;

    	setStaticPath(config.static_path);

    	setupRoutes(app);

    	app.port(config.port).multithreaded().run();

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