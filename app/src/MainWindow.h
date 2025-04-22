#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QWidget>
#include <QPoint>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void handleUrlEntered();

private:
    void createCEFBrowser(const QString &url);

    QTabWidget *tabWidget;
    QLineEdit *urlBar;
    QWidget *cefContainer;

    bool isDragging;
    QPoint dragPosition;
};
