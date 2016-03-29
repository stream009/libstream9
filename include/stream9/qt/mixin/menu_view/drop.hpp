#ifndef MIXIN_MENU_VIEW_DROP_HPP
#define MIXIN_MENU_VIEW_DROP_HPP

#include "../action_view/drop.hpp"
#include "../menu/drop.hpp"

namespace stream9 { namespace qt { namespace mixin { namespace menu_view {

template<typename BaseT>
using DropBase = mixin::action_view::Drop<
                 mixin::menu::Drop<BaseT>>;

template<typename BaseT>
class Drop : public DropBase<BaseT>
{
    using Base = DropBase<BaseT>;
public:
    Drop(QWidget* const parent);

    virtual bool isEmpty() const = 0;

protected:
    // @override mixin::widget::Drop
    using Base::canDrop;
    bool canDrop(const QDragMoveEvent &) override;
    void onDrop(QDropEvent &) override;
};

}}}} // namespace stream9::qt::mixin::menu_view

namespace stream9 { namespace qt { namespace mixin { namespace menu_view {

template<typename BaseT>
inline Drop<BaseT>::
Drop(QWidget* const parent)
    : Base { parent }
{}

template<typename BaseT>
inline bool Drop<BaseT>::
canDrop(const QDragMoveEvent &event)
{
    if (this->isEmpty()) return true;

    return Base::canDrop(event);
}

template<typename BaseT>
inline void Drop<BaseT>::
onDrop(QDropEvent &event)
{
    if (this->isEmpty()) {
        auto* const mimeData = event.mimeData();
        assert(mimeData);

        const auto success =
            this->model().dropMimeData(mimeData, event.dropAction(),
                             0, 0, this->rootIndex());
        if (!success) return;

        event.acceptProposedAction();
    }
    else {
        Base::onDrop(event);
    }
}

}}}} // namespace stream9::qt::mixin::menu_view

#endif // MIXIN_MENU_VIEW_DROP_HPP
