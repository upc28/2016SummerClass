#ifndef HAFDISPLAY_H
#define HAFDISPLAY_H

#include <QWidget>
#include "showhaftree.h"
#include "minheap.h"
#include "showhaftreepro.h"

namespace Ui {
class HafDisplay;
}

class HafDisplay : public QWidget
{
    Q_OBJECT

public:
    explicit HafDisplay(QWidget *parent = 0);
    ~HafDisplay();
    void displayWords(QChar words, qint32 times);
    void setWordsCount(int num);
    void displayHafCode(QChar word,QString str);
    void displayNewTree(Node* node);
    void displayNewHeap(QMap<QChar,qint32>* map);

private slots:
    void on_pushButton_clicked();

    void on_But_show_clicked();

    void on_But_show_pro_clicked();

private:
    Ui::HafDisplay *ui;
    int columun;

    ShowHafTree* showhafTree;
    ShowHafTreePro* showhafTreePro;
};

#endif // HAFDISPLAY_H
