#include "MainWindow.h"
#include <QApplication>
//#include "SetSDKLocation.h"
//#include "OpenDatabaseUi.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
