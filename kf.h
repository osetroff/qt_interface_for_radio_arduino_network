//main function window
#ifndef KF_H
#define KF_H

#include "k.h"
#include "g.h"
#include "d.h"

//========
namespace Ui {class kf;}
//========
class kf : public QDialog{
    Q_OBJECT

public:
    explicit kf(QWidget *parent = 0);
    ~kf();
signals:
    //close program
    void kclosemain();
public slots:
    //fill kf with data
    void kfinit();
    //exit program
    void kclose(){emit kclosemain();}
    //save connection type
    void ksavecon(int li);
    //open function window
    void kopen();
    //mark zero when window closed
    void markzero(uint lf_i);
    void kfshow(){activateWindow();
                  QDesktopWidget *desktop = QApplication::desktop();
                  move(desktop->screen()->rect().center()-
                   rect().center());
                  }
    void etfsetfocus();
    void etfchanged(QString ls);

private:
    Ui::kf *ui;
    //firsttime
    bool init;
};

//========================
class mf : public dm{Q_OBJECT
public:
    static const byte
    mi=1,//model index to save col width
    cc=2;//col count
    hs h[cc]={
    {q("Функция"),Qt::AlignCenter,&cf},
    {q("Ошибки"),Qt::AlignCenter,&cer}
    };
    mf(QObject *parent):dm(parent){
        tf->o();
        fill(mi,cc);}
    inline int rowCount(const QModelIndex &parent = QModelIndex()) const{return
        tf->rows;}
    static bool cf(int li,int lj){return
        (QString::localeAwareCompare(tf->r[li].f_name,tf->r[lj].f_name)<0);}
    static bool cer(int li,int lj){return
        ((tf->r[li].f_er)<(tf->r[lj].f_er));}
    void dosort(int column){dmsort(h[column].s);}
    QVariant headerData(int section, Qt::Orientation orientation,int role) const{
        if (role==Qt::DisplayRole){if(orientation==Qt::Horizontal){return h[section].h;}} return QVariant();}
//data
    inline QVariant data(const QModelIndex &index, int role= Qt::DisplayRole) const{
        int lc=index.column();
        if ((role==Qt::DisplayRole)||(role==Qt::EditRole)) {
            int li=i[index.row()];
            switch (lc){
            case 0:return tf->r[li].f_name;break;
            case 1:return tf->r[li].f_er;break;
            }
        } else if (role == Qt::TextAlignmentRole) {return h[lc].a;}
        return QVariant();
    }
};
//==============
class gf : public dg{Q_OBJECT
public:
    mf * m;
    gf(QWidget *parent=0): dg(parent){
        m=new
        mf(this);
        setModel(m);
        //canedit=true;
    }
};
#endif // KF_H
