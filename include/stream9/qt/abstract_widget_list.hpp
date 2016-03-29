#ifndef WIDGET_LIST_HPP
#define WIDGET_LIST_HPP

#include <QtCore/QModelIndex>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>

class QAbstractItemModel;
class QPoint;
class QWidget;

namespace stream9 { namespace qt {

class AbstractWidgetList : public QScrollArea
{
    Q_OBJECT
    using Base = QScrollArea;
public:
    AbstractWidgetList(QWidget* const parent);
    ~AbstractWidgetList() override;

    QAbstractItemModel* model() const { return m_model; }

    void setModel(QAbstractItemModel* const);

    void select(const size_t);
    QModelIndex selection() const;
    void clearSelection();

Q_SIGNALS:
    void selectionChanged(const QModelIndex&, const QModelIndex&);

private:
    virtual QWidget &createItemWidget(const QModelIndex&) = 0;

    // @override QWidget
    void keyPressEvent(QKeyEvent* const) override;
    void mousePressEvent(QMouseEvent* const) override;

private Q_SLOTS:
    void onRowsInserted(const QModelIndex &parent,
                        const int start, const int end);
    void onRowsRemoved(const QModelIndex &parent,
                       const int start, const int end);

private:
    QModelIndex index(QWidget&) const;

    void select(QWidget&);
    void deselect();

    void moveCursor(int amount);
    void clear();
    QWidget *findItem(const QPoint&) const;
    void insertWidget(const int row);
    void removeWidget(const int row);
    void adjustBackgroundRole(const int start);
    void adjustBackgroundRole(QWidget&);

private:
    QAbstractItemModel *m_model = nullptr;
    QVBoxLayout *m_layout = nullptr;
    QWidget *m_selected = nullptr;
};

}} // namespace stream9::qt

#endif // WIDGET_LIST_HPP
