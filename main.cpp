//Qt interface for radio network
#include <QApplication>

#include "k.h"
#include "d.h"
#include "g.h"
#include "kmain.h"
//======
int main(int argc, char *argv[]){
    QApplication a(argc, argv);

    QString lptr=gptrpngpath;
    if (!fileexists(lptr)) return 1;
    gptrpng=new QPixmap(lptr);
    gptrpng->setMask(
    gptrpng->createMaskFromColor(QColor(255,255,255))
    //createHeuristicMask();
    );

    //database
    int li=dbopen(QString(dbpath));
    if (li) return li;
    //show clear kmain
    kmain w;
    w.setWindowTitle(q("Радиосеть"));
    w.setWindowFlags(Qt::FramelessWindowHint);
    w.show();
    //fill kf
    w.pkf->kfinit();
    //show kf
    w.pkf->setWindowTitle(w.windowTitle());
    w.pkf->show();
    //hide kmain
    w.move(16777000,16777000);
    li=a.exec();
    dbclose();
    return li;
}
