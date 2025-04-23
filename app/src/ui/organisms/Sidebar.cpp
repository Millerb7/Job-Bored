#include "Sidebar.h"
#include "../molecules/ControlButtons.h"
#include "../atoms/NavIcon.h"
#include <QVBoxLayout>

Sidebar::Sidebar(QWidget *parent)
    : QWidget(parent) {
    setStyleSheet("background-color: #ac90ce;");

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(10);

    auto *controls = new ControlButtons;
    connect(controls, &ControlButtons::minimizeClicked, parent, [parent]() {
        static_cast<QMainWindow*>(parent)->showMinimized();
    });
    connect(controls, &ControlButtons::closeClicked, parent, [parent]() {
        static_cast<QMainWindow*>(parent)->close();
    });
    connect(controls, &ControlButtons::toggleFullscreenClicked, parent, [parent]() {
        auto *win = static_cast<QMainWindow*>(parent);
        win->isFullScreen() ? win->showNormal() : win->showFullScreen();
    });

    layout->addWidget(controls);

    for (int i = 0; i < 5; ++i)
        layout->addWidget(new NavIcon("tab"));

    layout->addStretch();
}
