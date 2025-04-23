#include "MainWindow.h"                
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "handlers/SimpleHandler.h"     
#include "../organisms/Sidebar.h"
#include "../organisms/BottomBar.h"
#include "cef_browser.h"                 
#include "cef_app.h"
#include "cef_client.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    setWindowFlags(Qt::FramelessWindowHint);
    setMinimumSize(1024, 700);

    // Main container
    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Components
    auto *sidebar = new Sidebar(this);
    auto *bottomBar = new BottomBar(this);
    connect(bottomBar, &BottomBar::urlEntered, this, &MainWindow::handleUrlEntered);

    // CEF container
    cefContainer = new QWidget;
    cefContainer->setAttribute(Qt::WA_NativeWindow);
    cefContainer->setMinimumSize(800, 600);

    // Web content + sidebar layout
    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addWidget(sidebar);
    contentLayout->addWidget(cefContainer, 1);

    // Assemble
    mainLayout->addLayout(contentLayout);
    mainLayout->addWidget(bottomBar);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    createCEFBrowser("https://www.google.com");
}


void MainWindow::handleUrlEntered() {
    QString url = urlBar->text();
    if (!url.startsWith("http")) url = "https://" + url;
    createCEFBrowser(url);
}

void MainWindow::createCEFBrowser(const QString &url) {
    RECT rect;
    GetClientRect(HWND(cefContainer->winId()), &rect);
    CefWindowInfo window_info;
    window_info.SetAsChild(HWND(cefContainer->winId()),
                           CefRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top));

    CefBrowserSettings browser_settings;
    CefRefPtr<CefClient> client = new SimpleHandler();
    CefBrowserHost::CreateBrowser(window_info, client, url.toStdString(), browser_settings, nullptr, nullptr);
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - dragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event) {
    Q_UNUSED(event);
    isDragging = false;
}