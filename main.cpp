#include "MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("jimtendo");
    QCoreApplication::setOrganizationDomain("jazzysites.com.au");
    QCoreApplication::setApplicationName("QuteSQL");

    MainWindow w;
    w.show();

    return a.exec();
}
