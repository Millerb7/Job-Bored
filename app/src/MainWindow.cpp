#include "MainWindow.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTabWidget>
#include <QLineEdit>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QMouseEvent>
#include <QFrame>
#include "simple_handler.h"
#include "cef_browser.h"
#include "cef_app.h"
#include "cef_client.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), isDragging(false) {
    // Frameless window
    setWindowFlags(Qt::FramelessWindowHint | Qt::Window);
    //setAttribute(Qt::WA_TranslucentBackground);

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Custom top bar
    QFrame *titleBar = new QFrame;
    titleBar->setObjectName("titleBar");
    titleBar->setStyleSheet("#titleBar { background-color: #2b2b2b; height: 30px; }");
    QHBoxLayout *titleLayout = new QHBoxLayout(titleBar);
    titleLayout->setContentsMargins(5, 0, 5, 0);

    QLabel *titleLabel = new QLabel("JobBored");
    titleLabel->setStyleSheet("color: white;");
    titleLayout->addWidget(titleLabel);
    titleLayout->addStretch();

    QPushButton *minBtn = new QPushButton("–");
    QPushButton *closeBtn = new QPushButton("x");
    minBtn->setFixedSize(20, 20);
    closeBtn->setFixedSize(20, 20);
    titleLayout->addWidget(minBtn);
    titleLayout->addWidget(closeBtn);

    connect(minBtn, &QPushButton::clicked, this, &QMainWindow::showMinimized);
    connect(closeBtn, &QPushButton::clicked, this, &QMainWindow::close);

    QSplitter *horizontalSplitter = new QSplitter(Qt::Horizontal);

    // Tabs on the left
    tabWidget = new QTabWidget(horizontalSplitter);
    tabWidget->setTabPosition(QTabWidget::West);
    tabWidget->setMinimumWidth(200);
    tabWidget->addTab(new QLabel("Tab 1"), "Google");

    // CEF container area
    cefContainer = new QWidget(horizontalSplitter);
    cefContainer->setAttribute(Qt::WA_NativeWindow);
    cefContainer->setMinimumSize(800, 600);

    horizontalSplitter->addWidget(tabWidget);
    horizontalSplitter->addWidget(cefContainer);
    horizontalSplitter->setStretchFactor(1, 1);

    // URL bar on the bottom
    urlBar = new QLineEdit();
    urlBar->setPlaceholderText("Enter a URL and press Enter");

    mainLayout->addWidget(titleBar);
    mainLayout->addWidget(horizontalSplitter);
    mainLayout->addWidget(urlBar);
    central->setLayout(mainLayout);
    setCentralWidget(central);

    connect(urlBar, &QLineEdit::returnPressed, this, &MainWindow::handleUrlEntered);

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