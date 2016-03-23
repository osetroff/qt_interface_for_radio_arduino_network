#ifndef K_H
#define K_H

#include <QTableView>
#include <QAbstractTableModel>
#include <QStyledItemDelegate>
#include <QSpinBox>
#include <QMessageBox>
#include <QDialog>
#include <QKeyEvent>
#include <QFileDialog>
#include <QString>
#include <QHeaderView>
#include <QApplication>
#include <QDesktopWidget>
#include <QLineEdit>
#include <QDebug>
#include <QBitmap>

//===================================================
//types
//============
//8
typedef unsigned char byte;
//16
typedef unsigned short word;
//32
typedef unsigned int dword;

typedef bool (*fcomp)(int,int);
//=====================================================
// macros
//============
#define q(ls) QObject::tr(ls)
//int to str
#define itos(li) QString::number(li)
#define se(ls) QMessageBox::about(0, q("Ошибка"),(ls))
#define sp(ls) qDebug() << (ls)
#define spe(ls) qDebug() << q("Error: ") << (ls)
//#define spn(s) sp(s);qDebug() << "\n"


//======================================================
// functions
//=============
//int to str
#define itos(li) QString::number(li)
byte ishex(byte lb);
int htoi(QVariant lv);
bool fileexists(QString ls);
#endif


