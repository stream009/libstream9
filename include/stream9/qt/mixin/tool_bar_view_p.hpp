#ifndef TOOL_BAR_VIEW_P_HPP
#define TOOL_BAR_VIEW_P_HPP

class QAction;

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar_view {

class Slots
{
public:
    virtual void onActionTriggered(QAction&) = 0;
};

}}}} // namespace stream9::qt::mixin::tool_bar_view

#include <QtCore/QObject>

class QAction;

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar_view {

class SlotsDelegate : public QObject
{
    Q_OBJECT
public:
    SlotsDelegate(Slots &host);

private Q_SLOTS:
    // connect with QToolBar
    void onActionTriggered(QAction*);

private:
    Slots &m_host;
};

}}}} // namespace stream9::qt::mixin::tool_bar_view

#include <QtCore/QObject>

class QEvent;
class QObject;
class QWidget;

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar_view {

class MouseMoveEventCapturer : public QObject
{
public:
    MouseMoveEventCapturer(QWidget &target);

private:
    bool eventFilter(QObject* const obj, QEvent* const event) override;

private:
    QWidget &m_target;
};

}}}} // namespace stream9::qt::mixin::tool_bar_view

class QEvent;
class QObject;

namespace stream9 { namespace qt { namespace mixin { namespace tool_bar_view {

class ToolBarMiddleButtonHandler : public QObject
{
private:
    bool eventFilter(QObject* const, QEvent* const) override;
};

}}}} // namespace stream9::qt::mixin::tool_bar_view

#endif // TOOL_BAR_VIEW_P_HPP
