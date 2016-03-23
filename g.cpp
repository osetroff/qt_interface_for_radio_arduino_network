#include "g.h"
/*
//=======================================================
// delegates
//==============
SpinBoxDelegate::SpinBoxDelegate(QObject *parent)
    : QStyledItemDelegate(parent){
}
//
QWidget *SpinBoxDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &lso,
    const QModelIndex &index) const{
    sp("ce");
    if (index.column()==2) {
        QSpinBox *editor = new QSpinBox(parent);
        editor->setFrame(false);
        editor->setMinimum(0);
        editor->setMaximum(100);
        return editor;
    } else {
        return QStyledItemDelegate::createEditor(parent,lso,index);
    }
}
//
void SpinBoxDelegate::setEditorData(QWidget *editor,const QModelIndex &index) const{
    sp("sed");
    if (index.column()==2) {
        int value = index.model()->data(index, Qt::EditRole).toInt();
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->setValue(value);
    } else {
        QStyledItemDelegate::setEditorData(editor,index);
    }
}
//
void SpinBoxDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
                                   const QModelIndex &index) const{
    sp("smd");
    if (index.column()==2) {
        QSpinBox *spinBox = static_cast<QSpinBox*>(editor);
        spinBox->interpretText();
        int value = spinBox->value();

        model->setData(index, value, Qt::EditRole);
    } else {
        QStyledItemDelegate::setModelData(editor,model,index);
    }
}
//
void SpinBoxDelegate::updateEditorGeometry(QWidget *editor,
    const QStyleOptionViewItem &option, const QModelIndex &) const{
    sp("ueg");
    editor->setGeometry(option.rect);
}


void SpinBoxDelegate::keyPressEvent(QKeyEvent* event){
    QStyledItemDelegate::keyPressEvent(event);
}
*/





