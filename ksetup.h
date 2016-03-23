#ifndef KSETUP_H
#define KSETUP_H
#include "g.h"
#include "d.h"

namespace Ui {class ksetup;}
class ksetup : public kw{
    Q_OBJECT
public:
    explicit ksetup(QWidget *parent = 0,uint lf_i=0);
    ~ksetup();
signals:
    void kclosed();
public slots:
     void ktestip();
     void ktestpath();
     void tabchanged(int li);
     void onaddpath();
private:
    Ui::ksetup *ui;
};





//===============
class mpath : public dm{Q_OBJECT
public:
    static const byte
    cc=4,//col count
    mi=2;//model index to save col width
    hs h[cc]={
    {q("Ip адрес"),Qt::AlignRight,&cipaddr},
    {q("Куда"),Qt::AlignRight,&cgn},
    {q("Путь (ggnn ....)"),Qt::AlignLeft,&cpath},
    {q("Прим."),Qt::AlignLeft,&ctxt}
    };
    QString newgn;
    mpath(QObject *parent):dm(parent){
        tpath->o();
        fill(mi,cc,1);
        }
    inline int rowCount(const QModelIndex &parent = QModelIndex()) const{return
        tpath->rows;}
    static bool cgn(int li,int lj){return ((tpath->r[li].gn)<(tpath->r[lj].gn));}
    static bool cipaddr(int li,int lj){return
        (QString::localeAwareCompare(
        tip->gip_addr(tpath->r[li].ip_i),
        tip->gip_addr(tpath->r[lj].ip_i))<0);}
    static bool cpath(int li,int lj){return
        (tpath->r[li].path)<(tpath->r[lj].path);}
    static bool ctxt(int li,int lj){return
        (QString::localeAwareCompare(
        tpath->r[li].txt,
        tpath->r[lj].txt)<0);}
    void dosort(int column){dmsort(h[column].s);}
    QVariant headerData(int section, Qt::Orientation orientation,int role) const{
        if (role == Qt::DisplayRole){
            if (orientation == Qt::Horizontal) {return h[section].h;}
            else return " ";
            //if (section==currow) {return dmptrchar;} else {return dmptrchars;}
        } else if (orientation==Qt::Vertical){
            if ((role==Qt::DecorationRole)&&(section==currow)){
                return *gptrpng;
            } else if (role==Qt::SizeHintRole) {
               return QSize(gptrpng->width(),1);
            }
        }
        return QVariant();
    }

    QVariant data(const QModelIndex &index, int role) const{
        int lc=index.column();
        if ((role==Qt::DisplayRole)||(role==Qt::EditRole)) {
                int li=i[index.row()];
                switch (lc){
                case 1: return QString::number(tpath->r[li].gn,16).rightJustified(4,'0');break;
                case 0: return QString(tip->gip_addr(tpath->r[li].ip_i));break;
                case 2: return tpath->gpathtohex(li);break;
                case 3: return tpath->r[li].txt;break;
                }
        //} else if (role==Qt::SizeHintRole) {
        //  return QSize(100,20);
        } else if (role == Qt::TextAlignmentRole) {return (h[lc].a+Qt::AlignVCenter);}
        return QVariant();
    }
    Qt::ItemFlags flags(const QModelIndex &index) const {
        return Qt::ItemIsSelectable |  Qt::ItemIsEditable | Qt::ItemIsEnabled ;
    }

    bool setData(const QModelIndex &index, const QVariant &value, int role){
        if ((role == Qt::EditRole)&&(data(index,Qt::DisplayRole)!=value)){
            int lc=index.column();
            int lr=index.row();
            int li=i[lr];
            int lgn=tpath->r[li].gn;
            int lv;
            switch (lc){
            case 1:
                lv=htoi(value);
                lv&=0xFFFF;
                if (lgn!=lv){
                 if (!dsi("select count(*) from path where gn="+itos(lv))) {
                    if (!du("update path set gn=? where gn="+itos(lgn),lv)) {
                        return false;
                    }
                    tpath->r[li].gn=lv;
                 }
                }
                break;
            case 0:
                if (!du("update path set ip_i=? where gn="+itos(tpath->r[li].gn),
                            value.toInt())) {
                        return false;
                    }
                tpath->r[li].ip_i=value.toInt();
                break;
            case 2:
                if (tpath->ghextopath(value.toString(),li)) {
                    if (!du("update path set path=?,len="+itos(tpath->r[li].len)+
                         " where gn="+itos(tpath->r[li].gn),
                            tpath->r[li].path)) {
                        return false;
                    }
                }
                break;
            case 3:
                QString ls1=QString(value.toString());
                if (ls1.localeAwareCompare(tpath->r[li].txt)!=0) {
                    if (!du("update path set txt=? where gn="+itos(lgn),ls1)){
                        return false;
                    }
                    tpath->r[li].txt=ls1;
                }
                break;
            }

        }
        return true;
    }

    bool insertRows ( int row,int count, const QModelIndex & parent){
        //sp(newgn);
        int lgn=htoi(newgn);
        lgn&=0xFFFF;
        if (dsi("select count(*) from path where gn="+itos(lgn))) {
            se(q("Невозможно добавить вторую запись с одинаковым полем Кому"));
            return false;
        }
        int li=dsi("select max(ip_i) from ip");
        QString ls="insert into path(gn,ip_i) values (?,"+itos(li)+")";
        QSqlQuery lq;lq.prepare(ls);
        lq.bindValue(0,lgn);
        if (!lq.exec()) {spe(ls);sp(lq.lastError());lq.clear();return false;}
        db.commit();lq.clear();
        beginInsertRows(parent, row, row);
        int lj=tpath->rows;
        //find to where insert to keep ascending order
        uint lgni=tpath->getgni(lgn);
        sp(lgni);
        tpath->rows+=1;
        ctresize(tpath->r,lj,2);
        //put in the end, then rotate
        tpath->r[lj].gn=lgn;
        tpath->r[lj].ip_i=li;
        //rotate right
        std::rotate(
            tpath->r.rend()-(lj+1),
            tpath->r.rend()-lj,
            tpath->r.rend()-lgni);
        //change i
        ctresize(i,lj,10);
        //recalc i
        int lk=0;
        for(std::vector<int>::iterator it =i.begin();lk<lj;++it) {
            if ((*it)>=lgni) {(*it)++;} lk++;}
        i[lj]=lgni;
        //change i
        //std::rotate(i.rend()-(lj+1),i.rend()-lj,i.rend()-lgni);
        endInsertRows();
        //find and go and sort
        //dg * lg=static_cast<dg *>(parent());
        //lg->gorow(lj);
        return true;
    }
    bool removeRows( int row, int count, const QModelIndex & parent){
        int ln=rowCount();
        if ((ln>0)&&(row>=0)) {
            int lj=i[row];
            int lgn=tpath->r[lj].gn;
            if (dd("delete from path where gn="+itos(lgn))) {
                beginRemoveRows(parent, row, row );
                tpath->clear(lj);
                if (lj!=(ln-1)) {std::rotate(
                    tpath->r.begin()+lj,
                    tpath->r.begin()+lj+1,
                    tpath->r.begin()+ln);
                }
                //change i
                if (row!=(ln-1)) {std::rotate(i.begin()+row,i.begin()+row+1,i.begin()+ln);}
                ln--;
                tpath->rows=ln;
                //recalc i
                int lk=0;
                for(std::vector<int>::iterator it =i.begin();lk<ln;++it) {
                        if ((*it)>lj) {(*it)--;} lk++;}
                endRemoveRows();
                return true;
            }
            return false;
        }
    }
public slots:
    void refreship(int lcol){
       sp("!");
       //beginResetModel();
       //endResetModel();
       emit dataChanged(this->index(0,
       1),index(rowCount()-1,
       1));
       //emit layoutChanged();
    }

};





//=============
class mip : public dm{Q_OBJECT
public:
    static const byte
    mi=3,//model index to save col width
    cc=1;//col count
    hs h[cc]={
    {q("Ip адрес"),Qt::AlignRight,&cip}
    };
    mip(QObject *parent):dm(parent){
        tip->o();
        fill(mi,cc);}
    inline int rowCount(const QModelIndex &parent = QModelIndex()) const{return
        tip->rows;}
    static bool cip(int li,int lj){return
        (QString::localeAwareCompare(
        tip->r[li].ip_addr,
        tip->r[lj].ip_addr)<0);}
    void dosort(int column){dmsort(h[column].s);}
    QVariant headerData(int section, Qt::Orientation orientation,int role) const{
        if (role==Qt::DisplayRole){if(orientation==Qt::Horizontal){return h[section].h;}} return QVariant();}
    QVariant data(const QModelIndex &index, int role) const{
        int lc=index.column();
        if ((role==Qt::DisplayRole)||(role==Qt::EditRole)) {
            int li=i[index.row()];
            switch (lc){
            case 0:return
                   tip->r[li].ip_addr;break;
            }
        } else if (role == Qt::TextAlignmentRole) {return (h[lc].a+Qt::AlignVCenter);}
        return QVariant();
    }
    Qt::ItemFlags flags(const QModelIndex &index) const {
        return Qt::ItemIsSelectable|Qt::ItemIsEditable|Qt::ItemIsEnabled;}
    bool setData(const QModelIndex &index, const QVariant &value, int role){
        if ((role == Qt::EditRole)&&(data(index,Qt::DisplayRole)!=value)){
            //bool lres=true;
            int lc=index.column();
            int lr=index.row();
            int li=i[lr];
            switch (lc){
            case 0:
                QString ls1=value.toString();
                if (ls1.localeAwareCompare(tip->r[li].ip_addr)!=0) {
                    if (!du("update ip set ip_addr=? where ip_i="+
                         itos(tip->r[li].ip_i),ls1)) {
                        return false;
                    } else {tip->r[li].ip_addr=ls1;}
                }
                //clear cache
                tip->resetcache(lc);
                emit coldatachanged(lc);
                //emit layoutChanged();
                break;
            }
            //if (lres) {emit coldatachanged(lc);}
            //return lres;
        }
        return true;
    }
    bool insertRows ( int row,int count, const QModelIndex & parent){
        int li;
        li=dsi("select max(ip_i) from ip")+1;
        QString lname="...";
        QString ls="insert into ip(ip_i,ip_addr) values (?,?)";
        QSqlQuery lq;
        lq.prepare(ls);
        lq.bindValue(0,li);
        lq.bindValue(1,lname);
        if (!lq.exec()) {
            spe(ls);
            sp(lq.lastError());
            lq.clear();
            return false;
        }
        db.commit();
        lq.clear();
        beginInsertRows(parent, row, row);
        int lj=tip->rows;
        tip->rows+=1;
        ctresize(tip->r,lj,2);
        //sp("r--- "+itos(tip->r.size()));
        //put in the end because of max() in select
        tip->r[lj].ip_i=li;
        tip->r[lj].ip_addr=lname;
        //change i
        ctresize(i,lj,10);
        //sp("i "+itos(i.size()));
        if (lj!=row) {
            int li=lj;
            std::vector<int>::iterator it0=i.begin()+lj-1;
            for(std::vector<int>::iterator it=i.begin()+lj;li>row;li--){
                *it--=*it0--;
            }
        }
        i[row]=lj;
        endInsertRows();
        //sp(itos(rowCount()));
        return true;
    }
    bool removeRows( int row, int count, const QModelIndex & parent){
        int ln=rowCount();
        if ((ln>0)&&(row>=0)) {
            //test if exists current ip in other tables
            int lj=i[row];
            int li=tip->r[lj].ip_i;
            if (dsi("select count(*) from path where ip_i="+itos(li))==0){
                if (dd("delete from ip where ip_i="+itos(li))) {
                    beginRemoveRows(parent, row, row );
                    tip->clear(lj);
                    if (lj!=(ln-1)) {
                        std::rotate(
                        tip->r.begin()+lj,
                        tip->r.begin()+lj+1,
                        tip->r.begin()+ln);
                    }
                    //change i
                    if (row!=(ln-1)) {
                        std::rotate(i.begin()+row,i.begin()+row+1,i.begin()+ln);
                    }
                    ln--;
                    tip->rows=ln;
                    int lk=0;
                    for(std::vector<int>::iterator it =i.begin();lk<ln;++it) {
                        if ((*it)>lj) {(*it)--;} lk++;
                    }
                    endRemoveRows();
                    return true;
                }
            } else se(q("Невозможно удалить запись с ссылками на нее из другой таблицы"));
        }
        return false;
    }
    //ret row with ip_i
    int findip(int lip){
      int li=0;
      int ln=rowCount();
      for(std::vector<int>::iterator it=i.begin();li<ln;++it){
        if (tip->r[*it].ip_i==lip) {return li;}
        li++;
      }
    }
};



//==============
class dpathip:public QStyledItemDelegate{Q_OBJECT
public:
    mpath *m;
    mip * md;
    dpathip(QObject *parent = 0){}
    //void keyPressEvent(QKeyEvent* event);
    QWidget * createEditor(QWidget *parent,const QStyleOptionViewItem &lso,const QModelIndex &index) const{
          QComboBox *le=new
          QComboBox(parent);
          le->setFrame(false);
          le->setEditable(true);
          le->lineEdit()->setReadOnly(true);
          le->lineEdit()->setAlignment(Qt::AlignRight);
          //le->setEditable(false);
          md->sort(0);
          le->setModel(md);
          return le;}
    void setEditorData(QWidget *editor,const QModelIndex &index) const{
        QComboBox *le=static_cast<
        QComboBox*>(editor);
        le->setCurrentIndex(md->findip(tpath->r[m->i[m->currow]].ip_i));
        le->showPopup();}
    void setModelData(QWidget *editor, QAbstractItemModel *model,const QModelIndex &index) const{
        QComboBox *le=static_cast<
        QComboBox*>(editor);
        m->setData(m->index(m->currow,m->curcol),
          tip->r[md->i[le->currentIndex()]].ip_i,
          Qt::EditRole);}
    void updateEditorGeometry(QWidget *editor,const QStyleOptionViewItem &option,const QModelIndex &) const{
        editor->setGeometry(option.rect);}
};
//==============
class gpath : public dg{Q_OBJECT
public:
    mpath * m;
    //delegates
    dpathip *
    dip;

    gpath(QWidget *parent=0):dg(parent){
        m=new
        mpath(this);setModel(m);
        canedit=true;
        //delegates
        dip=new
        dpathip(this);setItemDelegateForColumn(
        0,dip);
        dip->m=m;
        horizontalHeader()->setSortIndicator(1, Qt::AscendingOrder);
    }
};
//==============
class gip : public dg{Q_OBJECT
public:
    mip * m;
    gip(QWidget *parent=0):dg(parent){
        m=new
        mip(this);
        setModel(m);
        canedit=true;
    }
};


#endif // KSETUP_H
