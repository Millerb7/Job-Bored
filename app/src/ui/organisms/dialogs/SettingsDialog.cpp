#include "SettingsDialog.h"
#include <QCheckBox>
#include <QVBoxLayout>
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent)
    : QDialog(parent) {
    setWindowTitle("Settings");
    setFixedSize(200, 100);

    darkModeToggle = new QCheckBox("Enable Dark Mode");
    
    // Load saved state
    QSettings settings("JobBoredCompany", "JobBoredApp");
    bool darkEnabled = settings.value("theme", "light") == "dark";
    darkModeToggle->setChecked(darkEnabled);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(darkModeToggle);
    setLayout(layout);

    connect(darkModeToggle, &QCheckBox::toggled, this, [this](bool checked) {
        QSettings settings("JobBoredCompany", "JobBoredApp");
        settings.setValue("theme", checked ? "dark" : "light");
        emit themeChanged(checked ? "dark" : "light");
    });
}
