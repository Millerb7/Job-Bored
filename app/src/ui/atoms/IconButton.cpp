#include "IconButton.h"

IconButton::IconButton(const QString &text, QWidget *parent)
    : QPushButton(text, parent) {
    setFixedSize(24, 24);
    setStyleSheet(
        "QPushButton { background-color: #a178c5; color: white; border: none; }"
        "QPushButton:hover { background-color: #8f5fbd; }"
    );
}
