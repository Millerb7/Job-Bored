#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include "ui/templates/MainWindow.h"
#include "handlers/SimpleApp.h"
#include "cef_app.h"
#include "cef_sandbox_win.h"

int main(int argc, char* argv[]) {
    // CEF setup
    CefMainArgs main_args(GetModuleHandle(nullptr));
    CefRefPtr<SimpleApp> app = new SimpleApp();

    // Subprocess check
    int exit_code = CefExecuteProcess(main_args, app, nullptr);
    if (exit_code >= 0) {
        qDebug() << "Subprocess ended, exiting main process.";
        return exit_code;
    }

    // Init CEF
    CefSettings settings;
    settings.multi_threaded_message_loop = true;
    CefInitialize(main_args, settings, app, nullptr);

    // Qt Init
    QApplication appQt(argc, argv);
    MainWindow w;
    w.show();

    qDebug() << "[Startup] Showing window.";
    int result = appQt.exec();
    qDebug() << "[Startup] Qt App exited with code" << result;


    CefShutdown(); // Called only after QApplication returns
    return result;
}
