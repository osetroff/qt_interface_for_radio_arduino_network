#include "k.h"
inline byte ishex(byte lb){
   if ((lb>='0')&&(lb<='9')) {return lb-'0';}
   if ((lb>='a')&&(lb<='f')) {return lb-'a'+10;}
   if ((lb>='A')&&(lb<='F')) {return lb-'A'+10;}
   return 255;
}
int htoi(QVariant lv){
    QByteArray ls=lv.toByteArray();
    //skip
    int ln=ls.size();
    int lh=0;
    if (ln>0) {
        int li=0;
        byte lb;
        while (li<ln) {
            lb=ishex(ls[li++]);
            if (lb!=255) {
                lh<<=4;
                lh+=lb;
            }
        }
    }
    return lh;
}
bool fileexists(QString ls){
    QFileInfo lfi(ls);
    if (!((lfi.exists() && lfi.isFile()))) {
        se(q("Не найден файл ")+ls);
        return false;
    }
    return true;
}

