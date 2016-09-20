#include "showhaftree.h"
#include "ui_showhaftree.h"

ShowHafTree::ShowHafTree( QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowHafTree)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setVisible(true);
    ui->tableWidget->verticalHeader()->setVisible(true);
}

ShowHafTree::~ShowHafTree()
{
    delete ui;
}

void ShowHafTree::nextstep(Node *node, int i,int j)
{
    if(i>=row)  {ui->tableWidget->setRowCount(i+1);row = i+1;}
    if(j>=colum) {ui->tableWidget->setColumnCount(j+1);colum = j+1;}
    if(node->left)
    {
        ui->tableWidget->setItem(i,j,new QTableWidgetItem(QString::number(node->weight,10)));
        ui->tableWidget->item(i,j)->setBackgroundColor(QColor(Qt::green));
        nextstep(node->left,i+2,j);
        paintVerLine(j,i,i+2);
        int t=std::max(j+2,maxj);
        nextstep(node->right,i,t);
        paintHorLine(i,j,t);
    }
    else{
        ui->tableWidget->setItem(i,j,new QTableWidgetItem(node->value));
        ui->tableWidget->item(i,j)->setBackgroundColor(QColor(Qt::yellow));
        maxj = j+2;
    }
     ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
}

void ShowHafTree::paintHorLine(int i, int j1, int j2)
{
   if(j2>=colum) {ui->tableWidget->setColumnCount(j2+1);colum = j2+1;}
   ui->tableWidget->setItem(i,j1+1,new QTableWidgetItem("--1--"));
   qDebug()<<"paint Hor "<<i<<" "<<j1<<"to"<<j2;
    for(j1+=2;j1<j2;j1++)  ui->tableWidget->setItem(i,j1,new QTableWidgetItem("-----"));

}

void ShowHafTree::paintVerLine(int j, int i1, int i2)
{
    if(i2>=row)  {ui->tableWidget->setRowCount(i2+1);row = i2+1;}
    qDebug()<<"paint Ver "<<j<<" "<<i1<<"to"<<i2;
    for(i1+=1;i1<i2;i1++) {ui->tableWidget->setItem(i1,j,new QTableWidgetItem("|0"));
        ui->tableWidget->item(i1,j)->setTextAlignment(Qt::AlignCenter);}
}

void ShowHafTree::showNewTree(Node *node)
{
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setRowCount(1);
    row = 1;colum = 1;
    nextstep(node,0,0);
}

void ShowHafTree::on_pushButton_clicked()
{
    this->close();
}
