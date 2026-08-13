#include <iostream>
#include "handlers.h"

int main() {
    crow::SimpleApp app;

    setupRoutes(app);

    app.port(18080).multithreaded().run();
    return 0;
}