#include "main_window.hpp"

#include "menu.hpp"
#include "tool_bar.hpp"

#include <QtCore/QDebug>
#include <QtGui/QStandardItem>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QToolBar>
#else
#include <QtWidgets/QToolBar>
#endif

MainWindow::
MainWindow(QWidget* const parent)
    : QMainWindow(parent)
{
    m_ui.setupUi(this);

    createModel();
    createMenuBar();
    createToolBar();
}

void MainWindow::
quit()
{
    QApplication::quit();
}

void MainWindow::
onTriggered(const QModelIndex &index)
{
    qDebug() << __func__ << index;
}

void MainWindow::
onHovered(const QModelIndex &/*index*/)
{
    //qDebug() << __PRETTY_FUNCTION__ << index;
}

void MainWindow::
createModel()
{
    auto* const root = new QStandardItem { "root" };
    m_model.appendRow(root); // model takes ownership
    m_rootIndex = root->index();

    root->setChild(0, new QStandardItem { "first" }); // root takes ownership
    root->setChild(1, new QStandardItem { "second" });
    auto* child = new QStandardItem { "third" };
    assert(child);
    child->setToolTip("3rd");
    root->setChild(2, child);

    auto* item = root->child(0);
    item->setChild(0, new QStandardItem { "I-1" }); // item takes ownership
    item->setChild(1, new QStandardItem { "I-2" });
    child = new QStandardItem { "I-3" };
    assert(child);
    child->setToolTip("third");
    item->setChild(2, child);
}

void MainWindow::
createMenuBar()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    this->menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&Quit"), this, SLOT(quit()), tr("Ctrl+Q"));

    auto* const menu = new Menu { this };
    menu->setModel(m_model, m_rootIndex);

    menu->setTitle("My Menu");
    menu->addAction(new QAction { tr("&Add Bookmark"), this });
    menu->addAction(new QAction { tr("&Manage Bookmarks"), this });
    menu->addSeparator();

    this->menuBar()->addMenu(menu);

    this->connect(menu, SIGNAL(triggered(const QModelIndex&)),
                  this,   SLOT(onTriggered(const QModelIndex&)));
    this->connect(menu, SIGNAL(hovered(const QModelIndex&)),
                  this,   SLOT(onHovered(const QModelIndex&)));
}

void MainWindow::
createToolBar()
{
    auto* const toolBar = new ToolBar { this };
    toolBar->setModel(m_model, m_rootIndex);
    this->addToolBar(toolBar);

    this->connect(toolBar, SIGNAL(actionTriggered(const QModelIndex&)),
                  this,      SLOT(onTriggered(const QModelIndex&)));
}
