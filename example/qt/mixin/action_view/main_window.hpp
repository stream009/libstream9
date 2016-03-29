#ifndef MAIN_WINDOW_E554C60D_D28E_489B_9896_7661A66FB6F4
#define MAIN_WINDOW_E554C60D_D28E_489B_9896_7661A66FB6F4

#include "ui_main_window.h"
#include <QtCore/QModelIndex>
#include <QtGui/QStandardItemModel>
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QMainWindow>
#else
#include <QtWidgets/QMainWindow>
#endif

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* const parent = nullptr);

private Q_SLOTS:
    void onHovered(const QModelIndex&);
    void onTriggered(const QModelIndex&);
    void quit();

private:
    void createModel();
    void createMenuBar();
    void createToolBar();

private:
    Ui::MainWindow m_ui;
    QStandardItemModel m_model;
    QModelIndex m_rootIndex;
};

#endif // MAIN_WINDOW_E554C60D_D28E_489B_9896_7661A66FB6F4
