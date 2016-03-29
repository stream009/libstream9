#ifndef ACTION_VIEW_P_HPP
#define ACTION_VIEW_P_HPP

#include <cstddef>

class QAction;
class QModelIndex;

namespace stream9 { namespace qt { namespace mixin { namespace action_view {

class Slots
{
public:
    // connect with QAbstractItemModel
    virtual void onDataChanged(const QModelIndex&, const QModelIndex&) = 0;
    virtual void onRowsInserted(
        const QModelIndex &parent, const size_t start, const size_t end) = 0;
    virtual void onRowsRemoved(
        const QModelIndex &parent, const size_t start, const size_t end) = 0;
};

}}}} // namespace stream9::qt::mixin::action_view

#include <QtCore/QObject>

class QAction;
class QModelIndex;

namespace stream9 { namespace qt { namespace mixin { namespace action_view {

class SlotsDelegate : public QObject
{
    Q_OBJECT
public:
    SlotsDelegate(Slots &host);

private Q_SLOTS:
    // connect with QAbstractItemModel
    void onDataChanged(const QModelIndex&, const QModelIndex&);
    void onRowsInserted(
        const QModelIndex &parent, const int start, const int end);
    void onRowsRemoved(
        const QModelIndex &parent, const int start, const int end);

private:
    Slots &m_host;
};

}}}} // namespace stream9::qt::mixin::action_view

#endif // ACTION_VIEW_P_HPP
