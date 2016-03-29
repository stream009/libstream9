#include "main_window.hpp"

#include <cassert>
#include <cstdio>

#include <execinfo.h>
#include <signal.h>

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
#include <QtGui/QApplication>
#else
#include <QtWidgets/QApplication>
#endif

int main(int argc, char *argv[])
{
    QApplication app { argc, argv };
    MainWindow mainWin;

    mainWin.show();

    return app.exec();
}
