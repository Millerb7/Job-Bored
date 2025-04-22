#include "MainWindow.h"
#include <QVBoxLayout>
#include <QWidget>
#include "cef_browser.h"
#include "cef_app.h"
#include "cef_client.h"
#include "simple_handler.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    QWidget *central = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout;

    urlBar = new QLineEdit;
    tabWidget = new QTabWidget;

    layout->addWidget(urlBar);
    layout->addWidget(tabWidget);
    central->setLayout(layout);
    setCentralWidget(central);

    connect(urlBar, &QLineEdit::returnPressed, this, &MainWindow::handleUrlEntered);

    createCEFTab("https://www.google.com");
}

void MainWindow::handleUrlEntered() {
    QString url = urlBar->text();
    if (!url.startsWith("http")) url = "https://" + url;
    createCEFTab(url);
}

void MainWindow::createCEFTab(const QString &url) {
    QWidget *cefContainer = new QWidget;
    cefContainer->setAttribute(Qt::WA_NativeWindow);
    cefContainer->setMinimumSize(800, 600);
    tabWidget->addTab(cefContainer, url);

    // Delay browser creation until after event loop processes native window
    QTimer::singleShot(1000, [=]() {
        RECT rect;
        GetClientRect(HWND(cefContainer->winId()), &rect);

        CefWindowInfo window_info;
        window_info.SetAsChild(HWND(cefContainer->winId()),
                               CefRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top));

        CefBrowserSettings browser_settings;
        CefRefPtr<SimpleHandler> handler = new SimpleHandler();
        cefClients.append(handler); 
        CefBrowserHost::CreateBrowser(window_info, handler, url.toStdString(), browser_settings, nullptr, nullptr);        
    });

    this->setWindowTitle("QT IS WORKING!");
}
