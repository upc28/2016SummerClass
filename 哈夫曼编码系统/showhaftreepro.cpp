#include "showhaftreepro.h"
#include "ui_showhaftreepro.h"
#include <QSlider>


ShowHafTreePro::ShowHafTreePro(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ShowHafTreePro)
{
    ui->setupUi(this);
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(goNext()));

    connect(ui->Slider_speed,SIGNAL(valueChanged(int)),this,SLOT(slideValueChanged()));
}

ShowHafTreePro::~ShowHafTreePro()
{
    delete ui;
}


void ShowHafTreePro::showNewHeap(QMap<QChar, qint32> *m)
{
    map = m;
    row = 0;colum = 0;
    times = 0;
    ui->tableWidget->clear();
    heap = new MinHeap(map->count());
    for ( QMap<QChar,qint32>::iterator it = map->begin(); it != map->end(); it++ )
    {
               heap->insert(new Node(it.key(),it.value()));
    }
    qDebug()<<"show heap run";
    //goNext();
    for(qint32 i =1;i<=heap->length();i++)
    {
        int t = colum;
        nextstep(heap->at(i),0,colum+1);
        ui->tableWidget->item(0,t+1)->setBackgroundColor(Qt::red);
    }
    if(heap->length()==1)
    {
        ui->But_next->setEnabled(false);
        ui->But_autoNext->setEnabled(false);
    }
    //goNext();
   //Node*first,*second;
   /*for(int i=1;i<=map->count()-1;i++)
   {
       first = heap.reMini();
       second = heap.reMini();
       parent = new Node(first->weight+second->weight);
       parent->left=first; parent->right = second;
       heap.insert(parent);
   }*/
}

void ShowHafTreePro::goNext()
{
    colum = 0;
    Node*first,*second,*parent;
    first = heap->reMini();
    second = heap->reMini();
    parent = new Node(first->weight+second->weight);
    parent->left=first; parent->right = second;
    heap->insert(parent);

    ui->tableWidget->clear();
    for(qint32 i =1;i<=heap->length();i++)
    {
        int t = colum;
        nextstep(heap->at(i),0,colum+1);
        ui->tableWidget->item(0,t+1)->setBackgroundColor(Qt::red);
    }
    if(heap->length()==1)
    {
        ui->But_next->setEnabled(false);
        if(timer->isActive())  timer->stop();
        ui->But_autoNext->setEnabled(false);
    }
}


void ShowHafTreePro::nextstep(Node *node, int i,int j)
{
    if(i>=row)  {ui->tableWidget->setRowCount(i+1);row = i+1;}
    if(j>=colum) {ui->tableWidget->setColumnCount(j+1);colum = j+1;}
    if(node->left)
    {
        ui->tableWidget->setItem(i,j,new QTableWidgetItem(QString::number(node->weight,10)));
        ui->tableWidget->item(i,j)->setBackgroundColor(QColor(Qt::yellow));
        nextstep(node->left,i+2,j);
        paintVerLine(j,i,i+2);
        int t=std::max(j+2,maxj);
        nextstep(node->right,i,t);
        paintHorLine(i,j,t);
    }
    else{
        ui->tableWidget->setItem(i,j,new QTableWidgetItem(node->value));
        ui->tableWidget->item(i,j)->setBackgroundColor(QColor(Qt::green));
        maxj = j+2;
    }
     ui->tableWidget->item(i,j)->setTextAlignment(Qt::AlignCenter);
}

void ShowHafTreePro::paintHorLine(int i, int j1, int j2)
{
   if(j2>=colum) {ui->tableWidget->setColumnCount(j2+1);colum = j2+1;}
   ui->tableWidget->setItem(i,j1+1,new QTableWidgetItem("-1-"));
   ui->tableWidget->item(i,j1+1)->setTextAlignment(Qt::AlignCenter);
   qDebug()<<"paint Hor "<<i<<" "<<j1<<"to"<<j2;
    for(j1+=2;j1<j2;j1++)
    {
        ui->tableWidget->setItem(i,j1,new QTableWidgetItem("---"));
        ui->tableWidget->item(i,j1)->setTextAlignment(Qt::AlignCenter);
    }

}

void ShowHafTreePro::paintVerLine(int j, int i1, int i2)
{
    if(i2>=row)  {ui->tableWidget->setRowCount(i2+1);row = i2+1;}
    qDebug()<<"paint Ver "<<j<<" "<<i1<<"to"<<i2;
    for(i1+=1;i1<i2;i1++) {ui->tableWidget->setItem(i1,j,new QTableWidgetItem("|0"));
        ui->tableWidget->item(i1,j)->setTextAlignment(Qt::AlignCenter);}
}

void ShowHafTreePro::on_But_next_clicked()
{
    goNext();
}

void ShowHafTreePro::on_But_close_clicked()
{
    this->close();
}

void ShowHafTreePro::on_But_autoNext_clicked()
{
    timer->start(2050-ui->Slider_speed->value());
    ui->But_autoNext->setEnabled(false);
    ui->But_next->setEnabled(false);
}

void ShowHafTreePro::on_But_reset_clicked()
{
    if(timer->isActive())  timer->stop();
    ui->But_autoNext->setEnabled(true);
    ui->But_next->setEnabled(true);
    showNewHeap(map);
}

void ShowHafTreePro::slideValueChanged()
{
    if(timer->isActive())
    {
        timer->stop();
        timer->start(2050-ui->Slider_speed->value());
    }
}

