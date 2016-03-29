#ifndef MAIN_WINDOW_E554C60D_D28E_489B_9896_7661A66FB6F4
#define MAIN_WINDOW_E554C60D_D28E_489B_9896_7661A66FB6F4

#include <stream9/qt/abstract_widget_list.hpp>
#include "sample_model.hpp"

#include <QtWidgets/QMainWindow>

class WidgetList : public stream9::qt::AbstractWidgetList
{
public:
    WidgetList(QWidget* const parent);

private:
    QWidget &createItemWidget(const QModelIndex &index) override;
};

class MainWindow : public QMainWindow
{
public:
    MainWindow(QWidget* const parent = nullptr);
    ~MainWindow() override;

private:
    SampleModel m_model;
};

#endif // MAIN_WINDOW_E554C60D_D28E_489B_9896_7661A66FB6F4
