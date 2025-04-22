#pragma once
#include <QMainWindow>
#include <QLineEdit>
#include <QTabWidget>
#include "simple_handler.h"     // For SimpleHandler
#include "include/cef_base.h"   // For CefRefPtr
#include <QList>                // For QList

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

    QList<CefRefPtr<SimpleHandler>> cefClients;

};
