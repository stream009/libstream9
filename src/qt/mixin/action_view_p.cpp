#include "action_view_p.hpp"

#include <cassert>

#include <QtCore/QModelIndex>
#if(QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QAction>
#else
#include <QtWidgets/QAction>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace action_view {

SlotsDelegate::
SlotsDelegate(Slots &host)
    : m_host { host }
{}

void SlotsDelegate::
onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    m_host.onDataChanged(topLeft, bottomRight);
}

void SlotsDelegate::
onRowsInserted(
    const QModelIndex &parent, const int start, const int end)
{
    assert(start >= 0);
    assert(end >= 0);
    m_host.onRowsInserted(parent, start, end);
}

void SlotsDelegate::
onRowsRemoved(
    const QModelIndex &parent, const int start, const int end)
{
    assert(start >= 0);
    assert(end >= 0);
    m_host.onRowsRemoved(parent, start, end);
}

}}}} // namespace stream9::qt::mixin::action_view
