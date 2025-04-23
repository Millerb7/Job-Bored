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
#include "../handlers/SimpleHandler.h"
#include "cef_browser.h"
#include "cef_app.h"
#include "cef_client.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    setWindowFlags(Qt::FramelessWindowHint);
    setMinimumSize(1024, 700);

    QWidget *central = new QWidget(this);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // Left sidebar
    QWidget *sidebar = new QWidget;
    sidebar->setStyleSheet("background-color: #ac90ce;");
    QVBoxLayout *sideLayout = new QVBoxLayout(sidebar);
    sideLayout->setContentsMargins(5, 5, 5, 5);
    sideLayout->setSpacing(10);

    // Top control buttons
    QPushButton *minBtn = new QPushButton("-");
    QPushButton *closeBtn = new QPushButton("x");
    QPushButton *maxBtn = new QPushButton("□");
    minBtn->setFixedSize(24, 24);
    closeBtn->setFixedSize(24, 24);
    maxBtn->setFixedSize(24, 24);
    connect(minBtn, &QPushButton::clicked, this, &QMainWindow::showMinimized);
    connect(closeBtn, &QPushButton::clicked, this, &QMainWindow::close);
    connect(maxBtn, &QPushButton::clicked, this, [this]() {
        isFullScreen() ? showNormal() : showFullScreen();
    });
    
    QString btnStyle = "QPushButton { background-color: #a178c5; color: white; border: none; }"
                   "QPushButton:hover { background-color: #8f5fbd; }";
    minBtn->setStyleSheet(btnStyle);
    maxBtn->setStyleSheet(btnStyle);
    closeBtn->setStyleSheet(btnStyle);

    QWidget *buttonRow = new QWidget;
    QHBoxLayout *buttonLayout = new QHBoxLayout(buttonRow);
    buttonLayout->setContentsMargins(0, 0, 0, 0);
    buttonLayout->setSpacing(4);
    buttonLayout->addWidget(closeBtn);
    buttonLayout->addWidget(minBtn);
    buttonLayout->addWidget(maxBtn);

    sideLayout->addWidget(buttonRow);

    // Example tabs
    for (int i = 0; i < 5; ++i) {
        QPushButton *tab = new QPushButton("tab");
        tab->setStyleSheet("background-color: #9c77bd; color: white;");
        sideLayout->addWidget(tab);
    }
    sideLayout->addStretch();

    // Web container
    cefContainer = new QWidget;
    cefContainer->setAttribute(Qt::WA_NativeWindow);
    cefContainer->setMinimumSize(800, 600);

    // Horizontal layout: sidebar + web content
    QHBoxLayout *contentLayout = new QHBoxLayout;
    contentLayout->setContentsMargins(0, 0, 0, 0);
    contentLayout->addWidget(sidebar);
    contentLayout->addWidget(cefContainer, 1);

    // Bottom bar
    QWidget *bottomBar = new QWidget;
    bottomBar->setStyleSheet("background-color: #f3eaff;");
    QHBoxLayout *bottomLayout = new QHBoxLayout(bottomBar);
    bottomLayout->setContentsMargins(8, 4, 8, 4);

    bottomLayout->addWidget(new QPushButton("⚙️"));
    bottomLayout->addWidget(new QPushButton("⬅️"));
    bottomLayout->addWidget(new QPushButton("➡️"));
    bottomLayout->addWidget(new QPushButton(""));
    bottomLayout->addWidget(new QPushButton("⭐"));

    QHBoxLayout *urlLayout = new QHBoxLayout;

    urlBar = new QLineEdit;
    urlBar->setPlaceholderText("https://google.com");
    urlLayout->addWidget(urlBar);
    QLabel *logo = new QLabel;
    logo->setPixmap(QPixmap(":/icons/searchy.png").scaled(18, 18, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    urlLayout->addWidget(logo);
    urlLayout->setStretch(1, 1); 

    bottomLayout->addLayout(urlLayout, 1);

    connect(urlBar, &QLineEdit::returnPressed, this, &MainWindow::handleUrlEntered);

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