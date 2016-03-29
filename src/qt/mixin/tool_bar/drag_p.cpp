#include "tool_bar/drag_p.hpp"

#include <cassert>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QEvent>
#include <QtGui/QMouseEvent>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QAction>
#include <QtGui/QWidget>
#else
#include <QtWidgets/QAction>
#include <QtWidgets/QWidget>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar {
                                                     namespace drag {

MouseEventCapturer::
MouseEventCapturer(QWidget &destination)
    : m_destination { destination }
{}

bool MouseEventCapturer::
eventFilter(QObject* const, QEvent* const event)
{
    assert(event);

    const auto type = event->type();
    if (type == QEvent::MouseMove || isLeftButtonPress(event)) {
        auto ev = translate(event);
        QCoreApplication::sendEvent(&m_destination, &ev);
    }

    return false;
}

bool MouseEventCapturer::
isLeftButtonPress(QEvent* const ev) const
{
    assert(ev);
    if (ev->type() == QEvent::MouseButtonPress) {
        auto* const event = dynamic_cast<QMouseEvent*>(ev);
        if (event && event->button() == Qt::LeftButton) {
            return true;
        }
    }
    return false;
}

QMouseEvent MouseEventCapturer::
translate(QEvent* const ev) const
{
    auto* const event = dynamic_cast<QMouseEvent*>(ev);
    assert(event);

    const auto &pos = m_destination.mapFromGlobal(event->globalPos());
    return QMouseEvent {
        event->type(), pos, event->globalPos(),
        event->button(), event->buttons(), event->modifiers()
    };
}

}}}}} // namespace stream9::qt::mixin::tool_bar::drag
