#include "kmain.h"
#include "ui_kmain.h"
#include "ui_kf.h"
//=======
kmain::kmain(QWidget *parent) :
    QMainWindow(parent),ui(new Ui::kmain){
    ui->setupUi(this);
    pkf=new kf(this);
    connect(pkf,SIGNAL(kclosemain()),this,SLOT(kclose()));
}
//=======
kmain::~kmain(){delete ui;}
