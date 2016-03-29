#ifndef TOOL_BAR_HPP
#define TOOL_BAR_HPP

#include <stream9/qt/mixin/tool_bar_view.hpp>
#include <stream9/qt/mixin/tool_bar_view/drag.hpp>
#include <stream9/qt/mixin/tool_bar_view/drop.hpp>

#include <QtCore/QtGlobal>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QToolBar>
#else
#include <QtWidgets/QToolBar>
#endif

using ToolBarBase = stream9::qt::mixin::ToolBarView<
                    stream9::qt::mixin::tool_bar_view::Drag<
                    stream9::qt::mixin::tool_bar_view::Drop<QToolBar>>>;

class ToolBar : public ToolBarBase
{
    Q_OBJECT
    using Base = ToolBarBase;
public:
    ToolBar(QWidget* const parent);

Q_SIGNALS:
    void actionTriggered(const QModelIndex&);

private:
    // @override mixin::ToolBarView
    QMenu &createSubMenu(QAbstractItemModel &,
                    const QModelIndex &, QWidget* const parent) override;
    void onActionTriggered(const QModelIndex&) override;
};

#endif // TOOL_BAR_HPP
