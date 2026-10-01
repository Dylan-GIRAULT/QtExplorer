#include "qtexplorer.h"

#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QApplication::setOrganizationName("Girault");
    QApplication::setApplicationName("Gt Explorer");
    QApplication::setApplicationVersion(QT_VERSION_STR);
    QApplication::setWindowIcon(QIcon(":/icon/Bentley.jpg"));
    QtExplorer w;
    w.show();
    return QApplication::exec();
}
