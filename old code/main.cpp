#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _CRT_SECURE_NO_WARNINGS
#define CEF_USE_SANDBOX 0

#include <QApplication>
#include "MainWindow.h"
#include "include/cef_app.h"
#include <windows.h>

int main(int argc, char *argv[]) {
    // 1. Start Qt app first (argc/argv stay valid throughout)
    QApplication qtApp(argc, argv);

    // 2. CEF Setup
    CefMainArgs main_args(GetModuleHandle(nullptr));
    CefRefPtr<CefApp> app;

    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0)
        return exit_code;

    CefSettings settings;
    CefInitialize(main_args, settings, app, nullptr);

    // 3. Launch Qt window
    MainWindow window;
    window.show();

    // 4. Run main loop
    int result = qtApp.exec();

    // 5. Cleanup CEF
    CefShutdown();
    return result;
}
