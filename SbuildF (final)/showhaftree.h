#ifndef SHOWHAFTREE_H
#define SHOWHAFTREE_H

#include <QWidget>
#include "minheap.h"

namespace Ui {
class ShowHafTree;
}

class ShowHafTree : public QWidget
{
    Q_OBJECT

public:
    explicit ShowHafTree(QWidget *parent = 0);
    ~ShowHafTree();
    void showNewTree(Node* node);
private slots:
    void on_pushButton_clicked();

private:
    Ui::ShowHafTree *ui;

    void nextstep(Node* node,int i,int j);
    int count,maxj;
    int row,colum;
    void paintVerLine(int j,int i1,int i2);
    void paintHorLine(int i,int j1,int j2);
};

#endif // SHOWHAFTREE_H
