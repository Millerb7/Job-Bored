#include "MainWindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include <QDebug>
#include <windows.h>

#include "simple_handler.h"
#include "include/cef_app.h"
#include "include/cef_browser.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;

    urlBar = new QLineEdit;
    tabWidget = new QTabWidget;

    layout->addWidget(urlBar);
    layout->addWidget(tabWidget);

    connect(urlBar, &QLineEdit::returnPressed, this, &MainWindow::handleUrlEntered);

    central->setLayout(layout);
    setCentralWidget(central);

    createNewTab("https://www.google.com");
}

void MainWindow::handleUrlEntered() {
    QString url = urlBar->text();
    if (!url.startsWith("http"))
        url = "https://" + url;

    createNewTab(url);
}

void MainWindow::createNewTab(const QString &url) {
    QWidget *cefContainer = new QWidget;
    cefContainer->setAttribute(Qt::WA_NativeWindow);
    cefContainer->setMinimumSize(640, 480);
    tabWidget->addTab(cefContainer, url);

    // Embed CEF browser inside the QWidget
    HWND hwnd = (HWND)cefContainer->winId();
    CefWindowInfo window_info;

    RECT rect;
    GetClientRect(hwnd, &rect);
    CefRect cef_rect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
    window_info.SetAsChild(hwnd, cef_rect);

    CefBrowserSettings browser_settings;
    CefRefPtr<CefClient> client = new SimpleHandler();

    CefBrowserHost::CreateBrowser(
        window_info, client, url.toStdString(), browser_settings, nullptr, nullptr);
}

