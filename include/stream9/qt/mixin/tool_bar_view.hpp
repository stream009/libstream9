#ifndef TOOL_BAR_VIEW_HPP
#define TOOL_BAR_VIEW_HPP

#include <QtCore/QtGlobal>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QToolBar>
#include <QtGui/QToolButton>
#else
#include <QtWidgets/QToolBar>
#include <QtWidgets/QToolButton>
#endif

#include "action_view.hpp"
#include "tool_bar_view_p.hpp"

class QAbstractItemModel;
class QAction;
class QModelIndex;
class QWidget;

namespace stream9 { namespace qt { namespace mixin {

template<typename BaseT>
using ToolBarViewBase = ActionView<BaseT>;

template<typename BaseT = QToolBar>
class ToolBarView : public ToolBarViewBase<BaseT>, public tool_bar_view::Slots
{
    using Base = ToolBarViewBase<BaseT>;
public:
    ToolBarView(QWidget* const parent);

    void setPopupMode(const QToolButton::ToolButtonPopupMode);

    //@overload mixin::ActionView
    void setModel(QAbstractItemModel &,
                  const QModelIndex &root = QModelIndex {});
    void setRootIndex(const QModelIndex &);

protected:
    virtual void onActionTriggered(const QModelIndex&) {}

    // @override mixin::ActionView
    QAction &createItemAction(const QModelIndex &) override;

    // @override QWidget
    void actionEvent(QActionEvent* const) override;

private:
    // @override tool_bar_view::Slots
    void onActionTriggered(QAction&) override;

private:
    tool_bar_view::SlotsDelegate m_slots;
    tool_bar_view::MouseMoveEventCapturer m_eventCapturer;
    tool_bar_view::ToolBarMiddleButtonHandler m_middleButtonHandler;
    QToolButton::ToolButtonPopupMode m_popupMode = QToolButton::InstantPopup;
};

}}} // namespace stream9::qt::mixin

#include "tool_bar_view.tcc"

#endif // TOOL_BAR_VIEW_HPP
