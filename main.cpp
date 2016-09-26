#include <QApplication>
#include "filedialog.h"


int main(int argc, char *argv[])
{
    std::system("clear");
    QApplication a(argc, argv);
    FileDialog w;
    w.show();
    return a.exec();
}
