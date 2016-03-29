#ifndef MIXIN_ACTION_VIEW_DROP_HPP
#define MIXIN_ACTION_VIEW_DROP_HPP

class QAbstractItemModel;
class QDragMoveEvent;
class QDropEvent;
class QMimeData;
class QModelIndex;
class QWidget;

namespace stream9 { namespace qt { namespace mixin { namespace action_view {

template<typename BaseT>
class Drop : public BaseT
{
    using Base = BaseT;
public:
    Drop(QWidget* const parent);

protected:
    virtual QAbstractItemModel &model() const = 0;
    virtual const QModelIndex &rootIndex() const = 0;
    virtual QModelIndex index(QAction&) const = 0;

protected:
    // @override mixin::widget::Drop
    bool canDrop(const QDragMoveEvent &) override;
    bool canDrop(const QMimeData &) override;
    void onDrop(QDropEvent &) override;
};

}}}} // namespace stream9::qt::mixin:action_view

#include <QtCore/QAbstractItemModel>
#include <QtCore/QMimeData>
#include <QtCore/QModelIndex>
#include <QtGui/QDragMoveEvent>
#include <QtGui/QDropEvent>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QWidget>
#else
#include <QtWidgets/QWidget>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace action_view {

template<typename BaseT>
inline Drop<BaseT>::
Drop(QWidget* const parent)
    : Base { parent }
{}

template<typename BaseT>
inline bool Drop<BaseT>::
canDrop(const QDragMoveEvent &event)
{
    if (!Base::canDrop(event)) return false;

    auto* const action = this->actionAt(event.pos());
    if (!action) return false;

    const auto &index = this->index(*action);
    if (!index.isValid()) return false;

    return true;
}

template<typename BaseT>
inline bool Drop<BaseT>::
canDrop(const QMimeData &mimeData)
{
    const auto &mimeTypes = this->model().mimeTypes();
    return std::any_of(mimeTypes.begin(), mimeTypes.end(),
        [&mimeData] (const QString &mimeType) {
            return mimeData.hasFormat(mimeType);
        }
    );
}

template<typename BaseT>
inline void Drop<BaseT>::
onDrop(QDropEvent &event)
{
    auto* const dropTo = this->dropTarget(event.pos()); // nullptr => append
    auto* const mimeData = event.mimeData();
    assert(mimeData);
    auto* const action = mixin::getAction(*mimeData);
    assert(action);

    if (dropTo == action) return;

    int row = -1, column = -1;
    if (dropTo) {
        const auto &index = this->index(*dropTo);
        assert(index.isValid());
        assert(index.model() == &this->model());
        assert(index.parent() == this->rootIndex());

        row = index.row();
        column = index.column();
    }

    const auto success =
        this->model().dropMimeData(
            mimeData, event.dropAction(), row, column, this->rootIndex());
    if (!success) return;

    event.acceptProposedAction();
}

}}}} // namespace stream9::qt::mixin:action_view

#endif // MIXIN_ACTION_VIEW_DROP_HPP
