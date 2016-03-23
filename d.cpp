//kot.db
#include <QFileInfo>
#include "d.h"
//global database
QSqlDatabase db;
QPixmap * gptrpng;
//======================
//tables
ctf * tf=0;
ctip * tip=0;
ctpath * tpath=0;



















//=========================
void dbclose(){db.commit();db.close();}


int dbopen(QString ldb){
    if (!fileexists(ldb)) return 1;
    db=QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(ldb);
    if (!(db.open())) {
        se(q("Ошибка открытия базы ")+ldb);
        return 2;
    }
    return 0;
}
//return int from sql query
int dsi(QString ls){
        QSqlQuery lq;
        int li=0;
        if (!lq.exec(ls)) {spe(ls);sp(lq.lastError());}
        else {if (lq.next()) {li=lq.value(0).toInt();}}
        lq.clear();
        return li;
}
//update from sql query
bool du(QString ls, QVariant ldata){
        QSqlQuery lq;
        lq.prepare(ls);
        lq.bindValue(0,ldata);
        if (!lq.exec()) {spe(ls);sp(lq.lastError());lq.clear();return false;}
        db.commit();
        lq.clear();
        return true;
}
//delete from sql query
bool dd(QString ls){
        QSqlQuery lq;
        lq.prepare(ls);
        if (!lq.exec(ls)) {spe(ls);sp(lq.lastError());lq.clear();return false;}
        db.commit();
        lq.clear();
        return true;
}
