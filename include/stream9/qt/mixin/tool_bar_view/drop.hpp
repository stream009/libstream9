#ifndef MIXIN_TOOL_BAR_VIEW_DROP_HPP
#define MIXIN_TOOL_BAR_VIEW_DROP_HPP

#include "../action_view/drop.hpp"
#include "../tool_bar/drop.hpp"

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar_view {

template<typename BaseT>
using Drop = mixin::action_view::Drop<
             mixin::tool_bar::Drop<BaseT>>;

}}}} // namespace stream9::qt::mixin::tool_bar_view

#endif // MIXIN_TOOL_BAR_VIEW_DROP_HPP
