#include "BottomBar.h"
#include "../molecules/UrlBar.h"
#include <QHBoxLayout>
#include <QPushButton>
#include <QMenu>
#include <QAction>

BottomBar::BottomBar(QWidget *parent)
    : QWidget(parent) {
    setStyleSheet("background-color: #f3eaff;");

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(8, 4, 8, 4);

    QPushButton *settingsBtn = new QPushButton("⚙️");
    QMenu *settingsMenu = new QMenu(this);
    settingsBtn->setMenu(settingsMenu);
    
    // Add "Toggle Dark Mode" option
    QAction *toggleTheme = new QAction("Toggle Dark Mode", this);
    settingsMenu->addAction(toggleTheme);
    
    // Emit signal upward (handled in MainWindow)
    connect(toggleTheme, &QAction::triggered, this, [this]() {
        emit themeToggleRequested();
    });
    
    layout->addWidget(settingsBtn);    
    connect(settingsBtn, &QPushButton::clicked, this, &BottomBar::settingsClicked);

    layout->addWidget(new QPushButton("⬅️"));
    layout->addWidget(new QPushButton("➡️"));
    layout->addWidget(new QPushButton(""));
    layout->addWidget(new QPushButton("⭐"));

    auto *urlBar = new UrlBar;
    connect(urlBar, &UrlBar::urlEntered, this, &BottomBar::urlEntered);

    layout->addWidget(urlBar, 1);
}
