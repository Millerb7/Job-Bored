#pragma once

#include <QWidget>

class ControlButtons : public QWidget {
    Q_OBJECT
public:
    explicit ControlButtons(QWidget *parent = nullptr);
signals:
    void minimizeClicked();
    void closeClicked();
    void toggleFullscreenClicked();

};

