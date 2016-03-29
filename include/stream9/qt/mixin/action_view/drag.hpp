#ifndef MIXIN_ACTION_VIEW_DRAG_HPP
#define MIXIN_ACTION_VIEW_DRAG_HPP

class QDrag;
class QPoint;
class QWidget;

namespace stream9 { namespace qt { namespace mixin { namespace action_view {

template<typename BaseT>
class Drag : public BaseT
{
    using Base = BaseT;
public:
    Drag(QWidget* const parent);

    using Base::actionAt;

protected:
    virtual QAbstractItemModel &model() const = 0;
    virtual const QModelIndex &rootIndex() const = 0;
    virtual QModelIndex index(QAction&) const = 0;

protected:
    // @override mixin::widget::Drag
    bool canDrag(const QPoint &) override;
    void startDrag(const QPoint &) override;
    void onDragFinished(const QDrag &, const Qt::DropAction) override;
    void prepareDrag(QDrag &, const QPoint &) override;

private:
    QModelIndex m_originalIndex;
};

}}}} // namespace stream9::qt::mixin::action_view

#include "../mime_data.hpp"

#include <QtCore/QPoint>
#include <QtGui/QDrag>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QWidget>
#else
#include <QtWidgets/QWidget>
#endif

namespace stream9 { namespace qt { namespace mixin { namespace action_view {

template<typename BaseT>
inline Drag<BaseT>::
Drag(QWidget* const parent)
    : Base { parent }
{}

template<typename BaseT>
inline bool Drag<BaseT>::
canDrag(const QPoint &pos)
{
    //qDebug() << __func__;
    if (!Base::canDrag(pos)) return false;

    auto* const target = this->actionAt(pos);
    assert(target);
    return this->index(*target).isValid();
}

template<typename BaseT>
inline void Drag<BaseT>::
startDrag(const QPoint &pos)
{
    assert(!m_originalIndex.isValid());
    auto* const action = this->actionAt(pos);
    assert(action);
    const auto &index = this->index(*action);
    assert(index.isValid());

    m_originalIndex = index; // copy

    this->model().removeRow(index.row(), index.parent());
}

template<typename BaseT>
inline void Drag<BaseT>::
onDragFinished(const QDrag &drag, const Qt::DropAction dropAction)
{
    if (dropAction == Qt::IgnoreAction) {
        assert(m_originalIndex.isValid());

        auto* const mimeData = drag.mimeData();
        assert(mimeData);

        const auto success = this->model().dropMimeData(
                mimeData, Qt::MoveAction,
                m_originalIndex.row(),
                m_originalIndex.column(),
                m_originalIndex.parent()
        );
        assert(success); (void)success;

    }
    m_originalIndex = {};
}

template<typename BaseT>
inline void Drag<BaseT>::
prepareDrag(QDrag &drag, const QPoint &pos)
{
    //qDebug() << __func__;
    auto* const action = this->actionAt(pos);
    assert(action);

    const auto &index = this->index(*action);
    assert(index.isValid());
    assert(index.model() == &this->model());
    assert(index.parent() == this->rootIndex());

    QModelIndexList indices;
    indices.push_back(index);
    auto* const data = this->model().mimeData(indices);
    assert(data);

    drag.setMimeData(data);

    Base::prepareDrag(drag, pos);
}

}}}} // namespace stream9::qt::mixin::action_view

#endif // MIXIN_ACTION_VIEW_DRAG_HPP
