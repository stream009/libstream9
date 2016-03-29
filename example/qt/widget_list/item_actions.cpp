#include "item_actions.hpp"

#include "item_widget.hpp"
#include <stream9/qt/abstract_widget_list.hpp>

#include <cassert>

#include <boost/make_unique.hpp>

#include <QtCore/QDebug>
#include <QtWidgets/QAction>
#include <QtWidgets/QWidget>

using stream9::qt::AbstractWidgetList;

ItemActions::
ItemActions(AbstractWidgetList &list, ItemWidget &item)
    : m_list { list },
      m_item { item }
{
    setupActions();
}

ItemActions::~ItemActions() = default;

void ItemActions::
installTo(QWidget &widget)
{
    widget.addAction(&m_editTitle);
    widget.addAction(&m_showHideFilters);
    widget.addAction(&m_delete);
    widget.addAction(&m_separator);
    widget.addAction(&m_moveUp);
    widget.addAction(&m_moveDown);
}

void ItemActions::
onDelete()
{
    auto* const model = m_list.model();
    assert(model);

    model->removeRow(m_item.index().row());
}

void ItemActions::
moveRow(const int from, const int to)
{
    auto* const model = m_list.model();
    assert(model);

    std::vector<QVariant> columns;
    for (auto col = 0, cnt = model->columnCount(); col < cnt; ++col) {
        const auto &index = model->index(from, col);
        assert(index.isValid());
        columns.push_back(model->data(index));
    }

    model->removeRow(from);
    model->insertRow(to);

    for (auto col = 0, cnt = model->columnCount(); col < cnt; ++col) {
        const auto &index = model->index(to, col);
        model->setData(index, columns.at(static_cast<size_t>(col)));
    }

    assert(to >= 0);
    m_list.select(static_cast<size_t>(to));
}

void ItemActions::
onMoveUp()
{
    const auto row = m_item.index().row();
    if (row == 0) return;

    moveRow(row, row - 1);
}

void ItemActions::
onMoveDown()
{
    auto* const model = m_list.model();
    assert(model);

    const auto row = m_item.index().row();
    if (row == model->rowCount() - 1) return;

    moveRow(row, row + 1);
}

static void
setupAction(QAction &action,
            const QString &title, const QKeySequence &shortcut)
{
    action.setText(title);
    action.setShortcut(shortcut);
    action.setShortcutContext(Qt::WidgetShortcut);
}

static void
setupAction(QAction &action,
            const QString &title, const QKeySequence &shortcut,
            QObject &target, const char* const slot)
{
    setupAction(action, title, shortcut);
    action.connect(&action, SIGNAL(triggered()), &target, slot);
}

void ItemActions::
setupActions()
{
    setupAction(m_editTitle,
            "Edit title", Qt::Key_F2, m_item, SLOT(editTitle()));
    setupAction(m_showHideFilters,
            "Show/Hide filters", Qt::CTRL + Qt::Key_R);
    setupAction(m_delete,
            "Delete...", Qt::Key_Delete, *this, SLOT(onDelete()));

    m_separator.setSeparator(true);

    setupAction(m_moveUp,
            "Move up", Qt::CTRL + Qt::Key_Up, *this, SLOT(onMoveUp()));
    setupAction(m_moveDown,
            "Move down", Qt::CTRL + Qt::Key_Down, *this, SLOT(onMoveDown()));
}
