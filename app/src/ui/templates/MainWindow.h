#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QWidget>
#include <QPoint>
#include <QCheckBox>
#include "include/cef_browser.h"
#include "include/cef_client.h"
#include "handlers/SimpleHandler.h" 

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void applyTheme(const QString &theme); 

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
    bool nativeEvent(const QByteArray &eventType, void *message, qintptr *result) override;
    void updateWindowMask();

private slots:
    void handleUrlEntered();
    void openSettings(); 
    void toggleTheme();

private:
    void createCEFBrowser(const QString &url);

    QTabWidget *tabWidget;
    QLineEdit *urlBar;
    QWidget *cefContainer;

    bool isDragging;
    QPoint dragPosition;

    CefRefPtr<CefBrowser> browser;
    CefRefPtr<SimpleHandler> browserHandler;
};
