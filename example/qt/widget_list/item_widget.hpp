#ifndef ITEM_WIDGET_HPP
#define ITEM_WIDGET_HPP

#include "item_actions.hpp"

#include <memory>

#include <QtCore/QPersistentModelIndex>
#include <QtGui/QPalette>
#include <QtWidgets/QMenu>
#include <QtWidgets/QWidget>

class QDateTime;
class QModelIndex;
class QString;
class QUrl;

namespace stream9 { namespace qt {
class AbstractWidgetList;
}} // namespace stream9::qt

namespace Ui { class ItemWidget; }

class ItemWidget : public QWidget
{
    Q_OBJECT
    using Base = QWidget;
public:
    //constructor
    ItemWidget(stream9::qt::AbstractWidgetList&, const QModelIndex&);
    ~ItemWidget() override;

    // query
    const QModelIndex &index() const;

private Q_SLOTS:
    void editTitle();
    void onEditingTitleFinished();
    void onDataChanged(const QModelIndex&, const QModelIndex&);

private:
    void setValues();
    void setTitle(const QString &title);
    void setLinks(const QUrl &homePage, const QUrl &listLocation);
    void setUpdateStatus(const QDateTime &updated, const bool succeed);

private:
    class LinkHandler;

    std::unique_ptr<Ui::ItemWidget> m_ui;
    std::unique_ptr<LinkHandler> m_linkHandler;
    ItemActions m_actions;
    QMenu m_menu;
    stream9::qt::AbstractWidgetList &m_list;
    QPersistentModelIndex m_index;
};

#endif // ITEM_WIDGET_HPP
