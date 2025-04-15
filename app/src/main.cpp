#include "include/cef_app.h"
#include "simple_app.h"
#include <windows.h>

int main(int argc, char* argv[]) {
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    CefMainArgs main_args(hInstance);
    CefRefPtr<SimpleApp> app(new SimpleApp());

    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0)
        return exit_code;

    CefSettings settings;
    settings.no_sandbox = true;
    settings.windowless_rendering_enabled = false;

    if (!CefInitialize(main_args, settings, app, nullptr))
        return 1;

    CefRunMessageLoop();
    CefShutdown();
    return 0;
}
