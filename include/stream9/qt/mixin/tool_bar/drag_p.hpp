#ifndef MIXIN_TOOLBAR_DRAG_P_HPP
#define MIXIN_TOOLBAR_DRAG_P_HPP

#include <QtCore/QObject>

class QAction;
class QMouseEvent;
class QWidget;

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar {
                                                     namespace drag {

class MouseEventCapturer : public QObject
{
public:
    MouseEventCapturer(QWidget &destination);

    bool eventFilter(QObject* const, QEvent* const);

private:
    bool isLeftButtonPress(QEvent* const) const;
    QMouseEvent translate(QEvent* const) const;

private:
    QWidget &m_destination;
};

}}}}} // namespace stream9::qt::mixin::tool_bar::drag

#endif // MIXIN_TOOLBAR_DRAG_P_HPP
