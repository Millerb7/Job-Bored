#pragma once
#include <QString>
#include <QPushButton>

class IconButton : public QPushButton {
    Q_OBJECT
public:
    IconButton(const QString &text, QWidget *parent = nullptr);
};
