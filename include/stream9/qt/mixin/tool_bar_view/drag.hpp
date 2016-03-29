#ifndef MIXIN_TOOL_BAR_VIEW_DRAG_HPP
#define MIXIN_TOOL_BAR_VIEW_DRAG_HPP

#include "../action_view/drag.hpp"
#include "../tool_bar/drag.hpp"

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar_view {

template<typename BaseT>
using Drag = mixin::action_view::Drag<
             mixin::tool_bar::Drag<BaseT>>;

}}}} // namespace stream9::qt::mixin::tool_bar_view

#endif // MIXIN_TOOL_BAR_VIEW_DRAG_HPP
