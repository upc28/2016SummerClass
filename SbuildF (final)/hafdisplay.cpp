#include "hafdisplay.h"
#include "ui_hafdisplay.h"
#include <QDebug>

HafDisplay::HafDisplay(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HafDisplay)
{
    ui->setupUi(this);
    columun = 0;
    showhafTree = new ShowHafTree;
    showhafTreePro = new ShowHafTreePro;
    ui->tableWidget->horizontalHeader()->setHidden(true);
    ui->tableWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->tableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    ui->tableWidget->setGeometry(ui->tableWidget->x(),ui->tableWidget->y(),ui->tableWidget->width(),ui->tableWidget->height()*1.25);
}

HafDisplay::~HafDisplay()
{
    delete ui;
}
void  HafDisplay::setWordsCount(int num)
{
    ui->tableWidget->setColumnCount(num);
     for(int i=0;i<num;i++)
            ui->tableWidget->setColumnWidth(i,40);
    columun = 0;
    ui->BrowhafCode->clear();
}

void HafDisplay::displayWords(QChar words, qint32 times)
{
    if(words=='\xa')     ui->tableWidget->setItem(0,columun,new QTableWidgetItem("Return"));
    else if(words==' ')     ui->tableWidget->setItem(0,columun,new QTableWidgetItem("Space"));
    //else if(words=='\x0')   ui->tableWidget->setItem(0,columun,new QTableWidgetItem("\x0"));
    else    ui->tableWidget->setItem(0,columun,new QTableWidgetItem(words));
    ui->tableWidget->setItem(1,columun,new QTableWidgetItem(QString::number(times,10)));
    columun++;
}

void HafDisplay::displayHafCode(QChar word, QString str)
{
    if(str=="1101000111")   qDebug()<<word<<QString(" error");
    if(word=='\xa')     ui->BrowhafCode->append(QString("Return")+'\t'+str);
    else if(word==' ')      ui->BrowhafCode->append(QString("Space")+'\t'+str);
    else    ui->BrowhafCode->append(word+'\t'+str);
}

void HafDisplay::on_pushButton_clicked()
{
    this->close();
    showhafTree->close();
}

void HafDisplay::on_But_show_clicked()
{
    showhafTree->show();
}

void HafDisplay::displayNewTree(Node *node)
{
    showhafTree->showNewTree(node);
}

void HafDisplay::on_But_show_pro_clicked()
{
    showhafTreePro->show();
}

void HafDisplay::displayNewHeap(QMap<QChar, qint32> *map)
{
    showhafTreePro->showNewHeap(map);
}
