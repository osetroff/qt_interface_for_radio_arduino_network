#ifndef GF_H
#define GF_H
#include "d.h"
#include "g.h"

class gf : public dg{Q_OBJECT
public:
    mf * m;
    gf(QWidget *parent=0): dg(parent){
        m=new
        mf(this);
        setModel(m);
        //canedit=true;
    }
    //void fill(){m=new mf(this);setModel(m);}
};
#endif
