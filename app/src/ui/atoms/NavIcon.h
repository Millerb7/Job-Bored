#pragma once
#include <QPushButton>

class NavIcon : public QPushButton {
    Q_OBJECT
public:
    explicit NavIcon(const QString &label, QWidget *parent = nullptr);
};
