#ifndef KMAIN_H
#define KMAIN_H
#include <QMainWindow>
#include "kf.h"
//========
namespace Ui {class kmain;}
class kmain : public QMainWindow{
    Q_OBJECT
public:
    explicit kmain(QWidget *parent = 0);
    ~kmain();
    //function window
    kf * pkf;
public slots:
    void kclose(){ qApp->quit();}
protected:
    void changeEvent( QEvent* e ){
        QMainWindow::changeEvent( e );
        if( (e->type() == QEvent::ActivationChange)
             && (this->isActiveWindow())){
           //spn("kmain::activated");
           pkf->activateWindow();
        }
    }
private:
    Ui::kmain *ui;
};
#endif // KMAIN_H
