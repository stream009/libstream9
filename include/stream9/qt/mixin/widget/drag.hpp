#ifndef MIXIN_WIDGET_DRAG_HPP
#define MIXIN_WIDGET_DRAG_HPP

#include <QtCore/QPoint>

class QDrag;
class QMouseEvent;
class QPoint;
class QWidget;

namespace stream9 { namespace qt { namespace mixin { namespace widget {

template<typename Base>
class Drag : public Base
{
    enum class State { None, Start, InProgress };
public:
    Drag(QWidget* const parent);

protected:
    // @override QWidget
    void mousePressEvent(QMouseEvent* const) override;
    void mouseReleaseEvent(QMouseEvent* const) override;
    void mouseMoveEvent(QMouseEvent* const) override;

private:
    virtual bool canDrag(const QPoint &) { return true; } //TODO make it const
    virtual bool canStartDrag(const QPoint &from, const QPoint &to) const;
    virtual void prepareDrag(QDrag &, const QPoint &) = 0;
    virtual void startDrag(const QPoint &) {}
    virtual void onDragFinished(const QDrag &, const Qt::DropAction) {}

private:
    State m_state = State::None;
    QPoint m_dragStartPosition;
};

}}}} // namespace stream9::qt::mixin::widget

#include <cassert>

#include <QtCore/QDebug>
#include <QtCore/QMimeData>
#include <QtGui/QDrag>
#include <QtGui/QMouseEvent>
#if(QT_VERSION >= QT_VERSION_CHECK(5, 0, 0))
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>
#else
#include <QtGui/QApplication>
#include <QtGui/QWidget>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace widget {

template<typename Base>
inline Drag<Base>::
Drag(QWidget* const parent)
    : Base { parent }
{}

template<typename Base>
inline void Drag<Base>::
mousePressEvent(QMouseEvent* const event)
{
    const auto &pos = event->pos();
    if (event->button() == Qt::LeftButton && canDrag(pos)) {
        m_dragStartPosition = pos;
        m_state = State::Start;
    }

    Base::mousePressEvent(event);
}

template<typename Base>
inline void Drag<Base>::
mouseReleaseEvent(QMouseEvent* const event)
{
    Base::mouseReleaseEvent(event);
    m_state = State::None;
}

template<typename Base>
inline bool Drag<Base>::
canStartDrag(const QPoint &from, const QPoint &to) const
{
    const auto distance =
                (to - from).manhattanLength();
    return distance > QApplication::startDragDistance();
}

template<typename Base>
inline void Drag<Base>::
mouseMoveEvent(QMouseEvent* const event)
{
    Base::mouseMoveEvent(event);

    if (m_state != State::Start) return;
    if (event->buttons() != Qt::LeftButton) return;

    const auto &pos = event->pos();
    if (!canDrag(pos)) return;
    if (!canStartDrag(m_dragStartPosition, pos)) return;

    // Qt will take take of the deletion of drag and data.
    auto* const drag = new QDrag { this };
    assert(drag);
    QMimeData *data = new QMimeData;
    assert(data);
    drag->setMimeData(data);

    prepareDrag(*drag, pos);

    m_state = State::InProgress;
    startDrag(pos);
    const auto dropAction = drag->exec();
    m_state = State::None;

    onDragFinished(*drag, dropAction);

    this->update(); // clean up widget
}

}}}} // namespace stream9::qt::mixin::widget

#endif // MIXIN_DRAG_HPP
