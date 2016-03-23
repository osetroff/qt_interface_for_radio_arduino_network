#ifndef G_H
#define G_H
//base class dbgrid
//delegates
#include "k.h"



//==============
class dgk: public QObject{Q_OBJECT
public:  dgk(QWidget *parent){}
signals: void keypressed(int lk);
protected:
    bool eventFilter(QObject *obj, QEvent *event){
          if (event->type()==QEvent::KeyPress){
              QKeyEvent *lk=static_cast<QKeyEvent *>(event);
              if (lk->key()==Qt::Key_Return) {
                //sp("emitkeyp");
                emit keypressed(lk->key());
                return true;
              }
          }
          return QObject::eventFilter(obj,event);
    }

};
class dh: public QHeaderView{Q_OBJECT
public:
 dh(Qt::Orientation orientation, QWidget * parent = 0):
  QHeaderView(orientation, parent){
    sectionResizeMode(QHeaderView::Interactive);
    //setSortIndicatorShown(true);
    setSortIndicator(0, Qt::AscendingOrder);
  }
protected:
    void mousePressEvent(QMouseEvent * event){
        QHeaderView::mousePressEvent(event);
        Qt::MouseButton lb=event->button();
        if ((lb==Qt::LeftButton)||(lb==Qt::RightButton)){
            emit dhmouse(logicalIndexAt(event->pos()),lb);
        }
    }
signals:
    void dhmouse(int lcol,Qt::MouseButton lbtn);
};
//=================
//  dbgrid
//==========
//!!!! always set sorting=true to enable restore col width
class dg : public QTableView{Q_OBJECT
signals:
    void newselected(int lr,int lc);
    void returnpressed();
    void selected();
    void loadcolwidth(int lcol);
public:
    //~dg();
    //goto row, -1 go to next
    void gorow(int lr){
        if (model()) {
            int ln=model()->rowCount();
            if (lr<0) {
              lr=currentIndex().row()+1;
              if (lr>=ln) {lr=0;}
            }
            int lc=currentIndex().column();
            if (lc<0) {lc=0;}
            if ((lc<model()->columnCount())&&(lr<ln)) {
                setCurrentIndex(model()->index(lr,lc));
                scrollTo(model()->index(lr,lc));
            }
        }
    }
    //goto col, -1 go to next
    void gocol(int lc){
        if (model()) {
            int ln=model()->columnCount();
            if (lc<0) {
              lc=currentIndex().column()+1;
              if (lc>=ln) {lc=0;}
            }
            int lr=currentIndex().row();
            if (lr<0) {lr=0;}
            if ((lr<model()->rowCount())&&(lc<ln)) {
                setCurrentIndex(model()->index(lr,lc));
                scrollTo(model()->index(lr,lc));
            }
        }
    }

    void selectionChanged(const QItemSelection & selected, const QItemSelection & deselected){
        emit newselected(currentIndex().row(),currentIndex().column());
        QTableView::selectionChanged(selected,deselected);
    }
    bool canedit=false;
    void keypressed(int lk) {
        if (lk==Qt::Key_Return) {
            emit returnpressed();
            if (canedit) {
                //sp("canedit");
                if (state()!=QAbstractItemView::EditingState) {
                    edit(currentIndex());
                }
            }// else sp("readonly");
        }
    }
public slots:
    void dhmouse(int lcol,Qt::MouseButton lb){
      //sort by mouse
      if (lb==Qt::LeftButton) {
       model()->sort(lcol,Qt::AscendingOrder);
       horizontalHeader()->setSortIndicator(lcol, Qt::AscendingOrder);
      } else if (lb==Qt::RightButton) {
       model()->sort(lcol,Qt::DescendingOrder);
       horizontalHeader()->setSortIndicator(lcol, Qt::DescendingOrder);
      }
    }
    void aftersorted(int li){gorow(li);}
    void rowadd(){
        model()->insertRow(currentIndex().row());
        gorow(currentIndex().row()-1);
        setFocus();
    }
    void rowdel(){
        model()->removeRow(currentIndex().row());
        gorow(currentIndex().row());
        setFocus();
    }
    void resizerc(int oldcount,int newcount){
        //resizeColumnsToContents();
        resizeRowsToContents();
    }
public:
    dh * hh;
    dg(QWidget *parent){
        dgk * lk=new dgk(this);
        connect(lk,&dgk::keypressed,this,&keypressed);
        this->installEventFilter(lk);
        const QColor hlClr = Qt::blue; // highlight color to set
        const QColor txtClr = Qt::white; // highlighted text color to set
        QPalette p = palette();
        p.setColor(QPalette::Highlight, hlClr);
        p.setColor(QPalette::HighlightedText, txtClr);
        setPalette(p);
        //connect(this->verticalHeader(),&QHeaderView::sectionCountChanged,
        //        this,&resizerc);
        setSelectionBehavior(QAbstractItemView::SelectRows);
        hh=new dh(Qt::Horizontal);
        //hh->setClickable(true);
        setHorizontalHeader(hh);
        connect(hh,SIGNAL(dhmouse(int,Qt::MouseButton)),this,SLOT(dhmouse(int,Qt::MouseButton)));
        //horizontalHeader()->sectionResizeMode(QHeaderView::Interactive);
        //connect(this->verticalHeader(),&QHeaderView::geometriesChanged,
        //        this,&resizerc);
        //set rows height  = h-(h/(1+1/(rowwhitetopbottom/rowtextheight)))
        int li=verticalHeader()->minimumSectionSize();
        verticalHeader()->setDefaultSectionSize(li-(li/7));
    }
    void mousePressEvent(QMouseEvent *event){
        QTableView::mousePressEvent(event);
        if (event->button() == Qt::LeftButton) {
            if (canedit) {
                if (state()!=QAbstractItemView::EditingState) {
                    edit(currentIndex());
                }
            }
        }
    }
};

//=========================
class kw: public QDialog{Q_OBJECT
public:
    explicit kw(QWidget *parent = 0, uint lf_i=0){ f_i=lf_i;}
    ~kw(){emit markzero(f_i);}
    //mark zero when window has closed
    uint f_i;
signals:
    void markzero(uint lf_i);
    void kfshow();
public slots:
    void fshow(){emit kfshow();}
};





//==============
class dek: public QObject{Q_OBJECT
public:  dek(QWidget *parent){}
signals: void keypressed(int lk);
protected:
    bool eventFilter(QObject *obj, QEvent *event){
          if (event->type()==QEvent::KeyPress){
              QKeyEvent *lk=static_cast<QKeyEvent *>(event);
              if (lk->key()==Qt::Key_Return) {
                //sp("emitkeyp");
                emit keypressed(lk->key());
                return true;
              }
          }
          return QObject::eventFilter(obj,event);
    }
};
//=================
//  lineedit
//==========
class de : public QLineEdit{Q_OBJECT
signals:
    void returnpressed();
public:
    de(QWidget *parent){
        dek * lk=new dek(this);
        connect(lk,&dek::keypressed,this,&keypressed);
        this->installEventFilter(lk);
    }
    void keypressed(int lk) {
        if (lk==Qt::Key_Return) {
            //sp("kp");
            emit returnpressed();
        }
    }
};

/*
//===========================================
// delegates
//===============
    //SpinBoxDelegate * dgd=new SpinBoxDelegate;
    //ui->dgf->setItemDelegate(dgd);
class SpinBoxDelegate : public QStyledItemDelegate{Q_OBJECT
public:
    SpinBoxDelegate(QObject *parent = 0);
    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
                          const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setEditorData(QWidget *editor, const QModelIndex &index) const Q_DECL_OVERRIDE;
    void setModelData(QWidget *editor, QAbstractItemModel *model,
                      const QModelIndex &index) const Q_DECL_OVERRIDE;
    void updateEditorGeometry(QWidget *editor,
        const QStyleOptionViewItem &option, const QModelIndex &index) const Q_DECL_OVERRIDE;
    //void keyPressEvent(QKeyEvent* event);
};
*/


#endif


