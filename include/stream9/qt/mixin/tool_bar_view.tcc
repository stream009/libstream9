#include "tool_bar_view.hpp"

#include <QtGui/QActionEvent>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QAction>
#include <QtGui/QMenu>
#else
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#endif

namespace stream9 { namespace qt { namespace mixin {

template<typename BaseT>
ToolBarView<BaseT>::
ToolBarView(QWidget* const parent)
    : Base { parent },
      m_slots { *this },
      m_eventCapturer { *this }
{
    this->connect(this,    SIGNAL(actionTriggered(QAction*)),
                  &m_slots,  SLOT(onActionTriggered(QAction*)));
}

template<typename BaseT>
void ToolBarView<BaseT>::
setModel(QAbstractItemModel &model,
         const QModelIndex &root/* = QModelIndex {}*/)
{
    Base::setModel(model, root);
    this->init();
}

template<typename BaseT>
void ToolBarView<BaseT>::
setRootIndex(const QModelIndex &index)
{
    Base::setRootIndex(index);
    this->refresh();
}

template<typename BaseT>
QAction &ToolBarView<BaseT>::
createItemAction(const QModelIndex &index)
{
    auto &action = Base::createItemAction(index);

    auto* const menu = action.menu();
    if (menu) {
        menu->installEventFilter(&m_eventCapturer);
    }

    return action;
}

template<typename BaseT>
void ToolBarView<BaseT>::
actionEvent(QActionEvent* const event)
{
    assert(event);
    Base::actionEvent(event);
    if (event->type() != QActionEvent::ActionAdded) return;

    auto* const action = event->action();
    assert(action);

    auto* button =
        dynamic_cast<QToolButton*>(this->widgetForAction(action));
    if (!button) return;

    button->installEventFilter(&m_middleButtonHandler);

    if (action->menu()) {
        button->setPopupMode(m_popupMode);
    }
    if (action->text().isEmpty()) {
        button->setToolButtonStyle(Qt::ToolButtonIconOnly);
    }
}

template<typename BaseT>
void ToolBarView<BaseT>::
onActionTriggered(QAction &action)
{
    const auto &index = this->index(action);
    if (!index.isValid()) return;

    onActionTriggered(index);
}

}}} // namespace stream9::qt::mixin
