#include "include/cef_app.h"
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "simple_handler.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::cout << "Starting CEF..." << std::endl;
    CefMainArgs main_args(GetModuleHandle(nullptr));
    CefRefPtr<SimpleApp> app(new SimpleApp());

    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0)
        return exit_code;

    CefSettings settings;
    settings.no_sandbox = true;

    std::cout << "Initializing CEF..." << std::endl;
    CefInitialize(main_args, settings, app, nullptr);

    std::cout << "Running message loop..." << std::endl;
    CefRunMessageLoop();

    CefShutdown();
    return 0;
}
