#ifndef CONTEXT_MENU_HPP
#define CONTEXT_MENU_HPP

#include <cassert>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QMenu>
#else
#include <QtWidgets/QMenu>
#endif

class QAction;
class QContextMenuEvent;
class QWidget;

namespace stream9 { namespace qt { namespace mixin { namespace widget {

template<typename Base>
class ContextMenu : public Base
{
public:
    ContextMenu(QWidget* const parent);

    QMenu &contextMenu() { return *m_menu; }
    void setContextMenu(QMenu &menu); // takes ownership of menu

protected:
    virtual bool hasContext(const QPoint &) { return true; }

protected:
    // @override QWidget
    void mousePressEvent(QMouseEvent* const) override;
    void keyPressEvent(QKeyEvent* const) override;

private:
    QMenu *m_menu;
};

}}}} // namespace stream9::qt::mixin::widget

#include <QtCore/QDebug>
#include <QtGui/QContextMenuEvent>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QAction>
#include <QtGui/QWidget>
#else
#include <QtWidgets/QAction>
#include <QtWidgets/QWidget>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace widget {

template<typename Base>
inline ContextMenu<Base>::
ContextMenu(QWidget* const parent)
    : Base { parent },
      m_menu { new QMenu { this } }
{}

template<typename Base>
inline void ContextMenu<Base>::
setContextMenu(QMenu &menu)
{
    static_cast<QObject&>(menu).setParent(this);
    m_menu->deleteLater();

    m_menu = &menu;
}

// By right mouse button QApplication send QContextMenuEvent
// to a widget or top-most popup if it is available.
// By menu key, QKeyManager send QContextMenuEvent to a widget
// which has keyboard focus at that moment.
// These are not desirable situation for sub menu items, because
// for sub menu items, sub menu IS top-most popup and focused
// widget insted of sub menu item.

// So, in order to support sub menu we don't rely on QContextMenuEvent,
// insted we handle MouseButtonPress event and KeyPress event directory.

template<typename Base>
inline void ContextMenu<Base>::
mousePressEvent(QMouseEvent* const event)
{
    //qDebug() << __PRETTY_FUNCTION__ << event;
    assert(event);
    Base::mousePressEvent(event);

    const auto &pos = event->pos();
    if (!this->rect().contains(pos)) return;
    if (!hasContext(pos)) return;

    if (event->button() == Qt::RightButton) {
        m_menu->exec(event->globalPos());
        event->accept();
    }
}

template<typename Base>
inline void ContextMenu<Base>::
keyPressEvent(QKeyEvent* const event)
{
    //qDebug() << __PRETTY_FUNCTION__ << event;
    assert(event);
    Base::keyPressEvent(event);

    const auto &pos = this->rect().center();
    if (!hasContext(pos)) return;

    if (event->key() == Qt::Key_Menu) {
        m_menu->exec(this->mapToGlobal(pos));
    }
}

}}}} // namespace stream9::qt::mixin::widget

#endif // CONTEXT_MENU_HPP
