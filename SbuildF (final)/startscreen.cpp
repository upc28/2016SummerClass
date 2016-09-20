#include "startscreen.h"
#include "ui_startscreen.h"
#include <QNetworkInterface>


StartScreen::StartScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::StartScreen)
{
    ui->setupUi(this);

    /*QList<QHostAddress> list=QNetworkInterface::allAddresses();
    foreach(QHostAddress address,list)
    {
        if(address.protocol()==QAbstractSocket::IPv4Protocol&&address.toString()!=QString("127.0.0.1"))
        {
           ui->lab_IP_W->setText(address.toString());
           break;
        }
    }*/
}

StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::on_Button_exit_clicked()
{
    this->close();
}

void StartScreen::on_Button_connect_clicked()
{
    QString tarIp_text = ui->LineEdit_TargetIp->text();
    bool flag = true;
    for(int i=0;i<tarIp_text.length();i++)
    {
        if(tarIp_text[i]!='.'&&(tarIp_text[i]<'0'||tarIp_text[i]>'9'))
           { ui->textBro_msgout->append("target IP error!");
            flag = false;
            break;
        }
    }
    if(flag)
    {
        //qDebug()<<"run";
        priWin = new PrivateScreen(tarIp_text);
        qDebug()<<"runing";
        connect(tcpsocket1,SIGNAL(connected()),this,SLOT(slotShow()));
    }
}

void StartScreen::on_Button_listen_clicked()
{
    tcpserver = new QTcpServer;
    if(tcpserver->listen(QHostAddress::Any,28282))   ui->textBro_msgout->append(QString("Listening port 28282..."));
    else{
        ui->textBro_msgout->append(QString("Listen port 28282 fail."));
    }
    connect(tcpserver,SIGNAL(newConnection()),this,SLOT(receiveConnection));
}

void StartScreen::receiveConnection()
{
    tcpsocket = tcpserver->nextPendingConnection();
    ui->textBro_msgout->append(QString("receive connect from ")+tcpsocket->peerAddress().toString());
    connect(tcpsocket, SIGNAL(readyRead()), this, SLOT(readSocket()));
}

void StartScreen::slotShow()
{

    ui->textBro_msgout->append(QString("connect success"));
    priWin->move(this->x(),this->y()+this->frameSize().height());
    priWin->show();
    //char* data = "hello";
   // tcpsocket1->write(data);
}

void StartScreen::readSocket()
{
    qDebug()<<"run";
    QString str = tcpsocket->readAll();
    ui->textBro_msgout->append(str);
}
