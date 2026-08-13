#include <iostream>
#include "handlers.h"
#include "logger_middleware.h"

int main() {
    crow::App<LoggingMiddleware> app;

    setupRoutes(app);

    app.port(18080).multithreaded().run();
    return 0;
}