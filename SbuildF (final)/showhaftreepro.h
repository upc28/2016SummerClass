#ifndef SHOWHAFTREEPRO_H
#define SHOWHAFTREEPRO_H

#include <QWidget>
#include <QTimer>
#include "minheap.h"

namespace Ui {
class ShowHafTreePro;
}

class ShowHafTreePro : public QWidget
{
    Q_OBJECT

public:
    explicit ShowHafTreePro(QWidget *parent = 0);
    void  showNewHeap(QMap<QChar,qint32>* m);
    ~ShowHafTreePro();

private slots:
    void on_But_next_clicked();

    void on_But_close_clicked();

    void on_But_autoNext_clicked();

    void on_But_reset_clicked();

    void goNext();

    void slideValueChanged();

private:
    Ui::ShowHafTreePro *ui;
    QMap<QChar,qint32>* map;
    MinHeap* heap;
    void nextstep(Node* node,int i,int j);
    int count,maxj;
    int row,colum;
    void paintVerLine(int j,int i1,int i2);
    void paintHorLine(int i,int j1,int j2);
    int times;
    QTimer* timer;

};

#endif // SHOWHAFTREEPRO_H
