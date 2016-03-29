#include "item_widget.hpp"
#include "ui_item_widget.h"

#include <stream9/qt/abstract_widget_list.hpp>

#include <cassert>

#include <QtCore/QDateTime>
#include <QtCore/QDebug> //TODO
#include <QtCore/QString>
#include <QtCore/QUrl>
#include <QtGui/QCursor>
#include <QtGui/QTextDocument>
#include <QtWidgets/QSizePolicy>
#include <QtWidgets/QToolTip>

class ItemWidget::LinkHandler : public QObject
{
    Q_OBJECT
public:
    LinkHandler(QObject &target)
        : m_target { target }
    {
        this->connect(&target, SIGNAL(linkHovered(const QString&)),
                         this,   SLOT(onLinkHovered(const QString&)));
        m_target.installEventFilter(this);
    }

private Q_SLOTS:
    // @override QObject
    bool eventFilter(QObject* const target, QEvent* const event) override
    {
        assert(target); assert(event);
        if (target != &m_target) return false;

        if (event->type() == QEvent::ToolTip) {
            QToolTip::showText(QCursor::pos(), m_toolTip);
        }

        return false;
    }

    void onLinkHovered(const QString &link)
    {
        m_toolTip = link;
    }

private:
    QObject &m_target;
    QString m_toolTip;
};

#include "item_widget.moc"

using stream9::qt::AbstractWidgetList;

ItemWidget::
ItemWidget(AbstractWidgetList &list, const QModelIndex &index)
    : Base { &list },
      m_ui { new Ui::ItemWidget },
      m_actions { list, *this },
      m_list { list },
      m_index { index }
{
    assert(m_ui);
    m_ui->setupUi(this);

    assert(m_index.isValid());

    assert(m_ui->title);
    m_ui->title->setVisible(true);

    assert(m_ui->titleEdit);
    m_ui->titleEdit->setVisible(false);
    this->connect(m_ui->titleEdit, SIGNAL(editingFinished()),
                  this,              SLOT(onEditingTitleFinished()));

    assert(m_ui->links);
    m_linkHandler.reset(new LinkHandler { *m_ui->links });

    this->setAutoFillBackground(true);

    m_actions.installTo(*this);
    m_actions.installTo(m_menu);

    assert(m_ui->button);
    m_ui->button->setMenu(&m_menu);

    setValues();

    auto* const model = m_list.model();
    assert(model);

    this->connect(model, SIGNAL(dataChanged(const QModelIndex&,
                                            const QModelIndex&)),
                  this,    SLOT(onDataChanged(const QModelIndex&,
                                              const QModelIndex&)));
}

ItemWidget::~ItemWidget() = default;

const QModelIndex &ItemWidget::
index() const
{
    assert(m_index.isValid());
    return m_index;
}

void ItemWidget::
setTitle(const QString &title)
{
    assert(m_ui->title);
    m_ui->title->setText(tr("<b>%1</b>").arg(title));
}

void ItemWidget::
setLinks(const QUrl &homePage, const QUrl &listLocation)
{
    assert(m_ui->links);
    m_ui->links->setText(tr(R"(
        (<a href="%1">Homepage</a>, <a href="%2">Filter list</a>)
    )").arg(homePage.toString()).arg(listLocation.toString()));
}

void ItemWidget::
setUpdateStatus(const QDateTime &updated, const bool succeed)
{
    assert(m_ui->updateStatus);
    m_ui->updateStatus->setText(tr(
        "Last downloaded: %1 (%2)"
    ).arg(updated.toString()).arg(succeed ? "Success" : "Fail"));
}

void ItemWidget::
editTitle()
{
    assert(m_ui->title);
    assert(m_ui->titleEdit);

    QTextDocument doc;
    doc.setHtml(m_ui->title->text());
    m_ui->titleEdit->setText(doc.toPlainText());

    m_ui->title->setVisible(false);
    m_ui->titleEdit->setVisible(true);
    m_ui->titleEdit->setFocus();
    m_ui->titleEdit->selectAll();
}

void ItemWidget::
onEditingTitleFinished()
{
    //TODO validate
    assert(m_ui->title);
    assert(m_ui->titleEdit);

    setTitle(m_ui->titleEdit->text());

    m_ui->title->setVisible(true);
    m_ui->titleEdit->setVisible(false);
}

void ItemWidget::
onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if (topLeft.row() < m_index.row()) return;
    if (bottomRight.row() > m_index.row()) return;

    setValues();
}

void ItemWidget::
setValues()
{
    auto* const model = m_list.model();
    assert(model);

    const auto &titleIndex = model->index(m_index.row(), 0);
    assert(titleIndex.isValid());
    setTitle(titleIndex.data().toString());

    const auto &homePage = model->index(m_index.row(), 1);
    assert(homePage.isValid());
    const auto &location = model->index(m_index.row(), 2);
    assert(location.isValid());
    setLinks(homePage.data().toUrl(), location.data().toUrl());

    const auto &updated = model->index(m_index.row(), 3);
    assert(updated.isValid());
    const auto &status = model->index(m_index.row(), 4);
    assert(status.isValid());
    setUpdateStatus(updated.data().toDateTime(), status.data().toBool());
}
