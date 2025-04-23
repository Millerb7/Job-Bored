#include "SearchIcon.h"
#include <QPixmap>

SearchIcon::SearchIcon(QWidget *parent)
    : QLabel(parent) {
    setPixmap(QPixmap(":/icons/searchy.png").scaled(18, 18, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}
