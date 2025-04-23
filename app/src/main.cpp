#include <QApplication>
#include "ui/templates/MainWindow.h"
#include "handlers/SimpleApp.h"
#include "cef_app.h"

int main(int argc, char *argv[]) {
    CefMainArgs main_args(GetModuleHandle(nullptr));
    CefRefPtr<SimpleApp> app(new SimpleApp);

    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0) return exit_code;

    CefSettings settings;
    settings.multi_threaded_message_loop = true;
    settings.log_severity = LOGSEVERITY_VERBOSE;
    CefString(&settings.log_file) = "debug.log";
    CefInitialize(main_args, settings, app, nullptr);

    QApplication qtApp(argc, argv);
    MainWindow window;
    window.show();

    int result = qtApp.exec();
    CefShutdown();
    return result;
}
