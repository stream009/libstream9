#ifndef MENU_HPP
#define MENU_HPP

#include <stream9/qt/mixin/menu/tool_tip.hpp>
#include <stream9/qt/mixin/menu_view.hpp>
#include <stream9/qt/mixin/menu_view/drag.hpp>
#include <stream9/qt/mixin/menu_view/drop.hpp>
#include <stream9/qt/mixin/widget/context_menu.hpp>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QMenu>
#else
#include <QtWidgets/QMenu>
#endif

class QAbstractItemModel;
class QModelIndex;
class QWidget;

using MenuBase = stream9::qt::mixin::MenuView<
                 stream9::qt::mixin::menu_view::Drag<
                 stream9::qt::mixin::menu_view::Drop<
                 stream9::qt::mixin::menu::ToolTip<
                 stream9::qt::mixin::widget::ContextMenu<QMenu>>>>>;

class Menu : public MenuBase
{
    Q_OBJECT
    using Base = MenuBase;
public:
    Menu(QWidget* const);

Q_SIGNALS:
    void triggered(const QModelIndex&);
    void hovered(const QModelIndex&);

private:
    // @override mixin::widget::ContextMenu
    bool hasContext(const QPoint &) override;

private Q_SLOTS:
    void onContextMenuTriggered(QAction*);

private:
    // @override MenuView
    Menu &createSubMenu(QAbstractItemModel &,
                const QModelIndex &, QWidget* const parent) override;
    void onTriggered(const QModelIndex&) override;
    void onHovered(const QModelIndex&) override;
};

#endif // MENU_HPP
