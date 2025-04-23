#pragma once

#include <QWidget>

class BottomBar : public QWidget {
    Q_OBJECT
public:
    explicit BottomBar(QWidget *parent = nullptr);
signals:
    void urlEntered(const QString &url);

};
