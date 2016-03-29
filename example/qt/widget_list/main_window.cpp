#include "main_window.hpp"

#include <cassert>

MainWindow::
MainWindow(QWidget* const parent)
    : QMainWindow(parent)
{
    this->resize(640, 480);

    auto* const list = new WidgetList { this };
    assert(list);
    this->setCentralWidget(list);
    list->setModel(&m_model);
}

MainWindow::~MainWindow() = default;


#include "item_widget.hpp"

WidgetList::
WidgetList(QWidget* const parent)
    : AbstractWidgetList { parent }
{}

QWidget &WidgetList::
createItemWidget(const QModelIndex &index)
{
    auto* const widget = new ItemWidget { *this, index };
    assert(widget);
    return *widget;
}
