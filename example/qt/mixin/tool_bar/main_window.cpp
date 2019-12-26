#include "main_window.hpp"

#include <cassert>
#include <vector>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QCheckBox>
#include <QtGui/QLineEdit>
#include <QtGui/QMenu>
#include <QtGui/QPushButton>
#include <QtGui/QRadioButton>
#include <QtGui/QToolBar>
#else
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMenu>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#endif

#include <stream9/qt/mixin/menu/drag.hpp>
#include <stream9/qt/mixin/menu/drop.hpp>
#include <stream9/qt/mixin/tool_bar/drag.hpp>
#include <stream9/qt/mixin/tool_bar/drop.hpp>
#include <stream9/qt/mixin/widget/context_menu.hpp>

using StandardToolBar = QToolBar;

using DragNDropToolBar = stream9::qt::mixin::tool_bar::Drag<
                         stream9::qt::mixin::tool_bar::Drop<QToolBar>>;

using DragNDropMenu = stream9::qt::mixin::menu::Drag<
                      stream9::qt::mixin::menu::Drop<QMenu>>;

MainWindow::
MainWindow(QWidget* const parent)
    : QMainWindow(parent)
{
    createStandardToolBar();
    createDragNDropToolBar();
    createWidgetToolBar();
}

void MainWindow::
createStandardToolBar()
{
    auto* const toolBar = new StandardToolBar { "Standard", this };
    assert(toolBar);
    this->addToolBar(toolBar);

    for (const auto &title: { "first", "second", "third", "fourth" }) {
        auto* const action = new QAction { title, this };
        assert(action);
        toolBar->addAction(action);
    }
    this->addToolBarBreak();
}

void MainWindow::
createDragNDropToolBar()
{
    auto* const toolBar = new DragNDropToolBar { this };
    assert(toolBar);
    this->addToolBar(toolBar);

    for (const auto &title: { "first", "second", "third", "fourth" }) {
        auto* const action = new QAction { title, this };
        assert(action);
        toolBar->addAction(action);
    }
    this->addToolBarBreak();

    auto* const toolBar2 = new DragNDropToolBar { this };
    assert(toolBar2);
    this->addToolBar(toolBar2);

    for (const auto &title: { "ichi", "ni", "san", "yon" }) {
        auto* const action = new QAction { title, this };
        assert(action);
        toolBar2->addAction(action);
    }

    auto* const menu = new DragNDropMenu { this };
    assert(menu);
    menu->setTitle("&Menu");
    for (const auto &title: { "un", "deux", "trois", "quatre" }) {
        auto* const action = new QAction { title, this };
        assert(action);
        menu->addAction(action);
    }
    toolBar2->insertAction(toolBar2->actions().front(), menu->menuAction());
    auto* button = dynamic_cast<QToolButton*>(toolBar2->widgetForAction(menu->menuAction()));
    button->setPopupMode(QToolButton::InstantPopup);

    this->addToolBarBreak();
}

void MainWindow::
createWidgetToolBar()
{
    auto* const toolBar = new DragNDropToolBar { this };
    assert(toolBar);
    this->addToolBar(toolBar);

    const std::vector<QWidget*> &widgets {
        new QLineEdit { this },
        new QPushButton { "Button", this },
        new QCheckBox { "CheckBox", this },
        new QRadioButton { "RadioButton", this }
    };

    for (auto* const widget: widgets) {
        assert(widget);

        toolBar->addWidget(widget);
    }

    this->addToolBarBreak();
}
