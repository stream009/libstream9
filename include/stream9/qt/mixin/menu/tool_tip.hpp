#ifndef MIXIN_MENU_TOOL_TIP_HPP
#define MIXIN_MENU_TOOL_TIP_HPP

#include <QtCore/QString>

class QEvent;
class QWidget;

namespace stream9 { namespace qt { namespace mixin { namespace menu {

template<typename BaseT>
class ToolTip : public BaseT
{
    using Base = BaseT;
public:
    ToolTip(QWidget* const parent);

protected:
    // @override QObject
    bool event(QEvent*) override;
};

}}}} // namespace stream9::qt::mixin::menu

#include <cassert>

#include <QtCore/QEvent>
#include <QtCore/QDebug>
#include <QtGui/QHelpEvent>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QToolTip>
#include <QtGui/QWidget>
#else
#include <QtWidgets/QToolTip>
#include <QtWidgets/QWidget>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace menu {

template<typename BaseT>
ToolTip<BaseT>::
ToolTip(QWidget* const parent)
    : Base { parent }
{}

template<typename BaseT>
inline bool ToolTip<BaseT>::
event(QEvent* const ev)
{
    assert(ev);
    const auto result = Base::event(ev);

    [this, &ev] {
        if (ev->type() == QEvent::ToolTip) {
            auto* const event = dynamic_cast<QHelpEvent*>(ev);
            auto* const action = this->actionAt(event->pos());
            if (!action) return;

            const auto &toolTip = action->toolTip();
            if (toolTip.isEmpty()) return;
            QToolTip::showText(event->globalPos(), toolTip);
        }
    }();

    return result;
}

}}}} // namespace stream9::qt::mixin::menu

#endif // MIXIN_MENU_TOOL_TIP_HPP
