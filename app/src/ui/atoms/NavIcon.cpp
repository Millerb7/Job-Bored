#include "NavIcon.h"

NavIcon::NavIcon(const QString &label, QWidget *parent)
    : QPushButton(label, parent) {
    setStyleSheet("background-color: #9c77bd; color: white;");
}
