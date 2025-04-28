#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
class QCheckBox;
QT_END_NAMESPACE

class SettingsDialog : public QDialog {
    Q_OBJECT

public:
    explicit SettingsDialog(QWidget *parent = nullptr);

signals:
    void themeChanged(const QString &theme);

private:
    QCheckBox *darkModeToggle;

};
