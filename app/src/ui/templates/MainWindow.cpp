#include "MainWindow.h"
#include <QMouseEvent>
#include <QCheckBox>
#include <QVBoxLayout>
#include <QFile>
#include <QWidget>
#include <QHBoxLayout>
#include <QApplication>
#include <QSettings>
#include <QPainterPath>
#include <QRegion> 
#include <QEvent>    
#include <QDebug>
#include "handlers/SimpleHandler.h"
#include "../organisms/Sidebar.h"
#include "../organisms/BottomBar.h"
#include "../organisms/dialogs/SettingsDialog.h"
#include "cef_browser.h"
#include "cef_app.h"
#include "cef_client.h"
#include <windows.h>

#ifndef GET_X_LPARAM
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#endif

#ifndef GET_Y_LPARAM
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))
#endif

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {
    qDebug() << "[MainWindow] Constructor entered";

    setWindowFlags(Qt::FramelessWindowHint);
    QSettings settings("JobBoredCompany", "JobBoredApp");
    QString savedTheme = settings.value("theme", "light").toString();
    applyTheme(savedTheme);
    setMinimumSize(1024, 700);

    qDebug() << "[MainWindow] Setting up UI";

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->setSpacing(0);

    auto *sidebar = new Sidebar(this);
    auto *bottomBar = new BottomBar(this);
    connect(bottomBar, &BottomBar::urlEntered, this, &MainWindow::handleUrlEntered);
    connect(bottomBar, &BottomBar::settingsClicked, this, &MainWindow::openSettings);
    connect(bottomBar, &BottomBar::themeToggleRequested, this, &MainWindow::toggleTheme);

    cefContainer = new QWidget;
    cefContainer->setAttribute(Qt::WA_NativeWindow);
    cefContainer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addWidget(sidebar);
    contentLayout->addWidget(cefContainer, 1);

    mainLayout->addLayout(contentLayout);
    mainLayout->addWidget(bottomBar);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    updateWindowMask();

    qDebug() << "[MainWindow] UI setup complete. Attempting to create CEF browser.";
    createCEFBrowser("https://www.google.com");
}

void MainWindow::handleUrlEntered() {
    qDebug() << "[MainWindow] URL entered!";
    QString url = urlBar->text();
    if (!url.startsWith("http")) url = "https://" + url;
    createCEFBrowser(url);
}

void MainWindow::createCEFBrowser(const QString &url) {
    qDebug() << "[MainWindow] createCEFBrowser called with URL:" << url;

    browserHandler = new SimpleHandler([this](CefRefPtr<CefBrowser> b) {
        qDebug() << "[MainWindow] Browser created!";
        this->browser = b;
    });

    CefBrowserSettings browser_settings;
    CefWindowInfo window_info;
    RECT rect;
    GetClientRect(HWND(cefContainer->winId()), &rect);

    HWND hwnd = HWND(cefContainer->winId());
    window_info.SetAsChild(hwnd, CefRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top));

    qDebug() << "[MainWindow] Launching CEF browser creation...";
    CefBrowserHost::CreateBrowser(window_info, browserHandler, url.toStdString(), browser_settings, nullptr, nullptr);
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

void MainWindow::updateWindowMask()
{
    if (isMaximized() || isFullScreen()) {
        setMask(QRegion()); // Full window, no rounded corners
    } else {
        const int radius = 12;
        QRegion region(rect(), QRegion::Rectangle);

        // Create a rounded rectangle path
        QPainterPath path;
        path.addRoundedRect(rect(), radius, radius);

        // Set the mask based on the path
        QRegion roundedRegion = QRegion(path.toFillPolygon().toPolygon());
        setMask(roundedRegion);
    }
}

void MainWindow::applyTheme(const QString &theme) {
    QFile file(":/styles/themes/" + theme + ".qss");
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        qApp->setStyleSheet(file.readAll());
        qDebug() << "[MainWindow] Applied theme:" << theme;
    } else {
        qDebug() << "[MainWindow] Failed to open theme file:" << theme;
    }
}

void MainWindow::openSettings() {
    SettingsDialog dialog(this);
    connect(&dialog, &SettingsDialog::themeChanged, this, &MainWindow::applyTheme);
    dialog.exec();
}

void MainWindow::toggleTheme() {
    QSettings settings("JobBoredCompany", "JobBoredApp");
    QString current = settings.value("theme", "light").toString();
    QString next = (current == "light") ? "dark" : "light";
    settings.setValue("theme", next);
    qDebug() << "[MainWindow] CEF Browser theme applying.";
    applyTheme(next);
}

void MainWindow::resizeEvent(QResizeEvent* event) {
    qDebug() << "[MainWindow] CEF Browser resize" << event;
    QMainWindow::resizeEvent(event);

    if (browser && browser->GetHost()) {
        HWND hwnd = browser->GetHost()->GetWindowHandle();
        if (hwnd) {
            RECT rect;
            GetClientRect(HWND(cefContainer->winId()), &rect);

            MoveWindow(
                hwnd,
                rect.left,
                rect.top,
                rect.right - rect.left,
                rect.bottom - rect.top,
                TRUE
            );

            browser->GetHost()->WasResized();
            updateWindowMask();
        }
    }
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, qintptr *result)
{
    qDebug() << message;

    if (eventType == "windows_generic_MSG") {
        MSG* msg = static_cast<MSG*>(message);

        if (msg->message == WM_NCHITTEST) {
            const LONG borderWidth = 8; // Must match your margins
            RECT winRect;
            GetWindowRect(HWND(winId()), &winRect);

            // Raw mouse position from message (screen coordinates)
            const LONG x = GET_X_LPARAM(msg->lParam);
            const LONG y = GET_Y_LPARAM(msg->lParam);

            // Calculate mouse position relative to window
            const LONG localX = x - winRect.left;
            const LONG localY = y - winRect.top;
            const LONG w = winRect.right - winRect.left;
            const LONG h = winRect.bottom - winRect.top;

            bool resizeWidth = minimumWidth() != maximumWidth();
            bool resizeHeight = minimumHeight() != maximumHeight();

            // Top-left corner
            if (resizeWidth && resizeHeight && localX < borderWidth && localY < borderWidth) {
                *result = HTTOPLEFT;
                return true;
            }
            // Top-right corner
            if (resizeWidth && resizeHeight && localX > w - borderWidth && localY < borderWidth) {
                *result = HTTOPRIGHT;
                return true;
            }
            // Bottom-left corner
            if (resizeWidth && resizeHeight && localX < borderWidth && localY > h - borderWidth) {
                *result = HTBOTTOMLEFT;
                return true;
            }
            // Bottom-right corner
            if (resizeWidth && resizeHeight && localX > w - borderWidth && localY > h - borderWidth) {
                *result = HTBOTTOMRIGHT;
                return true;
            }
            // Left border
            if (resizeWidth && localX < borderWidth) {
                *result = HTLEFT;
                return true;
            }
            // Right border
            if (resizeWidth && localX > w - borderWidth) {
                *result = HTRIGHT;
                return true;
            }
            // Top border
            if (resizeHeight && localY < borderWidth) {
                *result = HTTOP;
                return true;
            }
            // Bottom border
            if (resizeHeight && localY > h - borderWidth) {
                *result = HTBOTTOM;
                return true;
            }

            // Now handle dragging the window if you click somewhere in your fake "titlebar" area
            // (e.g., first 40px of height can be dragged)
            if (localY < 40) {
                *result = HTCAPTION;
                return true;
            }

            *result = HTCLIENT;
            return true;
        }
    }

    return QMainWindow::nativeEvent(eventType, message, result);
}
