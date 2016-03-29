#ifndef MENU_VIEW_P_HPP
#define MENU_VIEW_P_HPP

class QAction;
class QModelIndex;

namespace stream9 { namespace qt { namespace mixin { namespace menu_view {

class Slots
{
public:
    // connect with QMenu
    virtual void onTriggered(QAction&) = 0;
    virtual void onHovered(QAction&) = 0;
    virtual void onAboutToShow() = 0;
};

}}}} // namespace stream9::qt::mixin::menu_view

#include <QtCore/QObject>

class QAction;
class QModelIndex;

namespace stream9 { namespace qt { namespace mixin { namespace menu_view {

class SlotsDelegate : public QObject
{
    Q_OBJECT
public:
    SlotsDelegate(Slots &host);

private Q_SLOTS:
    // connect with QMenu
    void onTriggered(QAction*);
    void onHovered(QAction*);
    void onAboutToShow();

private:
    Slots &m_host;
};

}}}} // namespace stream9::qt::mixin::menu_view

#endif // MENU_VIEW_P_HPP
