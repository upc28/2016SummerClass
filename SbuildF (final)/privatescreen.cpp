#include "privatescreen.h"
#include "ui_privatescreen.h"
#include "qdebug.h"
#include <iostream>
#include <QTextCodec>
#include <QMap>
#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <QStack>
#include <QQueue>
#include <QTime>
using namespace std;

PrivateScreen::PrivateScreen(QTcpSocket* socket, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PrivateScreen)
{
    ui->setupUi(this);

    map = new QMap<QChar,qint32>;
    hafMap = new QMap<QChar,QString>;
    tcpsocket = socket;
    this->setWindowTitle(QString("消息窗口")+tcpsocket->peerAddress().toString());
    connect(tcpsocket,SIGNAL(readyRead()),this,SLOT(readMsg()));
    connect(tcpsocket,SIGNAL(bytesWritten(qint64)),this,SLOT(Socket_sendMsg()));
    hafWin = new HafDisplay;
    hafWin->move(this->x()-1.4*this->frameSize().width(),this->y());
    hisWin = new HistoryDisplay;
    hisWin->move(this->x()-this->frameSize().width(),this->y()+this->frameSize().height());
}

PrivateScreen::~PrivateScreen()
{
    delete ui;
}

void PrivateScreen::on_But_send_clicked()
{    
     QString gettext = ui->textEdit->toPlainText();
     ui->textBrowser->append(QString("Time:")+QTime::currentTime().toString());
     ui->textBrowser->append(gettext);

     if(gettext.length()==0)  return;
     toHaf(gettext);
     toHistory(gettext);
     sendHaf();
     ui->textEdit->clear();
     hafWin->displayNewTree(parent);
     //sendMsg(gettext);

}

void PrivateScreen::toHaf(QString gettext)
{
    map->clear();
    for(int i=0;i<gettext.length();i++)
    {
        if(map->find(gettext[i])==map->end())   map->insert(gettext[i],1);
        else    map->insert(gettext[i],map->find(gettext[i]).value()+1);
    }

    MinHeap heap(map->count());
    hafWin->setWordsCount(0);
    hafWin->setWordsCount(map->count());
    for ( QMap<QChar,qint32>::iterator it = map->begin(); it != map->end(); it++ )
    {
                hafWin->displayWords(it.key(),it.value());
               heap.insert(new Node(it.key(),it.value()));
    }
   Node*first,*second;
   for(int i=1;i<=map->count()-1;i++)
   {
       first = heap.reMini();
       second = heap.reMini();
       parent = new Node(first->weight+second->weight);
       parent->left=first; parent->right = second;
       heap.insert(parent);
   }
   hafMap->clear();
  // qDebug()<<"count: "<<map->count();
   if(map->count()==1)
   {
       hafWin->displayHafCode(map->begin().key(),"0");
       hafMap->insert(map->begin().key(),QString("0"));
       parent = new Node(map->begin().key(),map->begin().value());
       return;
   }
   buildHafCode(parent,"");
   for(QMap<QChar,QString>::iterator it = hafMap->begin();it!=hafMap->end();it++)
       hafWin->displayHafCode(it.key(),it.value());
   hafWin->displayNewHeap(map);
}

void PrivateScreen::on_But_close_clicked()
{
    hafWin->close();
    hisWin->close();

    this->close();
}

void PrivateScreen::on_pushButton_clicked()
{

    hafWin->show();
}

void PrivateScreen::buildHafCode(Node *node, QString str)
{
    if(!node->left&&!node->right)
    {
        qDebug()<<node->value<<' '<<str;
        hafMap->insert(node->value,str);
    }
    if(node->left)  buildHafCode(node->left,str+'0');
    if(node->right) buildHafCode(node->right,str+'1');
}

void PrivateScreen::on_But_history_clicked()
{
   hisWin->show();
}

void PrivateScreen::toHistory(QString str)
{
    hafStr.clear();
    hafStr = "";
    for(int i=0;i<str.length();i++)
    {
        hafStr+=hafMap->find(str[i]).value();
    }
    hisWin->displayMsg(str);
    hisWin->displayMsgHaf(hafStr);
}

void PrivateScreen::readMsg()
{
    QDataStream in(tcpsocket);
    in.setVersion(QDataStream::Qt_5_2);
    qint32 inhafCount;
    in>>inhafCount;
    hafArray = new TreeN[inhafCount];
    for(int i=0;i<inhafCount;i++)
    {
        in>>hafArray[i].data>>hafArray[i].left>>hafArray[i].right;
        qDebug()<<"get tree"<<hafArray[i].data<<" "<<hafArray[i].left<<" "<<hafArray[i].right;
    }
    qint32 incodeCount;
    qint32 hafStrLength;
    in>>hafStrLength;
    in>>incodeCount;
    QString inhafStr = "",inTemp;
    int inInt32;
    for(int i=0;i<incodeCount;i++)
    {
        in>>inInt32;
        inTemp = QString::number(inInt32,2);
        while(inTemp.length()!=31)  inTemp.push_front('0');
        inhafStr+=inTemp;
        qDebug()<<QString("get int ")<<inInt32<<QString("g ")<<inTemp;
    }
    int ttimes = inhafStr.length()-hafStrLength;
    inhafStr.remove(0,ttimes);
    qint32 k = 0;
    QString outToBrowser = "";
    qDebug()<<QString("g ")<<inhafStr;
    ui->textBrowser->append(QString("Time:")+QTime::currentTime().toString());

    for(int i=0;i<inhafStr.length();i++)
    {
        if(hafArray[k].data!='\0')
        {
            outToBrowser+=hafArray[k].data;
            k=0;
            if(inhafCount==1)  continue;
        }
        if(inhafStr[i]=='0')
        {
            k=hafArray[k].left;
        }
        else k=hafArray[k].right;
    }
    if(k) outToBrowser+=hafArray[k].data;
    ui->textBrowser->append(outToBrowser);
   hisWin->displayMsg(outToBrowser);
   hisWin->displayMsgHaf(inhafStr);
}

void PrivateScreen::Socket_sendMsg()
{
    ;
}

void PrivateScreen::sendMsg(QString str)
{
    tcpsocket->write(str.toStdString().data(),str.length());
}

void PrivateScreen::sendHaf()
{
    hafArray = new TreeN[map->count()*2-1];
    QQueue<Node*> qq;
    qq.push_back(parent);
    qint32 i = 0;
    qDebug()<<"run";
    while(!qq.isEmpty())
    {
        qq.head()->loc = i;
        //hafArray[i] = new TreeN;
        i++;
        if(qq.head()->left)  {
            qq.push_back(qq.head()->left);
            qq.push_back(qq.head()->right);
        }
        qq.pop_front();
    }
    qq.clear();
    qq.push_back(parent);

    while(!qq.isEmpty())
    {
        if(qq.head()->left)  {
            qq.push_back(qq.head()->left);
            qq.push_back(qq.head()->right);
            hafArray[qq.head()->loc].left = qq.head()->left->loc;
            hafArray[qq.head()->loc].right = qq.head()->right->loc;
        }
        else {
            hafArray[qq.head()->loc].left = -1;
            hafArray[qq.head()->loc].right = -1;
            hafArray[qq.head()->loc].data = qq.head()->value;
        }
        qq.pop_front();
    }
    for(i=0;i<map->count()*2-1;i++)
        qDebug()<<i<<'\t'<<hafArray[i].data<<'\t'<<hafArray[i].left<<'\t'<<hafArray[i].right;
    QByteArray block;
    QDataStream  out(&block,QIODevice::ReadWrite);
    out.setVersion(QDataStream::Qt_5_2);
    //out<<(qint32)0;
    qint32 outCount = map->count()*2-1;
    out<<outCount;
    for(int i=0;i<outCount;i++)
        out<<hafArray[i].data<<hafArray[i].left<<hafArray[i].right;
    /**********************/
    qint32 length = hafStr.length();
    qint32 frontCount = 31-length%31;
    qDebug()<<QString("s ")<<hafStr;
    for(int i=0;i<frontCount;i++)   hafStr.push_front('0');
    qint32 codeCount = (length+frontCount)/31;
    out<<length;
    out<<codeCount;
    QString t = "";
    int outInt32;
    bool flag;
    for(int i=0;i<codeCount;i++)
    {
        t.clear();t = "";
        for(int j=0;j<31;j++)  t+=hafStr[i*31+j];
        outInt32 = t.toInt(&flag,2);
        out<<outInt32;
        qDebug()<<QString("Send int ")<<outInt32<<QString("s ")<<t;
    }

    //qDebug()<<hafStr;
    //qDebug()<<t.toLong(&flag,2);

    /*for(int i=0;i<codeCount;i++)
    {

    }*/

     /************************/
    /*out.device()->seek(0);
    out<<(qint32)(block.size()-sizeof(qint32));*/
    tcpsocket->write(block);
}

/*void PrivateScreen::sendHafCode()
{
    qint32 length = hafStr.length();
    qint32 frontCount = length%32;
    for(int i=0;i<frontCount;i++)   hafStr.push_front('0');
    qDebug()<<hafStr;
}*/

