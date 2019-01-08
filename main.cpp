/* Liz Parker and Kyle Schultz
 * Final Project checkpoint1
 * This program is a simplified version of Settlers of Catan
 */

#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
