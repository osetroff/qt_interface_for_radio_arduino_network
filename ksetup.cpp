#include "g.h"
#include "ksetup.h"
#include "ui_ksetup.h"
//
ksetup::ksetup(QWidget *parent, uint lf_i) :
    kw(parent,lf_i),
    ui(new Ui::ksetup){

    ctadd(tip,ctip);
    ctadd(tpath,ctpath);
    ui->setupUi(this);
    tabchanged(0);
}
//
ksetup::~ksetup(){
    delete ui;
    ctdel(tpath,ctpath);
    ctdel(tip,ctip);
}
//
void ksetup::tabchanged(int li){
    switch (li) {
    case 0:
        ui->ggip->gorow(0);
        ui->ggpath->gorow(0);
        ui->ggpath->setFocus();
        ui->ggpath->dip->md=ui->ggip->m;
        connect(ui->ggip->m,SIGNAL(coldatachanged(int)),
                ui->ggpath->m,SLOT(refreship(int)));

        break;
    }
}
//
void ksetup::onaddpath(){
      ui->ggpath->m->newgn=ui->lepathto->text();
      ui->ggpath->rowadd();
      ui->ggpath->setCurrentIndex(
         ui->ggpath->m->index(
         ui->ggpath->m->rowCount()-1,ui->ggpath->m->curcol));
      ui->ggpath->m->sort(-1);
      ui->ggpath->gocol(2);
}
//
void ksetup::ktestip(){


}
//
void ksetup::ktestpath(){


}

