#include "tool_bar.hpp"

#include "menu.hpp"

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QMenu>
#else
#include <QtWidgets/QMenu>
#endif

ToolBar::
ToolBar(QWidget* const parent)
    : Base { parent }
{}

QMenu &ToolBar::
createSubMenu(QAbstractItemModel &model,
              const QModelIndex &root, QWidget* const parent)
{
    auto* const menu = new Menu { parent };
    assert(menu);
    menu->setModel(model, root);
    return *menu;
}

void ToolBar::
onActionTriggered(const QModelIndex &index)
{
    //qDebug() << __PRETTY_FUNCTION__;
    Q_EMIT actionTriggered(index);
}
