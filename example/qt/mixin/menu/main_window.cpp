#include "main_window.hpp"

#include <cassert>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#else
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#endif

#include <stream9/qt/mixin/menu/drag.hpp>
#include <stream9/qt/mixin/menu/drop.hpp>

using Menu = stream9::qt::mixin::menu::Drag<
             stream9::qt::mixin::menu::Drop<QMenu>>;

MainWindow::
MainWindow(QWidget* const parent)
    : QMainWindow(parent)
{
    auto* const menuBar = new QMenuBar { this };
    assert(menuBar);
    this->setMenuBar(menuBar);

    auto* const menu = new Menu { this };
    assert(menu);
    menu->setTitle("Drag && Drop");

    for (const auto &title: { "first", "second", "third", "fourth" }) {
        auto* const action = new QAction { title, this };
        assert(action);
        menu->addAction(action);
    }

    auto* const subMenu = new Menu { this };
    assert(subMenu);
    subMenu->setTitle("Level 2");
    for (const auto &title: { "first", "second", "third", "fourth" }) {
        auto* const action = new QAction { title, this };
        assert(action);
        subMenu->addAction(action);
    }

    auto* const subSubMenu = new Menu { this };
    assert(subSubMenu);
    subSubMenu->setTitle("Level 3");
    for (const auto &title: { "first", "second", "third", "fourth" }) {
        auto* const action = new QAction { title, this };
        assert(action);
        subSubMenu->addAction(action);
    }

    subMenu->addMenu(subSubMenu);

    menu->addMenu(subMenu);

    menuBar->addMenu(menu);
}
