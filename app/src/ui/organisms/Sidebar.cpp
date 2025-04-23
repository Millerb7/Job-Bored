#include "Sidebar.h"
#include "../molecules/ControlButtons.h"
#include "../atoms/NavIcon.h"

#include <QVBoxLayout>
#include <QMainWindow>

Sidebar::Sidebar(QWidget *parent)
    : QWidget(parent) {
    setStyleSheet("background-color: #ac90ce;");

    auto *layout = new QVBoxLayout(this);
    layout->setContentsMargins(5, 5, 5, 5);
    layout->setSpacing(10);

    auto *controls = new ControlButtons;

    // Safer parent cast and connection
    if (auto *win = qobject_cast<QMainWindow *>(parent)) {
        connect(controls, &ControlButtons::minimizeClicked, win, &QMainWindow::showMinimized);
        connect(controls, &ControlButtons::closeClicked, win, &QMainWindow::close);
        connect(controls, &ControlButtons::toggleFullscreenClicked, win, [win]() {
            win->isFullScreen() ? win->showNormal() : win->showFullScreen();
        });
    }

    layout->addWidget(controls);

    for (int i = 0; i < 5; ++i)
        layout->addWidget(new NavIcon("tab"));

    layout->addStretch();
}
