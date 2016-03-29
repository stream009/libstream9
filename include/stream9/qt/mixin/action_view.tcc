#include "action_view.hpp"

#include <algorithm>
#include <cassert>

#include <boost/range/algorithm_ext/erase.hpp>

#include <QtCore/QAbstractItemModel>
#include <QtGui/QHoverEvent>
#include <QtGui/QMouseEvent>
#include <QtGui/QStandardItemModel>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QWidget>
#else
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>
#include <QtWidgets/QWidget>
#endif

namespace stream9 { namespace qt { namespace mixin {

template<typename BaseT>
ActionView<BaseT>::
ActionView(QWidget* const parent)
    : Base { parent },
      m_slots { *this }
{
    m_defaultModel.reset(new QStandardItemModel);
    assert(m_defaultModel);
    this->setModel(*m_defaultModel);

    assert(m_model);
}

template<typename BaseT>
ActionView<BaseT>::~ActionView() = default;

template<typename BaseT>
QAbstractItemModel &ActionView<BaseT>::
model() const
{
    assert(m_model);
    return *m_model;
}

template<typename BaseT>
void ActionView<BaseT>::
setModel(QAbstractItemModel &model,
         const QModelIndex &root/* = QModelIndex {}*/)
{
    if (m_model) {
        m_model->disconnect(&m_slots);
    }

    m_model = &model;
    assert(m_model);
    m_rootIndex = root;

    this->connect(
        m_model,  SIGNAL(rowsInserted(const QModelIndex&, int, int)),
        &m_slots,   SLOT(onRowsInserted(const QModelIndex&, int, int)));
    this->connect(
        m_model,  SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
        &m_slots,   SLOT(onRowsRemoved(const QModelIndex&, int, int)));
    this->connect(
        m_model,  SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
        &m_slots,   SLOT(onDataChanged(const QModelIndex&, const QModelIndex&)));
}

template<typename BaseT>
const QModelIndex &ActionView<BaseT>::
rootIndex() const
{
    return m_rootIndex;
}

template<typename BaseT>
void ActionView<BaseT>::
setRootIndex(const QModelIndex &index)
{
    m_rootIndex = index;
}

template<typename BaseT>
const QModelIndex &ActionView<BaseT>::
currentIndex() const
{
    return m_currentIndex;
}

template<typename BaseT>
QModelIndex ActionView<BaseT>::
index(QAction &action) const
{
    return action.data().template value<QPersistentModelIndex>();
}

template<typename BaseT>
QAction *ActionView<BaseT>::
action(const QModelIndex &index) const
{
    const auto end = m_itemActions.end();
    const auto it = std::find_if(m_itemActions.begin(), end,
        [&index, this] (QAction* const action) {
            assert(action);
            return this->index(*action) == index;
        }
    );

    return it == end ? nullptr : *it;
}

template<typename BaseT>
void ActionView<BaseT>::
addAction(QAction* const action)
{
    assert(action);
    const auto offset = this->itemsOffset();
    const auto &actions = this->actions();
    assert(actions.size() >= 0);
    const auto size = static_cast<size_t>(actions.size());
    auto* const before = offset == size ? nullptr : actions.at(offset);

    Base::insertAction(before, action);
}

template<typename BaseT>
void ActionView<BaseT>::
insertAction(QAction* const before, QAction* const action)
{
    assert(action);
    if (!isItemAction(before)) {
        Base::insertAction(before, action);
    }
    else {
        addAction(action);
    }
}

template<typename BaseT>
QAction &ActionView<BaseT>::
createItemAction(const QModelIndex &index)
{
    assert(index.isValid());
    assert(index.model() == &this->model());
    assert(index.parent() == this->rootIndex());

    auto &model = this->model();
    const auto &text = model.data(index).toString();

    QAction *result = nullptr;
    if (isMenuItem(index)) {
        auto &subMenu = createSubMenu(model, index, this);
        subMenu.setTitle(text);
        result = subMenu.menuAction();
    }
    else {
        result = new QAction { text, this };
    }
    assert(result);

    return *result;
}

template<typename BaseT>
bool ActionView<BaseT>::
isMenuItem(const QModelIndex &index)
{
    return this->model().hasChildren(index);
}

template<typename BaseT>
void ActionView<BaseT>::
removeAction(QAction* const action)
{
    assert(action);
    if (!isItemAction(action)) {
        Base::removeAction(action);
    }
}

template<typename BaseT>
inline void ActionView<BaseT>::
mouseMoveEvent(QMouseEvent* const event)
{
    assert(event);
    Base::mouseMoveEvent(event);

    setCurrentIndex(event->pos());
}

template<typename BaseT>
bool ActionView<BaseT>::
event(QEvent* const ev)
{
    assert(ev);
    if (ev->type() == QEvent::HoverMove) {
        auto* const event = dynamic_cast<QHoverEvent*>(ev);
        assert(event);

        setCurrentIndex(event->pos());
    }
    return Base::event(ev);
}

template<typename BaseT>
void ActionView<BaseT>::
setCurrentIndex(const QModelIndex &index)
{
    m_currentIndex = index;
}

template<typename BaseT>
void ActionView<BaseT>::
init()
{
    if (!m_initialized) {
        refresh();
        m_initialized = true;
    }
}

template<typename BaseT>
void ActionView<BaseT>::
refresh()
{
    assert(m_model);

    clearItemActions();

    assert(m_model->rowCount(m_rootIndex) >= 0);
    const auto rowCount = static_cast<size_t>(m_model->rowCount(m_rootIndex));

    for (auto row = 0u; row < rowCount; ++row) {
        const auto &index = m_model->index(row, 0, m_rootIndex);
        assert(index.isValid());
        auto &action = prepareItemAction(index);

        addItemAction(action);
    }

    m_currentIndex = QModelIndex {};
}

template<typename BaseT>
inline void ActionView<BaseT>::
onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    assert(m_model);
    assert(topLeft.isValid());
    assert(bottomRight.isValid());
    assert(topLeft.parent() == bottomRight.parent());
    assert(topLeft.column() == 0);
    assert(bottomRight.column() == 0);

    if (topLeft.parent() != m_rootIndex) return;
    if (!m_initialized) return;

    const auto offset = itemsOffset();

    for (auto row = topLeft.row(); row <= bottomRight.row(); ++row) {
        auto* const oldAction = this->actions().at(row + offset);
        assert(oldAction);

        const auto &index = m_model->index(row, 0, m_rootIndex);
        assert(index.isValid());
        auto &newAction = prepareItemAction(index);

        insertItemAction(oldAction, newAction);
        removeItemAction(*oldAction);
        if (oldAction->menu()) {
            oldAction->menu()->deleteLater();
        }
        else {
            oldAction->deleteLater();
        }
    }
}

template<typename BaseT>
inline void ActionView<BaseT>::
onRowsInserted(
    const QModelIndex &parent, const size_t start, const size_t end)
{
    assert(parent.isValid());
    const auto rowCount = parent.model()->rowCount(parent); (void)rowCount;
    assert(rowCount >= 0);
    assert(start < static_cast<size_t>(rowCount));
    assert(end < static_cast<size_t>(rowCount));
    assert(start <= end);
    assert(m_model);

    if (parent != m_rootIndex) return;
    if (!m_initialized) return;

    const auto offset = itemsOffset();

    const auto &actions = this->actions();
    assert(actions.size() >= 0);
    const size_t count = actions.size();
    for (auto row = start; row <= end; ++row) {
        auto* const before = (offset + row) < count ?
                                actions.at(offset + row) : nullptr;

        const auto &index = m_model->index(row, 0, m_rootIndex);
        auto &newAction = prepareItemAction(index);

        insertItemAction(before, newAction);
    }
}

template<typename BaseT>
inline void ActionView<BaseT>::
onRowsRemoved(const QModelIndex &parent, const size_t start, const size_t end)
{
    assert(parent.isValid());
    const auto rowCount = parent.model()->rowCount(parent); (void)rowCount;
    assert(rowCount >= 0);
    assert(start <= end);
    const size_t count = end - start + 1;
    assert(start < rowCount + count);
    assert(end < rowCount + count);

    if (parent != m_rootIndex) return;
    if (!m_initialized) return;

    const auto offset = itemsOffset();

    for (auto i = 0u; i < count; ++i) {
        assert(offset + start < static_cast<size_t>(this->actions().size()));
        auto* const action = this->actions().at(offset + start);
        assert(action);
        removeItemAction(*action);

        auto* menu = action->menu();
        if (menu) {
            // We need delete this NOW in order to prevent
            // receiving an unwanted signal.
            // I tried disconnect and deleteLater, but it didn't make
            // them go away.
            //delete menu;

            // However, by deleting menu now, events got delivered to
            // the deleted object. That's far much worse.
            // Affected code: mouseMoveEvent() in menu_view.tcc
            menu->deleteLater();
        }
        else {
            action->deleteLater();
        }
    }
}

template<typename BaseT>
QAction &ActionView<BaseT>::
prepareItemAction(const QModelIndex &index)
{
    assert(index.isValid());

    QPersistentModelIndex pIndex { index };
    assert(pIndex.isValid());

    auto &action = createItemAction(index);
    action.setData(QVariant::fromValue(pIndex));

    const auto &toolTip = m_model->data(index, Qt::ToolTipRole).toString();
    action.setToolTip(toolTip);

    return action;
}

template<typename BaseT>
void ActionView<BaseT>::
addItemAction(QAction &action)
{
    m_itemActions.push_back(&action);
    Base::addAction(&action);
}

template<typename BaseT>
void ActionView<BaseT>::
insertItemAction(QAction* const before, QAction &action)
{
    const auto nextPos = [before, this] {
        const auto end = m_itemActions.end();
        if (before) {
            const auto it = std::find(m_itemActions.begin(), end, before);
            return it == end ? end : it + 1;
        }
        else {
            return end;
        }
    }();

    m_itemActions.insert(nextPos, &action);
    Base::insertAction(before, &action);
}

template<typename BaseT>
void ActionView<BaseT>::
removeItemAction(QAction &action)
{
    boost::remove_erase(m_itemActions, &action);
    Base::removeAction(&action);
}

template<typename BaseT>
void ActionView<BaseT>::
clearItemActions()
{
    for (auto* const action: m_itemActions) {
        Base::removeAction(action);
        auto* const menu = action->menu();
        if (menu) {
            menu->deleteLater();
        }
        else {
            action->deleteLater();
        }
    }
    m_itemActions.clear();
}

template<typename BaseT>
size_t ActionView<BaseT>::
itemsOffset() const
{
    const auto offset = this->actions().size() - m_itemActions.size();
    assert(offset >= 0);
    return offset;
}

template<typename BaseT>
bool ActionView<BaseT>::
isItemAction(QAction* const action) const
{
    const auto end = m_itemActions.end();
    const auto it = std::find(m_itemActions.begin(), end, action);
    return it != end;
}

template<typename BaseT>
void ActionView<BaseT>::
setCurrentIndex(const QPoint &pos)
{
    auto* const action = this->actionAt(pos);
    if (action) {
        const auto &index = this->index(*action);
        if (index.isValid()) {
            this->setCurrentIndex(index);
            return;
        }
    }
    this->setCurrentIndex(QModelIndex {});
}

}}} // namespace stream9::qt::mixin
