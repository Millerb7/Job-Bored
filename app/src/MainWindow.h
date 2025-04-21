#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QTabWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    void createCEFTab(const QString &url);

private slots:
    void handleUrlEntered();

private:
    QLineEdit *urlBar;
    QTabWidget *tabWidget;
};
