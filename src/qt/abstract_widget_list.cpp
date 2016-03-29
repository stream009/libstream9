#include "abstract_widget_list.hpp"

#include <cassert>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QDebug> //TODO
#include <QtGui/QMouseEvent>
#include <QtGui/QPalette>
#include <QtWidgets/QVBoxLayout>

namespace stream9 { namespace qt {

AbstractWidgetList::
AbstractWidgetList(QWidget* const parent)
    : Base { parent }
{
    auto* const container = new QWidget { this };

    auto* const layout = new QVBoxLayout { container };
    assert(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);

    m_layout = new QVBoxLayout;
    assert(m_layout);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(0);

    layout->addLayout(m_layout);
    layout->addStretch();

    this->setWidget(container);
    this->setWidgetResizable(true);
}

AbstractWidgetList::~AbstractWidgetList() = default;

void AbstractWidgetList::
setModel(QAbstractItemModel* const model)
{
    clear();
    m_model = model;

    if (m_model == nullptr) return;

    const auto count = model->rowCount();
    for (auto row = 0; row < count; ++row) {
        insertWidget(row);
    }

    this->connect(
        m_model, SIGNAL(rowsInserted(const QModelIndex&, int, int)),
        this,      SLOT(onRowsInserted(const QModelIndex&, int, int)));

    this->connect(
        m_model, SIGNAL(rowsRemoved(const QModelIndex&, int, int)),
        this,      SLOT(onRowsRemoved(const QModelIndex&, int, int)));
}

void AbstractWidgetList::
select(const size_t index)
{
    auto* const item = m_layout->itemAt(static_cast<int>(index));
    assert(item);
    auto* const widget = dynamic_cast<QWidget*>(item->widget());
    assert(widget);

    select(*widget);
}

QModelIndex AbstractWidgetList::
selection() const
{
    return m_selected ? index(*m_selected) : QModelIndex {};
}

void AbstractWidgetList::
clearSelection()
{
    const QModelIndex &before =
                    m_selected ? index(*m_selected) : QModelIndex {};
    deselect();

    Q_EMIT selectionChanged(before, {});
}

void AbstractWidgetList::
keyPressEvent(QKeyEvent* const event)
{
    assert(event);

    switch (event->key()) {
    case Qt::Key_Up:
        moveCursor(-1);
        break;
    case Qt::Key_Down:
        moveCursor(1);
        break;
    case Qt::Key_Home:
        this->ensureVisible(0, 0);
        break;
    case Qt::Key_End: {
            auto* const container = this->widget();
            assert(container);
            const auto &bottomLeft = container->geometry().bottomLeft();
            this->ensureVisible(bottomLeft.x(), bottomLeft.y());
        }
        break;
    default:
        Base::keyPressEvent(event);
        break;
    }
}

void AbstractWidgetList::
mousePressEvent(QMouseEvent* const event)
{
    assert(event);

    auto* const item = findItem(event->pos());
    if (!item) return;

    select(*item);
}

QModelIndex AbstractWidgetList::
index(QWidget &widget) const
{
    assert(m_layout);
    const auto row = m_layout->indexOf(&widget);
    assert(row != -1);

    assert(m_model);
    return m_model->index(row, 0);
}

void AbstractWidgetList::
select(QWidget &item)
{
    const QModelIndex &before =
                    m_selected ? index(*m_selected) : QModelIndex {};

    deselect();

    m_selected = &item;

    const QModelIndex &after = index(item);

    adjustBackgroundRole(item);
    item.setFocus();
    this->ensureWidgetVisible(&item);

    Q_EMIT selectionChanged(before, after);
}

void AbstractWidgetList::
deselect()
{
    if (!m_selected) return;

    auto &widget = *m_selected;
    m_selected = nullptr;
    adjustBackgroundRole(widget);
}

void AbstractWidgetList::
moveCursor(int amount)
{
    if (!m_selected) {
        select(0);
        return;
    }

    const auto current = m_layout->indexOf(m_selected);
    const auto last = m_layout->count() - 1;

    auto next = current + amount;
    if (next < 0) {
        next = 0;
    }
    else if (next > last) {
        next = last;
    }
    assert(0 <= next && next <= last);

    select(static_cast<size_t>(next));
}

void AbstractWidgetList::
clear()
{
    for (auto i = 0, count = m_layout->count(); i < count; ++i) {
        auto* const item = m_layout->itemAt(i);
        assert(item);

        auto* const widget = item->widget();
        assert(widget);

        widget->deleteLater();
        m_layout->removeItem(item);
    }
}

void AbstractWidgetList::
onRowsInserted(const QModelIndex &parent, const int start, const int end)
{
    assert(!parent.isValid()); (void)parent;

    assert(m_model);
    assert(0 <= start && start < m_model->rowCount());
    assert(0 <= end   && end   < m_model->rowCount());

    for (auto row = start; row <= end; ++row) {
        insertWidget(row);
    }
    adjustBackgroundRole(start);
}

void AbstractWidgetList::
onRowsRemoved(const QModelIndex &parent, const int start, const int end)
{
    assert(!parent.isValid()); (void)parent;

    assert(0 <= start && start < m_model->rowCount() + 1);
    assert(0 <= end   && end   < m_model->rowCount() + 1);

    for (auto row = start; row <= end; ++row) {
        removeWidget(row);
    }
    adjustBackgroundRole(start);
}

QWidget *AbstractWidgetList::
findItem(const QPoint &pos) const
{
    QWidget *result = nullptr;
    auto *child = this->childAt(pos);

    while (child != nullptr) {
        if (m_layout->indexOf(child) != -1) {
            result = child;
            break;
        }

        child = child->parentWidget();
    }

    return result;
}

void AbstractWidgetList::
insertWidget(const int row)
{
    assert(m_model);
    auto &&index = m_model->index(row, 0);
    assert(index.isValid());

    auto &item = createItemWidget(index);

    m_layout->insertWidget(row, &item);
    adjustBackgroundRole(item);

    item.show(); // This show() will recalculate geometry immediately.
}

void AbstractWidgetList::
removeWidget(const int row)
{
    auto* const item = m_layout->itemAt(row);
    assert(item);
    auto* const widget = item->widget();
    assert(widget);

    if (widget == m_selected) {
        const auto count = m_layout->count();
        assert(count > 0);
        if (count == 1) {
            deselect();
        }
        else {
            const auto newIndex =
                row != count - 1 ? row + 1 : row - 1;
            assert(0 <= newIndex && newIndex < count);
            select(static_cast<size_t>(newIndex));
        }
    }

    m_layout->removeWidget(widget);
    widget->deleteLater();
}

void AbstractWidgetList::
adjustBackgroundRole(QWidget &widget)
{
    if (&widget == m_selected) {
        widget.setBackgroundRole(QPalette::Highlight);
    }
    else {
        const auto row = m_layout->indexOf(&widget);
        assert(row != -1);
        widget.setBackgroundRole(
            row % 2 ? QPalette::Base : QPalette::AlternateBase);
    }
}

void AbstractWidgetList::
adjustBackgroundRole(const int start)
{
    assert(0 <= start);
    if (start >= m_layout->count()) return; // This occurs when last item is removed.

    for (auto row = start, count = m_layout->count(); row < count; ++row) {
        auto* const item = m_layout->itemAt(row);
        assert(item);
        auto* const widget = item->widget();
        assert(widget);

        adjustBackgroundRole(*widget);
    }
}

}} // namespace stream9::qt
