#ifndef MENU_VIEW_HPP
#define MENU_VIEW_HPP

#include "action_view.hpp"
#include "menu_view_p.hpp"

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QAction>
#else
#include <QtWidgets/QAction>
#endif

class QAbstractItemModel;
class QActionEvent;
class QMenu;
class QModelIndex;
class QWidget;

namespace stream9 { namespace qt { namespace mixin {

template<typename BaseT>
using MenuViewBase = ActionView<BaseT>;

// Base has to be a derivative of the QMenu
template<typename BaseT>
class MenuView : public MenuViewBase<BaseT>, public menu_view::Slots
{
    using Base = MenuViewBase<BaseT>;
public:
    MenuView(QWidget* const parent);

    //@overload QMenu
    QAction *addMenu(QMenu* const);
    QAction *insertMenu(QAction* const before, QMenu* const);
    QAction *addSeparator();
    QAction *insertSeparator(QAction* const before);
    bool isEmpty() const;

protected:
    virtual void onTriggered(const QModelIndex&) {}
    virtual void onHovered(const QModelIndex&) {}

protected:
    // @override QWidget
    void actionEvent(QActionEvent*) override;

private:
    // @override menu_view::Slots
    void onTriggered(QAction&) override;
    void onHovered(QAction&) override;
    void onAboutToShow() override;

private:
    menu_view::SlotsDelegate m_slots;
    QAction m_emptyAction { QObject::tr("Empty"), nullptr };
};

}}} // namespace stream9::qt::mixin

#include "menu_view.tcc"

#endif // MENU_VIEW_HPP
