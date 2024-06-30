#include <QApplication>
#include "techloginwnd.h"
#include <QDir>
#include"desktoppet.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QDir::setCurrent(a.applicationDirPath());
    TechLoginWnd::getInstance()->show();
    DesktopPet pet;
    MuWinWindow mw;
    QObject::connect(&pet,&DesktopPet::petClicked,[&mw](){
        mw.show();
    });
    pet.show();
    return a.exec();
}
