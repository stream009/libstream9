#ifndef ACTION_VIEW_HPP
#define ACTION_VIEW_HPP

#include "action_view_p.hpp"

#include <memory>

#include <QtCore/QPersistentModelIndex>
#include <QtCore/QModelIndex>

class QAbstractItemModel;
class QMenu;
class QMouseEvent;
class QStandardItemModel;
class QWidget;

namespace stream9 { namespace qt { namespace mixin {

template<typename BaseT>
class ActionView : public BaseT, public action_view::Slots
{
    using Base = BaseT;
public:
    ActionView(QWidget* const parent);
    ~ActionView() override;

    QAbstractItemModel &model() const;
    void setModel(QAbstractItemModel &,
                  const QModelIndex &root = QModelIndex {});

    const QModelIndex &rootIndex() const;
    void setRootIndex(const QModelIndex &);

    const QModelIndex &currentIndex() const;

    QModelIndex index(QAction &) const;
    QAction *action(const QModelIndex&) const;

    //@overload QWidget
    void addAction(QAction* const);
    //void addActions(const QList<QAction*>&); //TODO
    void insertAction(QAction* const before, QAction* const);
    //void insertActions(QAction* const before, const QList<QAction*>&); //TODO
    void removeAction(QAction* const);

protected:
    virtual QAction &createItemAction(const QModelIndex&);
    virtual bool isMenuItem(const QModelIndex&);
    virtual QMenu &createSubMenu(QAbstractItemModel &,
                    const QModelIndex &, QWidget* const parent) = 0;
protected:
    // @override QWidget
    void mouseMoveEvent(QMouseEvent*) override;

    // @override QObject
    bool event(QEvent*) override;

protected:
    void setCurrentIndex(const QModelIndex &);

    void init();
    void refresh();

    QAction &prepareItemAction(const QModelIndex &parent) /*const*/;
    void addItemAction(QAction&);
    void insertItemAction(QAction* const, QAction&);
    void removeItemAction(QAction&);
    void clearItemActions();

    size_t itemsOffset() const;
    bool isItemAction(QAction* const) const;

private:
    // @override action_view::Slots
    void onDataChanged(const QModelIndex&, const QModelIndex&) override;
    void onRowsInserted(const QModelIndex &parent,
                        const size_t start, const size_t end) override;
    void onRowsRemoved(const QModelIndex &parent,
                       const size_t start, const size_t end) override;

    void setCurrentIndex(const QPoint&);

private:
    QAbstractItemModel *m_model = nullptr;
    QPersistentModelIndex m_rootIndex;
    QPersistentModelIndex m_currentIndex;

    std::unique_ptr<QStandardItemModel> m_defaultModel;
    action_view::SlotsDelegate m_slots;
    std::vector<QAction*> m_itemActions;
    //TODO std::unordered_map<QAction*, QPersistentModelIndex> m_itemActionMap;
    bool m_initialized = false;
};

}}} // namespace stream9::qt::mixin

Q_DECLARE_METATYPE(QPersistentModelIndex);

#include "action_view.tcc"

#endif // ACTION_VIEW_HPP
