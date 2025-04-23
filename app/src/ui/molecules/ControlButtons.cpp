#include "ControlButtons.h"
#include "../atoms/IconButton.h"
#include <QHBoxLayout>

ControlButtons::ControlButtons(QWidget *parent)
    : QWidget(parent) {
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    auto *closeBtn = new IconButton("x");
    auto *minBtn = new IconButton("-");
    auto *maxBtn = new IconButton("□");

    connect(minBtn, &QPushButton::clicked, this, [=]() { emit minimizeClicked(); });
    connect(closeBtn, &QPushButton::clicked, this, [=]() { emit closeClicked(); });
    connect(maxBtn, &QPushButton::clicked, this, [=]() { emit toggleFullscreenClicked(); });    

    layout->addWidget(closeBtn);
    layout->addWidget(minBtn);
    layout->addWidget(maxBtn);
}
