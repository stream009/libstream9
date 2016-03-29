#ifndef MIXIN_WIDGET_DROP_H
#define MIXIN_WIDGET_DROP_H

class QDragEnterEvent;
class QDragLeaveEvent;
class QDragMoveEvent;
class QDropEvent;
class QMimeData;
class QMouseEvent;
class QPaintEvent;
class QPoint;
class QWidget;

namespace stream9 { namespace qt { namespace mixin { namespace widget {

template<typename Base>
class Drop : public Base
{
    enum State { None, Accept };
public:
    Drop(QWidget* const parent);

protected:
    // @override QWidget
    void dragEnterEvent(QDragEnterEvent* const) override;
    void dragMoveEvent(QDragMoveEvent* const) override;
    void dragLeaveEvent(QDragLeaveEvent* const) override;
    void dropEvent(QDropEvent* const) override;
    void mouseReleaseEvent(QMouseEvent* const) override;
    void paintEvent(QPaintEvent* const) override;

private:
    virtual bool canDrop(const QDragMoveEvent &) { return false; }
    virtual bool canDrop(const QMimeData &) { return false; }
    virtual void paintDropMarker(QPaintEvent &) {}
    virtual void onDrop(QDropEvent&) {}

private:
    State m_state;
};

}}}} // namespace stream9::qt::mixin::widget

#include <cassert>

#include <QtCore/QDebug>
#include <QtGui/QDragEnterEvent>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QPaintEvent>
#if(QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QWidget>
#else
#include <QtWidgets/QWidget>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace widget {

template<typename Base>
inline Drop<Base>::
Drop(QWidget* const parent)
    : Base(parent),
      m_state(Drop::None)
{
    this->setAcceptDrops(true);
}

template<typename Base>
inline void Drop<Base>::
dragEnterEvent(QDragEnterEvent* const event)
{
    assert(event);
    const QMimeData* const mimeData = event->mimeData();
    assert(mimeData);
    if (canDrop(*mimeData)) {
        event->accept();
    }
    else {
        qDebug() << "unsupported drag:" << event->mimeData()->formats();
    }
}

template<typename Base>
inline void Drop<Base>::
dragMoveEvent(QDragMoveEvent* const event)
{
    assert(event);
    if (canDrop(*event)) {
        m_state = Drop::Accept;
        event->accept();
    }
    else {
        m_state = Drop::None;
        event->ignore();
    }
    this->update();
}

template<typename Base>
inline void Drop<Base>::
dragLeaveEvent(QDragLeaveEvent* const)
{
    m_state = Drop::None;
    this->update();
}

template<typename Base>
inline void Drop<Base>::
dropEvent(QDropEvent* const event)
{
    assert(event);
    //event->acceptProposedAction();
    onDrop(*event);
    m_state = Drop::None;
}

template<typename Base>
inline void Drop<Base>::
mouseReleaseEvent(QMouseEvent* const event)
{
    Base::mouseReleaseEvent(event);
    m_state = Drop::None;
}

template<typename Base>
inline void Drop<Base>::
paintEvent(QPaintEvent* const event)
{
    assert(event);
    Base::paintEvent(event);

    if (m_state != Drop::Accept) return;

    paintDropMarker(*event);
}

}}}} // namespace stream9::qt::mixin::widget

#endif // MIXIN_WIDGET_DROP_H
