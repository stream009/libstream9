#ifndef MIXIN_MENU_DROP_H
#define MIXIN_MENU_DROP_H

#include "../widget/drop.hpp"

class QDragMoveEvent;
class QDropEvent;
class QMenu;
class QMimeData;
class QPaintEvent;
class QWidget;

namespace stream9 { namespace qt { namespace mixin { namespace menu {

template<typename BaseT>
using DropBase = widget::Drop<BaseT>;

template<typename BaseT>
class Drop : public DropBase<BaseT>
{
    using Base = DropBase<BaseT>;
public:
    Drop(QWidget* const parent);

protected:
    QAction *dropTarget(const QPoint &pos) const;

    // @override QWidget
    void dragMoveEvent(QDragMoveEvent* const) override;
    // @override widget::Drop
    bool canDrop(const QDragMoveEvent &) override;
    bool canDrop(const QMimeData &) override;
    void paintDropMarker(QPaintEvent &) override;
    void onDrop(QDropEvent&) override;

private:
    QMenu *m_activeSubMenu = nullptr;
};

}}}} // namespace stream9::qt::mixin::menu

#include "../mime_data.hpp"

#include <cassert>

#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QMenu>
#include <QtGui/QWidget>
#else
#include <QtWidgets/QMenu>
#include <QtWidgets/QWidget>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace menu {

template<typename BaseT>
inline Drop<BaseT>::
Drop(QWidget* const parent)
    : Base { parent }
{}

template<typename BaseT>
QAction *Drop<BaseT>::
dropTarget(const QPoint &pos) const
{
    auto* const action = this->actionAt(pos);
    if (!action) return nullptr;

    const auto &rect = this->actionGeometry(action);
    if (pos.y() < rect.topLeft().y() + rect.height() / 2) {
        return action;
    }
    else { // cursor is in bottom half => next action
        const auto &actions = this->actions();
        const auto &index = actions.indexOf(action);

        const auto &lastIndex = actions.size() - 1;
        if (index == lastIndex) {
            return nullptr;
        }
        else {
            return actions.at(index + 1);
        }
    }
    assert(false);
}

template<typename BaseT>
inline void Drop<BaseT>::
dragMoveEvent(QDragMoveEvent* const event)
{
    assert(event);
    Base::dragMoveEvent(event);

    auto* const target = this->actionAt(event->pos());
    if (!target) return;

    auto* const menu = target->menu();
    if (m_activeSubMenu && m_activeSubMenu != menu) {
        m_activeSubMenu->close();
        m_activeSubMenu = nullptr;
    }
    if (menu && menu != m_activeSubMenu) {
        const auto &rect = this->actionGeometry(target);
        const QPoint pos(rect.right(), rect.top());
        m_activeSubMenu = menu;
        menu->popup(this->mapToGlobal(pos));
    }
}

template<typename BaseT>
inline bool Drop<BaseT>::
canDrop(const QDragMoveEvent &event)
{
    QAction* const target = this->actionAt(event.pos());
    // No item
    if (!target) return false;

    auto* const data = event.mimeData();
    assert(data);
    auto* const source = mixin::getAction(*data);
    if (!source) return false;

    // Drop sub-menu into itself
    if (source->menu()) {
        if (target->parentWidget() == source->menu()) {
            return false;
        }
    }

    return true;
}

template<typename BaseT>
inline bool Drop<BaseT>::
canDrop(const QMimeData &mimeData)
{
    return mimeData.hasFormat("text/plain"); //TODO
}

template<typename BaseT>
inline void Drop<BaseT>::
paintDropMarker(QPaintEvent &)
{
    const auto &pos = this->mapFromGlobal(QCursor::pos());

    auto* action = dropTarget(pos); // nullptr == append to last

    QPainter p { this };
    p.setPen(this->palette().windowText().color());

    if (action) { // insert
        const auto &rect = this->actionGeometry(action);

        p.drawLine(rect.topLeft(), rect.topRight());
    }
    else { // append
        const auto &actions = this->actions();
        if (actions.empty()) {
            const auto &rect = this->geometry();
            p.drawLine(rect.topLeft(), rect.topRight());
        }
        else {
            const auto &rect = this->actionGeometry(actions.back());
            p.drawLine(rect.bottomLeft(), rect.bottomRight());
        }
    }
}

template<typename BaseT>
inline void Drop<BaseT>::
onDrop(QDropEvent &event)
{
    assert(event.dropAction() == Qt::MoveAction);

    auto* dropTo = dropTarget(event.pos()); // nullptr == append to last

    const auto* const data = event.mimeData();
    assert(data);

    auto* const action = mixin::getAction(*data);
    assert(action);
    assert(action != dropTo);

    if (action->menu()) {
        this->insertMenu(dropTo, action->menu());
    }
    else {
        this->insertAction(dropTo, action);
    }

    event.acceptProposedAction();
}

}}}} // namespace stream9::qt::mixin::menu

#endif // MIXIN_MENU_DROP_H
