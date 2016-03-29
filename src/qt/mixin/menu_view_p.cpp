#include "menu_view_p.hpp"

#include <cassert>

#include <QtCore/QModelIndex>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QAction>
#else
#include <QtWidgets/QAction>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace menu_view {

SlotsDelegate::
SlotsDelegate(Slots &host)
    : m_host { host }
{}

void SlotsDelegate::
onTriggered(QAction* const action)
{
    assert(action);
    m_host.onTriggered(*action);
}

void SlotsDelegate::
onHovered(QAction* const action)
{
    assert(action);
    m_host.onHovered(*action);
}

void SlotsDelegate::
onAboutToShow()
{
    m_host.onAboutToShow();
}

}}}} // namespace stream9::qt::mixin::menu_view
