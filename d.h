#ifndef D_H
#define D_H

#include "k.h"
#include "g.h"

#include <QtSql>
//global database
extern QSqlDatabase db;
#define dbpath "C:\\_\\_home\\_arduino\\!kot\\k\\kot.db"
#define gptrpngpath "C:\\_\\_home\\_arduino\\!kot\\k\\ptr.png"
//==============
// functions
//==============
//open db
int dbopen(QString ldb);
void dbclose();
//return int from sql query
int dsi(QString ls);
//update from sql query
bool du(QString ls, QVariant ldata);
//delete etc
bool dd(QString ls);


//===========
#define ctadd(lct,T) if (lct==0) {lct=new T();} lct->refcount+=1;
#define ctdel(lct,T) if (lct!=0) {\
    if (lct->refcount==1) {lct->~T();lct=0;} else {lct->refcount-=1;}}
#define dmsort(a) std::sort(i.begin(),i.begin()+rowCount(),(a))
#define ctresize(a,lj,n) if ((a.size())<((uint)(lj)+1)) \
   {a.resize((uint)(lj)+n);a.shrink_to_fit();}



//===========
// data model
//==========
extern QPixmap * gptrpng;
class dm : public QAbstractTableModel{Q_OBJECT
signals:
    void coldatachanged(int column);
private:
    int cols;
public:
    //horisontalHeader struct
    struct hs {
      QString h;
      Qt::AlignmentFlag a;
      fcomp s;
    };
    bool setcolwidthok=false;
    int mi;
    int currow;
    int curcol;
    int lastsortedcol;
    Qt::SortOrder lastsortedord;
    std::vector<int> i;//index of rows in ctable
    //get row index
    inline int gri(int lr){return i[lr];}
    inline int gcri(){if (currow>0) {return i[currow];} return 0;}
    dm(QObject *parent):QAbstractTableModel(parent){
      currow=-1;curcol=-1;lastsortedcol=-1;cols=-1;
      connect((dg *)parent,SIGNAL(newselected(int,int)),this,SLOT(newselected(int,int)));
      connect(this,SIGNAL(sorted(int)),(dg *)parent,SLOT(aftersorted(int)));
    }
    inline void fill(int lmi,int lcols,int sortcolumn=0){
        mi=lmi;
        cols=lcols;
        uint ln=rowCount();
        i.resize(ln+4);
        uint li=0;
        for(std::vector<int>::iterator it=i.begin();li<ln;++it) { *it=li++;}
        dg *lg=static_cast<dg *>(parent());
        QHeaderView* hh = lg->horizontalHeader();
        //!!!this have to do manually in ui: setSortingEnabled(true)
        connect(hh,SIGNAL(sectionResized(int,int,int)),
          this,SLOT(savecolwidth(int,int,int)));
    }
    //sort
/*
    handleSortIndicatorChanged(self, index, order):
        if index != 0:
            self.table.horizontalHeader().setSortIndicator(
                0, self.table.model().sortOrder())
*/
    inline virtual void dosort(int column){}
    void sort(int column, Qt::SortOrder order=Qt::AscendingOrder){
        if (column<0) {column=lastsortedcol;}
        else if ((lastsortedcol==column)&&(lastsortedord==order)) {return;}

        int lrc=rowCount();
        if ((lrc>1)&&(column<columnCount())&&(column>=0)){
            beginResetModel();
            if (currow<0) currow=0;
            int lrs=i[currow];
            dosort(column);
            int lj=(lrc-1);
            if (order!=Qt::AscendingOrder) {
                for (int li=0;li<=( lj >> 1);li++){
                    std::iter_swap(i.begin()+li,i.begin()+lj-li);
                }
            }
            currow=-1;int li=-1;
            for(std::vector<int>::iterator it = i.begin(); ++li<=lj; ++it) {
                if (*it==lrs) {currow=li;break;}
            }
            endResetModel();
            lastsortedcol=column;
            lastsortedord=order;
            if (!setcolwidthok) {setcolwidths();}
            emit sorted(currow);
        }
    }
    inline int columnCount(const QModelIndex &parent = QModelIndex()) const{return cols;}
public slots:
    /*
    void refreshcol(int lcol){
        emit dataChanged(index(0,lcol),index(rowCount()-1,lcol));
        emit layoutChanged();
    }
    */
    //!!!if it is don't work -> restore defaults for horisHeader in ui
    void setcolwidths(){
        dg *lg=static_cast<dg *>(this->parent());
        QHeaderView* hh = lg->horizontalHeader();
        for(int li=0;li<cols;li++){
            int lj=(mi*100)+li;
            int lw=dsi("select data from setup where id="+itos(lj));
            if (lw==0) {
                if (!dsi("select count(*) from setup where id="+itos(lj))){
                    lw=hh->sectionSize(li);
                    if (lw<10) {lw=30;}
                    du("insert into setup(id,data) values("+itos(lj)+",?)",lw);}
            } else {
               if (lw<10) {lw=30;}
               hh->resizeSection(li,lw);
            }
        }
        setcolwidthok=true;
    }
    inline void newselected(int lr,int lc){currow=lr;curcol=lc;}
    void savecolwidth(int col,int los,int lns){
        if (setcolwidthok) {
           du("update setup set data=? where id="+itos((mi*100)+col),lns);
        }
    }
signals:
    void editcompleted(int row,int col);
    void sorted(int li);
};














//---------------------
//base class for tables
class ctable {
public:
  int opencount=0;
  int refcount=0;
  int rows=0;
  bool cacheok=false;
  virtual void oo(){}//open
  virtual void cc(bool lrealloc=true){}//close
  inline bool isopen(){return (opencount>0);}
  void c(bool lrealloc=true){//close
    if (opencount>0) {
        opencount--;
        if (opencount==0) {
            cc(lrealloc);
            rows=0;
        }
    }
  }
  void o(){//open
    opencount++;
    if (opencount==1){oo();}
  }
  //clear cache
  virtual void resetcache(int lcol){}
  //free resources like QString.clear for deleted row data
  virtual void clear(int lri){}

  ctable(){opencount=0;cacheok=false;}
  ~ctable(){if (opencount>0) {cc();}}
};






//====================================
// f
//====================================
class ctf: public ctable{
public:
  struct rs{//row
        uint f_i;//index
        byte f_er;//errors count
        QString f_name;//function name
        QDialog * f;//0 or ptr to QDialog
  };
  std::vector <rs> r;//rows ordered by key
  inline void cc(bool lrealloc=true){//close
      r.clear();if (lrealloc) {std::vector <rs>().swap(r);}}
  inline void oo(){//open
    QString dsel=
    "select f.f_i,f.f_name";
    //get user id
    int user_id=dsi("select data from setup where id=2");
    QString dselfrom=
    " from f,access where f.f_i=access.f_i and access.user_i="+
            itos(user_id);
    QString dorder=
    " order by f.f_i";
    rows=dsi("select count(*)"+dselfrom);
    r.resize(rows);r.shrink_to_fit();
    QSqlQuery lq;
    QString ls=dsel+dselfrom+dorder;
    lq.prepare(ls);
    if (!lq.exec()) {se(ls);}
    else {
        uint li=0;
        while (lq.next()) {
            r[li].f_i=lq.value(0).toInt();
            r[li].f_name=lq.value(1).toString();
            r[li].f_er=0;
            r[li].f=0;
            li++;
        }
    }
  }
};
extern ctf * tf;








//====================================
// ip
//====================================
class ctip: public ctable{
public:
  struct rs{
       uint ip_i;//index
       QString ip_addr;//ip address
  };
  std::vector <rs> r;//rows ordered by key
  inline void cc(bool lrealloc=true){//close
      r.clear();if (lrealloc) {std::vector <rs>().swap(r);}}
  inline void oo(){//open
    QString dsel=
    "select ip_i,ip_addr";
    QString dselfrom=
    " from ip";
    QString dorder=
    " order by ip_i";
    rows=dsi("select count(*)"+dselfrom);
    r.resize(rows+1);r.shrink_to_fit();
    QSqlQuery lq;
    QString ls=dsel+dselfrom+dorder;
    lq.prepare(ls);
    if (!lq.exec()) {se(ls);}
    else {
        uint li=0;
        while (lq.next()) {
                r[li].ip_i=lq.value(0).toInt();
                r[li].ip_addr=lq.value(1).toString();
                li++;
        }
    }
  }
  void clear(int lri){
      r[lri].ip_addr.clear();
  }
  //cache
  rs cache;
  void resetcache(int lcol){cacheok=false;}
  QString gip_addr(uint ip_i){
      if (cacheok) {
          if (ip_i==cache.ip_i) {return cache.ip_addr;}
      }
      //search
      int le=rows;
      if (le>0) {
        le--;
        int lb=0;
        uint li;
        uint lj;
        do {
           li=lb+((le-lb)>>1);
           lj=r[li].ip_i;
           if (lj==ip_i){
               cacheok=true;
               cache.ip_i=ip_i;
               cache.ip_addr=r[li].ip_addr;
               return cache.ip_addr;
               break;
           } else if (lj>ip_i) {
               le=li-1;
           } else { //lj<ip_i
               lb=li+1;
           }
        } while (lb<=le);
      }
      return QString("");
  }
};
extern ctip * tip;


//====================================
//====================================
#define ctpathmax 16
class ctpath: public ctable{
public:
  struct rs{
       word gn;//to
       uint ip_i;//index of ip_addr
       byte len;//path len
       QByteArray path;//path
       QString txt;//comments
  };
  std::vector <rs> r;//rows ordered by key
  inline void cc(bool lrealloc=true){//close
      r.clear();if (lrealloc) {std::vector <rs>().swap(r);}}
  inline void oo(){//open
    QString dsel=
    "select gn,ip_i,len,path,txt";
    QString dselfrom=
    " from path";
    QString dorder=
    " order by gn";
    rows=dsi("select count(*)"+dselfrom);
    r.resize(rows+2);r.shrink_to_fit();
    QSqlQuery lq;
    QString ls=dsel+dselfrom+dorder;
    lq.prepare(ls);
    if (!lq.exec()) {se(ls);}
    else {
        uint li=0;
        while (lq.next()) {
                r[li].gn=lq.value(0).toInt();
                r[li].ip_i=lq.value(1).toInt();
                r[li].len=lq.value(2).toInt();
                r[li].path=lq.value(3).toByteArray();
                r[li].txt=lq.value(4).toString();
                li++;
        }
    }
  }
  void clear(int lri){
      r[lri].path.clear();
      r[lri].txt.clear();
  }
  inline char toh(byte lb){
      if (lb>=10) return lb+='a'-10;
      return lb+='0';
  }
  QString gpathtohex(int li){
      QString ls="";
      int ll=r[li].len;
      if (ll) {
          QByteArray la=r[li].path;
          int ln=(la.size())>>1;
          //if ll>ln -> error
          if (ll>ln) {ll=ln;r[li].len=ln;}
          ln<<=1;
          //
          int li=0;
          byte lb,lb1;
          while (li<ln){
              lb=la[li++];lb1=la[li++];
              ls.push_back(toh(lb>>4));
              ls.push_back(toh(lb&0xF));
              ls.push_back(toh(lb1>>4));
              ls.push_back(toh(lb1&0xF));
              ls+=" ";
          }
      }
      return ls;
  }
  //true if we have to save path to db
  bool ghextopath(QString ls,int lri){
      int ll=ls.size();
      if (ll) {
          int ln=0;
          QByteArray la;
          int li=0;
          uint lgn=0;
          byte lb;
          while (li<ll){
            lb=ls[li++].toLatin1();
            if (lb!=' ') {
                lb=ishex(lb);
                if (lb!=255) {
                  lgn<<=4;
                  lgn+=lb;
                  while (li<ll){
                    lb=ishex(ls[li++].toLatin1());
                    if (lb==255) break;
                    lgn<<=4;
                    lgn+=lb;
                  }
                  la.push_back((lgn>>8)&0xFF);
                  la.push_back(lgn&0xFF);
                  ln+=1;
                }
            }
          }
          if (ln) {
            if (r[lri].len==ln) {
              if (r[lri].path==la) {return false;}
            } else {r[lri].len=ln;}
            r[lri].path=la;
            return true;
          }
      }
      //ln==0
      if (r[lri].len==0) {
        if (r[lri].path.size()==0) {return false;}
      } else {r[lri].len=0;}
      r[lri].path.resize(0);
      return true;
  }
  //return pos in r where to insert gn or where gn==gn
  uint getgni(int lgn){
    uint le=rows;//end
    uint li=0;
    if (le>0) {
        if (le>1) {
            le--;
            uint lb=0;//begin
            uint lj=0;
            do {
                li=lb+((le-lb)>>1);
                lj=r[li].gn;
                if (lj==lgn){
                    return li;
                } else if (lj>lgn) {
                    if (li==0) {return li;}
                    le=li-1;
                } else {lb=li+1;}
            } while ((lb!=le)&&(lb<le));
            li=lb;
        }
        if (r[li].gn<lgn){li++;}
    }
    return li;
  }
};
extern ctpath * tpath;

#endif


