#ifndef MIXIN_MENU_DRAG_H
#define MIXIN_MENU_DRAG_H

#include "../widget/drag.hpp"

#include <utility>

#include <boost/optional.hpp>

class QDrag;
class QDragLeaveEvent;
class QPoint;
class QWidget;

namespace stream9 { namespace qt { namespace mixin { namespace menu {

template<typename BaseT>
using DragBase = widget::Drag<BaseT>;

template<typename BaseT>
class Drag : public DragBase<BaseT>
{
    using Base = DragBase<BaseT>;
public:
    Drag(QWidget* const parent);

protected:
    // @override QWidget
    void dragLeaveEvent(QDragLeaveEvent* const) override;
    // @override Drag
    bool canDrag(const QPoint &) override;
    void prepareDrag(QDrag &, const QPoint &) override;
    void startDrag(const QPoint &) override;
    void onDragFinished(const QDrag &, const Qt::DropAction) override;

private:
    boost::optional<std::pair<QAction*, QAction*>> m_draggingAction;
};

}}}} // namespace stream9::qt::mixin::menu

#include "../mime_data.hpp"

#include <QtCore/QPoint>
#include <QtGui/QDrag>
#include <QtGui/QPixmap>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QWidget>
#else
#include <QtWidgets/QWidget>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace menu {

// QWidget::isAncestorOf can't be used. Since it requires widgets
// to be in a same window. Menu heirarchy doesn't satisfy that.
inline static bool
isDescendant(QWidget* const widget, QWidget* const ancestor)
{
    if (widget == nullptr) return false;
    auto* const parent = widget->parentWidget();
    if (parent == ancestor) {
        return true;
    }
    else {
        return isDescendant(parent, ancestor);
    }
}

inline static void
closeAncestors(QMenu* const menu)
{
    if (!menu) return;
    auto* const parent = dynamic_cast<QMenu*>(menu->parentWidget());
    if (parent) {
        parent->close();
        closeAncestors(parent);
    }
}

inline static void
closeDescendants(QMenu* const menu)
{
    if (!menu) return;
    const auto &subMenus = menu->template findChildren<QMenu*>();
    for (auto* const subMenu: subMenus) {
        subMenu->close();
        closeDescendants(subMenu);
    }
}

template<typename BaseT>
inline Drag<BaseT>::
Drag(QWidget* const parent)
    : Base { parent }
{}

template<typename BaseT>
inline void Drag<BaseT>::
dragLeaveEvent(QDragLeaveEvent* const event)
{
    auto* const widget = QApplication::widgetAt(QCursor::pos());
    if (widget &&
        (isDescendant(widget, this) || isDescendant(this, widget)))
    {
        return Base::dragLeaveEvent(event);
    }

    this->close();
    closeDescendants(this);
    closeAncestors(this);

    Base::dragLeaveEvent(event);
}

template<typename BaseT>
inline bool Drag<BaseT>::
canDrag(const QPoint &pos)
{
    auto* const target = this->actionAt(pos);
    return target;
}

template<typename BaseT>
inline void Drag<BaseT>::
prepareDrag(QDrag &drag, const QPoint &pos)
{
    auto* const action = this->actionAt(pos);
    assert(action);

    auto* const data = drag.mimeData();
    assert(data);
    setAction(*data, *action);

    data->setText(action->text());

    const auto &rect = this->actionGeometry(action);
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
    auto* const next =
            (index == actions.size()-1) ? nullptr : actions.at(index + 1);

    m_draggingAction = std::make_pair(action, next);

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

}}}} // namespace stream9::qt::mixin::menu

#endif // MIXIN_MENU_DRAG_H
