#include "menu.hpp"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QWidget>
#else
#include <QtWidgets/QWidget>
#endif

Menu::
Menu(QWidget* const parent)
    : Base { parent }
{
    auto &contextMenu = this->contextMenu();
    contextMenu.addAction(new QAction { tr("foo"), this });
    contextMenu.addAction(new QAction { tr("bar"), this });
    contextMenu.addAction(new QAction { tr("&Property"), this });

    this->connect(&contextMenu, SIGNAL(triggered(QAction*)),
                  this,           SLOT(onContextMenuTriggered(QAction*)));
}

bool Menu::
hasContext(const QPoint &)
{
    const auto &index = this->currentIndex();
    return index.isValid();
}

void Menu::
onContextMenuTriggered(QAction* const action)
{
    assert(action);

    const auto &index = this->currentIndex();
    if (!index.isValid()) return;

    const auto &itemText = this->model().data(index);

    qDebug() << "Action" << action->text() << "is triggered on item"
             << itemText.toString();
}

Menu &Menu::
createSubMenu(QAbstractItemModel &model,
                    const QModelIndex &root, QWidget* const parent)
{
    auto* const menu = new Menu { parent };
    assert(menu);
    menu->setModel(model, root);
    return *menu;
}

void Menu::
onTriggered(const QModelIndex &index)
{
    Q_EMIT triggered(index);
}

void Menu::
onHovered(const QModelIndex &index)
{
    Q_EMIT hovered(index);
}
