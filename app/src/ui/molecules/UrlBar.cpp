#include "UrlBar.h"
#include "../atoms/Icon.h"
#include <QHBoxLayout>

UrlBar::UrlBar(QWidget *parent)
    : QWidget(parent) {
    auto *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);

    input = new QLineEdit;
    input->setPlaceholderText("https://google.com");

    auto *icon = new SearchIcon;

    layout->addWidget(icon);
    layout->addWidget(input);
    layout->setStretch(1, 1);

    connect(input, &QLineEdit::returnPressed, this, &UrlBar::onReturnPressed);
}

void UrlBar::onReturnPressed() {
    emit urlEntered(input->text());
}
