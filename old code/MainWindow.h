#pragma once
#include <QMainWindow>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
private slots:
    void handleUrlEntered();
    void createNewTab(const QString &url);
private:
    QLineEdit* urlBar;
    QTabWidget* tabWidget;
};
