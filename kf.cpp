//========
//function window
#include "d.h"
#include "kf.h"
#include "ui_kf.h"

//=========
#include "ksetup.h"
//#include "kwaterplant.h"


//=========
kf::kf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::kf){
    init=true;

    ctadd(tf,ctf);
    ui->setupUi(this);
    setWindowTitle(parent->windowTitle());
    setWindowFlags(windowFlags()|Qt::WindowMinMaxButtonsHint);
    ui->ggf->gorow(0);
}
//=========
kf::~kf(){
    delete ui;
    ctdel(tf,ctf);
}
//fill kf with data
void kf::kfinit(){
  if (init) {
    //load connection type
    int li=dsi("select data from setup where id=1");
    if (ui->cbc->count()>li) {ui->cbc->setCurrentIndex(li);}
    init=false;


    //when debug to skip menu
    QTimer::singleShot(500,this,SLOT(kopen()));
  }
}

//open function window
void kf::kopen(){
    int li=ui->ggf->currentIndex().row();
    QDialog *lf=0;
    if (li>=0) {lf=tf->r[ui->ggf->m->gri(li)].f;}
    if (lf) {
        lf->activateWindow();
    } else {
        uint lf_i=tf->r[ui->ggf->m->gri(li)].f_i;
        setDisabled(true);
//==============
        QWidget * lp = static_cast<QWidget *>(this->parent());
        switch (lf_i) {
//!!just comment line if you won't compile or use punkt menu
        case 0: lf=new ksetup(lp,lf_i);break;
//        case 1: lf=new kwaterplant(lp,lf_i);break;
        default: {
            setDisabled(false);
            activateWindow();
            ui->etf->setFocus();
            return;
            }
        }
//===============
        //ui->ggf->saveopen(lf);
        if (li>=0) {tf->r[ui->ggf->m->gri(li)].f=lf;}

        lf->setAttribute(Qt::WA_DeleteOnClose);
        lf->setWindowTitle(
            ui->ggf->model()->data(
                ui->ggf->model()->index(ui->ggf->selectionModel()->currentIndex().row(),0)
            ).toString()
        );
        lf->setWindowFlags(lf->windowFlags()|Qt::WindowMinMaxButtonsHint);
        //
        connect(lf,SIGNAL(markzero(uint)),this,SLOT(markzero(uint)));
        connect(lf,SIGNAL(kfshow()),this,SLOT(kfshow()));
        //show centered and maximized
        lf->showMaximized();
        setDisabled(false);
    }
    //sp(itos(ui->dgfmenu->selectionModel()->currentIndex().row()));
}
//mark zero when window closed
void kf::markzero(uint lf_i){
    //ui->ggf->markzero(lf_i);
    //find row with lf_i and make f=0
    int lj=tf->rows;int li=-1;
    for(std::vector<ctf::rs>::iterator it = tf->r.begin(); ++li<lj; ++it) {
            if ((*it).f_i==lf_i) {(*it).f=0;break;}
    }
}
void kf::etfsetfocus(){ui->etf->setFocus();}
void kf::ksavecon(int li){
        if (!init) {du("update setup set data=? where id="+itos(1),li);}
}
void kf::etfchanged(QString ls){
        //etf changed -> find similar
        if (ui->ggf->m) {
            //sp(ls);
            int lr=0;
            int li=ls.length();
            if (li>0) {
                li=0;
                ls=ls.toLower();
                int ln=ui->ggf->m->rowCount();
                while (li<ln){
                    QString ls1=tf->r[ui->ggf->m->gri(li)].f_name;
                    if (ls1.indexOf(ls,0,Qt::CaseInsensitive)==0){
                        lr=li;
                        break;
                    }
                    li++;
                }
            }
            ui->ggf->gorow(lr);
        }
}

