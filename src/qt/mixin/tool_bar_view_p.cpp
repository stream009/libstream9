#include "tool_bar_view_p.hpp"

#include <cassert>

#include <QtCore/QModelIndex>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QAction>
#else
#include <QtWidgets/QAction>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar_view {

SlotsDelegate::
SlotsDelegate(Slots &host)
    : m_host { host }
{}

void SlotsDelegate::
onActionTriggered(QAction* const action)
{
    assert(action);
    m_host.onActionTriggered(*action);
}

}}}} // namespace stream9::qt::mixin::tool_bar_view

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QWidget>
#else
#include <QtWidgets/QWidget>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar_view {

MouseMoveEventCapturer::
MouseMoveEventCapturer(QWidget &target)
    : m_target { target }
{}

bool MouseMoveEventCapturer::
eventFilter(QObject* const, QEvent* const event)
{
    if (event->type() == QEvent::MouseMove) {
        auto* const ev = dynamic_cast<QMouseEvent*>(event);
        assert(ev);
        const auto &pos = m_target.mapFromGlobal(ev->globalPos());
        const auto &rect = m_target.rect();
        if (rect.contains(pos)) {
            QMouseEvent newEv {
                ev->type(), pos, ev->globalPos(),
                ev->button(), ev->buttons(), ev->modifiers()
            };
            QCoreApplication::sendEvent(&m_target, &newEv);
        }
    }
    return false;
}

}}}} // namespace stream9::qt::mixin::tool_bar_view

#include <QtCore/QCoreApplication>
#include <QtCore/QEvent>
#include <QtCore/QObject>
#include <QtGui/QMouseEvent>

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar_view {

bool ToolBarMiddleButtonHandler::
eventFilter(QObject* const obj, QEvent* const ev)
{
    //qDebug() << __func__ << obj << ev;
    assert(obj);
    assert(ev);

    const auto type = ev->type();
    if (type != QEvent::MouseButtonPress &&
        type != QEvent::MouseButtonRelease)
    {
        return false;
    }

    auto* const event = dynamic_cast<QMouseEvent*>(ev);
    if (event->button() != Qt::MiddleButton) return false;

    // Translate middle click into left click
    QMouseEvent newEvent {
        event->type(), event->pos(), event->globalPos(),
        Qt::LeftButton, Qt::LeftButton, event->modifiers()
    };
    QCoreApplication::sendEvent(obj, &newEvent);

    return false;
}

}}}} // namespace stream9::qt::mixin::tool_bar_view
