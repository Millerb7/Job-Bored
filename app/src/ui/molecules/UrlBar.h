#pragma once

#include <QWidget>
#include <QLineEdit> 
#include <QString>   

class UrlBar : public QWidget {
    Q_OBJECT
public:
    explicit UrlBar(QWidget *parent = nullptr);
private:
    QLineEdit *input;
private slots:
    void onReturnPressed();
signals:
    void urlEntered(const QString &url);
};
