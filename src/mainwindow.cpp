#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->ButtonConnect,&QPushButton::clicked,this,&MainWindow::button_connectLiveRoom);
    connect(ui->ButtonDisconnect,&QPushButton::clicked,this,&MainWindow::button_dissconnectLiveRoom);
    thread = new SoundThread();
}

MainWindow::~MainWindow()
{
    stopWebsocket();
    delete ui;
}

void MainWindow::stopWebsocket()
{
    if(api != nullptr)
    {
        disconnect(api,0,0,0);
        api->stop();
        delete api;
        api = nullptr;
    }
}

void MainWindow::button_dissconnectLiveRoom()
{
    stopWebsocket();
    ui->ButtonDisconnect->setDisabled(true);
    ui->ButtonConnect->setEnabled(true);
}

void MainWindow::TextEdit_newChat(QString name, QString mess)
{
    ui->textEdit->append(name+":"+mess+"\n");
}

void MainWindow::button_connectLiveRoom()
{
    //获取房间号
    int number = ui->LineEditLiveNumber->text().toInt();
    
    api = new WebSocketAPI_Bili(number);
    
    connect(api,&API_Bili::newChat,this,&MainWindow::TextEdit_newChat);
    connect(api,&API_Bili::newChat,thread,&SoundThread::makeSound,Qt::UniqueConnection);
    
    thread->start();
    api->start();
    
    ui->ButtonConnect->setDisabled(true);
    ui->ButtonDisconnect->setEnabled(true);
}

