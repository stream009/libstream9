#include "menu_view.hpp"

#include <cassert>

#include <QtCore/QDebug>
#include <QtCore/QModelIndex>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QMenu>
#include <QtGui/QWidget>
#else
#include <QtWidgets/QMenu>
#include <QtWidgets/QWidget>
#endif

namespace stream9 { namespace qt { namespace mixin {

template<typename BaseT>
inline MenuView<BaseT>::
MenuView(QWidget* const parent)
    : Base { parent },
      m_slots { *this }
{
    this->connect(this,     SIGNAL(triggered(QAction*)),
                  &m_slots,   SLOT(onTriggered(QAction*)));
    this->connect(this,     SIGNAL(hovered(QAction*)),
                  &m_slots,   SLOT(onHovered(QAction*)));
    this->connect(this,     SIGNAL(aboutToShow()),
                  &m_slots,   SLOT(onAboutToShow()));

    m_emptyAction.setEnabled(false);
    this->addAction(&m_emptyAction);
}

template<typename BaseT>
inline QAction *MenuView<BaseT>::
addMenu(QMenu* const menu)
{
    assert(menu);
    Base::addAction(menu->menuAction());

    return menu->menuAction();
}

template<typename BaseT>
inline QAction *MenuView<BaseT>::
insertMenu(QAction* const before, QMenu* const menu)
{
    assert(menu);
    Base::insertAction(before, menu->menuAction());

    return menu->menuAction();
}

template<typename BaseT>
inline QAction *MenuView<BaseT>::
addSeparator()
{
    const auto offset = this->itemsOffset();
    const auto &actions = this->actions();
    assert(actions.size() >= 0);
    const auto size = static_cast<size_t>(actions.size());
    auto* const before = offset == size ? nullptr : actions.at(offset);

    //qDebug() << __PRETTY_FUNCTION__ << action->text() << offset
    //         << before << (before ? before->text() : "null");

    return Base::insertSeparator(before);
}

template<typename BaseT>
inline QAction *MenuView<BaseT>::
insertSeparator(QAction* const before)
{
    if (!this->isItemAction(before)) {
        return Base::insertSeparator(before);
    }
    else {
        return addSeparator();
    }
}

template<typename BaseT>
inline bool MenuView<BaseT>::
isEmpty() const
{
    const auto &actions = this->actions();
    return (actions.size() == 1 && actions.at(0) == &m_emptyAction);
}

template<typename BaseT>
inline void MenuView<BaseT>::
actionEvent(QActionEvent* const action)
{
    assert(action);
    Base::actionEvent(action);

    const auto empty = (this->actions().size() == 1);
    m_emptyAction.setVisible(empty);
}

template<typename BaseT>
inline void MenuView<BaseT>::
onTriggered(QAction &action)
{
    const auto &index = this->index(action);
    if (!index.isValid()) return;

    onTriggered(index);
}

template<typename BaseT>
inline void MenuView<BaseT>::
onHovered(QAction &action)
{
    const auto &index = this->index(action);
    if (index.isValid()) {
        onHovered(index);
    }
}

template<typename BaseT>
inline void MenuView<BaseT>::
onAboutToShow()
{
    this->init();
}

}}} // namespace stream9::qt::mixin
