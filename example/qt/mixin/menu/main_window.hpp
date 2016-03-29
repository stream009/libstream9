#ifndef MAIN_WINDOW_HPP
#define MAIN_WINDOW_HPP

#include <QtCore/QtGlobal>
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
};

#endif // MAIN_WINDOW_HPP
