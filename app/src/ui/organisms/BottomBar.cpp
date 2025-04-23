#include "BottomBar.h"
#include "../molecules/UrlBar.h"
#include <QHBoxLayout>
#include <QPushButton>

BottomBar::BottomBar(QWidget *parent)
    : QWidget(parent) {
    setStyleSheet("background-color: #f3eaff;");

    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(8, 4, 8, 4);

    layout->addWidget(new QPushButton("⚙️"));
    layout->addWidget(new QPushButton("⬅️"));
    layout->addWidget(new QPushButton("➡️"));
    layout->addWidget(new QPushButton(""));
    layout->addWidget(new QPushButton("⭐"));

    auto *urlBar = new UrlBar;
    connect(urlBar, &UrlBar::urlEntered, this, &BottomBar::urlEntered);

    layout->addWidget(urlBar, 1);
}
