#ifndef ITEM_ACTIONS_HPP
#define ITEM_ACTIONS_HPP

#include <QtCore/QObject>
#include <QtWidgets/QAction>

class QWidget;

class ItemWidget;

namespace stream9 { namespace qt {
class AbstractWidgetList;
}} // namespace stream9::qt

class ItemActions : public QObject
{
    Q_OBJECT
public:
    ItemActions(stream9::qt::AbstractWidgetList&, ItemWidget&);
    ~ItemActions();

    void installTo(QWidget&);

private:
    void setupActions();
    void moveRow(const int, const int);

private Q_SLOTS:
    void onDelete();
    void onMoveUp();
    void onMoveDown();

private:
    stream9::qt::AbstractWidgetList &m_list;
    ItemWidget &m_item;
    QAction m_editTitle { nullptr };
    QAction m_showHideFilters { nullptr };
    QAction m_delete { nullptr };
    QAction m_separator { nullptr };
    QAction m_moveUp { nullptr };
    QAction m_moveDown { nullptr };
};

#endif // ITEM_ACTIONS_HPP
