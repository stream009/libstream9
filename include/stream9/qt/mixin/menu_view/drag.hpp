#ifndef MIXIN_MENU_VIEW_DRAG_HPP
#define MIXIN_MENU_VIEW_DRAG_HPP

#include "../action_view/drag.hpp"
#include "../menu/drag.hpp"

namespace stream9 { namespace qt { namespace mixin { namespace menu_view {

template<typename BaseT>
using Drag = mixin::action_view::Drag<
             mixin::menu::Drag<BaseT>>;

}}}} // namespace stream9::qt::mixin::menu_view

#endif // MIXIN_MENU_VIEW_DRAG_HPP
