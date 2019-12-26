#ifndef MIXIN_TOOLBAR_DRAG_HPP
#define MIXIN_TOOLBAR_DRAG_HPP

#include "../widget/drag.hpp"
#include "drag_p.hpp"

#include <utility>

#include <boost/optional.hpp>

class QActionEvent;
class QDrag;
class QEvent;
class QPoint;
class QWidget;

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar {

template<typename BaseT>
using DragBase = widget::Drag<BaseT>;

template<typename BaseT>
class Drag : public DragBase<BaseT>
{
    using Base = DragBase<BaseT>;
public:
    Drag(QWidget* const parent);

protected:
    // @override mixin::widget::Drag
    bool canDrag(const QPoint &) override;
    bool canStartDrag(const QPoint &from, const QPoint &to) const override;
    void prepareDrag(QDrag &, const QPoint &) override;
    void startDrag(const QPoint &) override;
    void onDragFinished(const QDrag &, const Qt::DropAction) override;

    // @override QWidget
    void actionEvent(QActionEvent* const) override;

private:
    tool_bar::drag::MouseEventCapturer m_mouseEventCapturer;
    boost::optional<std::pair<QAction*, QAction*>> m_draggingAction;
};

}}}} // namespace stream9::qt::mixin::tool_bar

#include "../mime_data.hpp"

#include <QtCore/QPoint>
#include <QtGui/QDrag>
#include <QtGui/QPixmap>
#if(QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QAbstractButton>
#include <QtGui/QWidget>
#else
#include <QtWidgets/QAbstractButton>
#include <QtWidgets/QWidget>
#endif
#include <QMenu>

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar {

template<typename BaseT>
inline Drag<BaseT>::
Drag(QWidget* const parent/*= 0*/)
    : Base { parent },
      m_mouseEventCapturer { *this }
{}

template<typename BaseT>
inline bool Drag<BaseT>::
canDrag(const QPoint &pos)
{
    auto* const action = this->actionAt(pos);
    return action;
}

template<typename Base>
inline bool Drag<Base>::
canStartDrag(const QPoint &from, const QPoint &to) const
{
    auto* const action = this->actionAt(to);
    if (action && action->menu()) {
        if (from.y() < to.y()) return false;
    }

    const auto distance = (to - from).manhattanLength();
    return distance > QApplication::startDragDistance();
}

template<typename BaseT>
inline void Drag<BaseT>::
prepareDrag(QDrag &drag, const QPoint &pos)
{
    auto* const action = this->actionAt(pos);
    if (!action) return; // Sometime it happens. ex. edge of the menu.

    auto* const widget = this->widgetForAction(action);
    assert(widget);
    auto* const button = dynamic_cast<QAbstractButton*>(widget);
    if (button) {
        button->setDown(false);
    }

    auto* const data = drag.mimeData();
    assert(data);
    mixin::setAction(*data, *action);

    data->setText(action->text());

    const auto &rect = widget->geometry();
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    drag.setPixmap(QPixmap::grabWidget(this, rect));
#else
    drag.setPixmap(this->grab(rect));
#endif
}

template<typename BaseT>
inline void Drag<BaseT>::
startDrag(const QPoint &pos)
{
    assert(!m_draggingAction);

    auto* const action = this->actionAt(pos);
    assert(action);

    const auto &actions = this->actions();
    const auto index = actions.indexOf(action);
    assert(index != -1);
    auto* const next = (index == actions.size()-1)
                                    ? nullptr : actions.at(index + 1);

    m_draggingAction = std::make_pair(action, next);

    auto* menu = action->menu();
    if (menu) {
        menu->close();
    }

    this->removeAction(action);
}

template<typename BaseT>
inline void Drag<BaseT>::
onDragFinished(const QDrag&, const Qt::DropAction dropAction)
{
    if (dropAction == Qt::IgnoreAction) {
        assert(m_draggingAction);

        auto* const action = m_draggingAction->first;
        assert(action);
        auto* const next = m_draggingAction->second;

        this->insertAction(next, action);
    }

    m_draggingAction = boost::none;
}

template<typename BaseT>
inline void Drag<BaseT>::
actionEvent(QActionEvent* const event)
{
    assert(event);
    Base::actionEvent(event);

    if (event->type() != QEvent::ActionAdded) return;

    auto* const action = event->action();
    assert(action);

    if (action->menu()) {
        action->menu()->installEventFilter(&m_mouseEventCapturer);
    }

    auto* const widget = this->widgetForAction(action);
    assert(widget);

    widget->installEventFilter(&m_mouseEventCapturer);
}

}}}} // namespace stream9::qt::mixin::tool_bar

#endif // MIXIN_TOOLBAR_DRAG_HPP
