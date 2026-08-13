#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <pathcch.h>

#include "handlers.h"
#include "logger_middleware.h"
#include "jwt_middleware.h"
#include "config.h"

std::string getExecutableDir() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(NULL, buffer, MAX_PATH);
    std::string path(buffer);
    return path.substr(0, path.find_last_of("\\/") + 1);
}

int main() {
    std::ofstream logfile("app_log.txt");
    if (logfile.is_open()) {
        logfile << "Program started" << std::endl;
        logfile.flush();
    }

    try {
        logfile << "Entering try" << std::endl;
        logfile.flush();

        std::string exe_dir = getExecutableDir();
        std::string dll_rel_path = exe_dir + "..\\..\\openssl\\bin";
        char fullPath[MAX_PATH];
        GetFullPathNameA(dll_rel_path.c_str(), MAX_PATH, fullPath, NULL);
        std::string dll_path(fullPath);
        logfile << "DLL path: " << dll_path << std::endl;
        logfile.flush();

        int size_needed = MultiByteToWideChar(CP_ACP, 0, dll_path.c_str(), -1, NULL, 0);
        std::wstring wdll_path(size_needed, 0);
        MultiByteToWideChar(CP_ACP, 0, dll_path.c_str(), -1, &wdll_path[0], size_needed);
        if (!wdll_path.empty() && wdll_path.back() == L'\0')
            wdll_path.pop_back();

        SetDllDirectoryW(wdll_path.c_str());
        logfile << "SetDllDirectoryW called" << std::endl;
        logfile.flush();

        HMODULE hLib = LoadLibraryA("libcrypto-3-x64.dll");
        if (hLib) {
            logfile << "Loaded libcrypto-3-x64.dll" << std::endl;
        } else {
            logfile << "Failed to load libcrypto-3-x64.dll, error: " << GetLastError() << std::endl;
        }
        logfile.flush();

        Config config = Config::load("config.json");
        logfile << "Config loaded, port: " << config.port << std::endl;
        logfile.flush();

        std::cout << "Starting server on port " << config.port << std::endl;
        std::cout << "Static files path: " << config.static_path << std::endl;

        setStaticPath(config.static_path);
        logfile << "Static path set" << std::endl;
        logfile.flush();

        logfile << "Creating crow::App..." << std::endl;
        logfile.flush();
        crow::App<LoggingMiddleware, JWTAuthMiddleware> app;
        logfile << "crow::App created" << std::endl;
        logfile.flush();

        logfile << "Calling setupRoutes..." << std::endl;
        logfile.flush();
        setupRoutes(app);
        logfile << "setupRoutes done" << std::endl;
        logfile.flush();

        logfile << "Starting app.run()..." << std::endl;
        logfile.flush();
        app.port(config.port).multithreaded().run();
        logfile << "app.run() finished (should not happen)" << std::endl;
        logfile.flush();

    } catch (const std::exception& e) {
        logfile << "EXCEPTION: " << e.what() << std::endl;
        std::cerr << "ERROR: " << e.what() << std::endl;
        logfile.flush();
        std::cin.get();
        return 1;
    } catch (...) {
        logfile << "UNKNOWN EXCEPTION" << std::endl;
        std::cerr << "Unknown error!" << std::endl;
        logfile.flush();
        std::cin.get();
        return 1;
    }

    logfile.close();
    return 0;
}