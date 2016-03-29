#ifndef MIXIN_TOOLBAR_DROP_HPP
#define MIXIN_TOOLBAR_DROP_HPP

#include "../widget/drop.hpp"

#include <QtCore/QPointer>

class QDragMoveEvent;
class QDropEvent;
class QMimeData;
class QPaintEvent;
class QWidget;

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar {

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

    // @override mixin::widget::Drop
    bool canDrop(const QMimeData &) override;
    bool canDrop(const QDragMoveEvent &) override;
    void paintDropMarker(QPaintEvent &event) override;
    void onDrop(QDropEvent&) override;

private:
    bool isFirstHalf(const QPoint&, const QRect&) const;

private:
    QPointer<QMenu> m_activeSubMenu = nullptr;
};

}}}} // namespace stream9::qt::mixin::tool_bar

#include "../mime_data.hpp"

#include <QtCore/QMimeData>
#include <QtCore/QPoint>
#include <QtCore/QRect>
#include <QtGui/QDropEvent>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#if(QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QMenu>
#include <QtGui/QWidget>
#else
#include <QtWidgets/QMenu>
#include <QtWidgets/QWidget>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar {

template<typename Base>
inline Drop<Base>::
Drop(QWidget* const parent)
    : Base(parent)
{}

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
        const QPoint pos(rect.left(), rect.bottom());
        m_activeSubMenu = menu;
        menu->popup(this->mapToGlobal(pos));
    }
}

template<typename BaseT>
bool Drop<BaseT>::
canDrop(const QMimeData &mimeData)
{
    return mimeData.hasFormat("text/plain");
}

template<typename BaseT>
bool Drop<BaseT>::
canDrop(const QDragMoveEvent &event)
{
    auto* const data = event.mimeData();
    assert(data);

    auto* const source = mixin::getAction(*data);

    return source != nullptr;
}

template<typename BaseT>
void Drop<BaseT>::
paintDropMarker(QPaintEvent &)
{
    const auto &pos = this->mapFromGlobal(QCursor::pos());

    auto* action = dropTarget(pos); // nullptr == append to last

    QPainter p { this };
    p.setPen(this->palette().foreground().color());

    if (action) { // insert
        auto* const button = this->widgetForAction(action);
        assert(button);
        const auto &rect = button->geometry();

        const auto &frontFrom = rect.topLeft();
        const auto &frontTo = this->orientation() == Qt::Horizontal
                                       ? rect.bottomLeft() : rect.topRight();

        p.drawLine(frontFrom, frontTo);
    }
    else { // append
        const auto &actions = this->actions();
        if (actions.empty()) { // to front
            const auto &rect = this->geometry();

            const auto &frontFrom = rect.topLeft();
            const auto &frontTo = this->orientation() == Qt::Horizontal
                                       ? rect.bottomLeft() : rect.topRight();
            p.drawLine(frontFrom, frontTo);
        }
        else { // to back
            auto* const button = this->widgetForAction(actions.back());
            assert(button);
            const auto &rect = button->geometry();

            const auto &backFrom = this->orientation() == Qt::Horizontal
                                       ? rect.topRight() : rect.bottomLeft();
            const auto &backTo = rect.bottomRight();

            p.drawLine(backFrom, backTo);
        }
    }
}

template<typename BaseT>
void Drop<BaseT>::
onDrop(QDropEvent &event)
{
    assert(event.dropAction() == Qt::MoveAction);

    auto* const dropTo = dropTarget(event.pos()); // nullptr => append

    const auto* const data = event.mimeData();
    assert(data);

    auto* const action = mixin::getAction(*data);
    assert(action);
    assert(action != dropTo);

    this->insertAction(dropTo, action);

    event.acceptProposedAction();
}

template<typename BaseT>
QAction *Drop<BaseT>::
dropTarget(const QPoint &pos) const
{
    auto* const action = this->actionAt(pos);
    if (!action) return nullptr;

    auto* const button = this->widgetForAction(action);
    assert(button);

    const auto &rect = button->geometry();
    if (isFirstHalf(pos, rect)) {
        return action;
    }
    else { // cursor is in right/bottom half => next action
        const auto &actions = this->actions();
        const auto &index = actions.indexOf(action);

        const auto &lastIndex = actions.size() - 1;
        if (index == lastIndex) { // next to last == nullptr
            return nullptr;
        }
        else {
            return actions.at(index + 1);
        }
    }
    assert(false);
}

template<typename BaseT>
bool Drop<BaseT>::
isFirstHalf(const QPoint &pos, const QRect &rect) const
{
    if (this->orientation() == Qt::Horizontal) {
        const auto &middleX = rect.topLeft().x() + rect.width() / 2;
        return pos.x() < middleX;
    }
    else {
        const auto &middleY = rect.topLeft().y() + rect.height() / 2;
        return pos.y() < middleY;
    }
}

}}}} // namespace stream9::qt::mixin::tool_bar

#endif // MIXIN_TOOLBAR_DROP_HPP
