#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qRegisterMetaType<QVector<QLocale>>("QVector<QLocale>");  //注册，否则信号槽不能传递自定义类型的容器
    ui->setupUi(this);
    
    ui->statusbar->showMessage("当前未订阅直播间");
    connect(ui->ButtonConnect,&QPushButton::clicked,this,&MainWindow::button_connectLiveRoom);
    connect(ui->ButtonDisconnect,&QPushButton::clicked,this,&MainWindow::button_dissconnectLiveRoom);
    connect(ui->checkBoxSpeech,&QCheckBox::stateChanged,this,&MainWindow::switchSpeech);  //语音播报
    connect(ui->checkBoxSongUse,&QCheckBox::stateChanged,this,&MainWindow::switchSong);
    connect(ui->comboBoxLanguage,QOverload<int>::of(&QComboBox::currentIndexChanged),this,&MainWindow::comboBox_changeLanguage);  //语言选择
    connect(ui->checkBoxMessageShow,&QCheckBox::clicked,this,&MainWindow::button_showMessagBox);  //弹幕窗口
    danmu = new Danmu();
    thread = new SoundThread();
    connect(thread,&SoundThread::speechSupportLanguage,this,&MainWindow::speechSupportLanguage); //获取支持的语言列表
    connect(this,&MainWindow::setSpeech,thread,&SoundThread::setStatus,Qt::QueuedConnection);  //设置语音播报是否开启
    connect(this,&MainWindow::setLanguage,thread,&SoundThread::setLanguage,Qt::QueuedConnection);  //设置语音播报语言
    thread->start();
    
    song = new Song();
    connect(this,&MainWindow::setSong,song,&Song::switchSong,Qt::QueuedConnection);
    connect(song,&Song::success,this,&MainWindow::logSong);
    song->start();
}

MainWindow::~MainWindow()
{
    
    stopWebsocket();
    thread->exit(0);
    thread->deleteLater();
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
    ui->statusbar->showMessage("当前未订阅直播间");
    stopWebsocket();
    ui->ButtonDisconnect->setDisabled(true);
    ui->ButtonConnect->setEnabled(true);
}

void MainWindow::button_showMessagBox(bool check)
{
    if(check)
    {
        danmu->show();
    }
    else
    {
        danmu->hide();
    }
}

void MainWindow::comboBox_changeLanguage(int language)
{
    QLocale locale = ui->comboBoxLanguage->itemData(language).toLocale();
    qDebug()<<locale;
    emit setLanguage(locale);
}

void MainWindow::MessageBox_Binray(const QByteArray buff)
{
    danmu->addBinray(buff);
}

void MainWindow::TextEdit_newChat(QString name, QString mess)
{
    ui->textEdit->append(name+":"+mess+"\n");
}

void MainWindow::switchSpeech(int state)
{
    bool status = ui->checkBoxSpeech->isChecked();
    emit setSpeech(status);
}

void MainWindow::switchSong(int state)
{
    bool status = ui->checkBoxSongUse->isChecked();
    emit setSong(status);
}

void MainWindow::speechSupportLanguage(QVector<QLocale> list)
{
    qDebug()<<list;
    for(auto iter=list.begin();iter!=list.end();iter++)
    {
        QLocale locale = *iter;
        QString name(QString("%1 (%2)")
                     .arg(QLocale::languageToString(locale.language()))
                     .arg(QLocale::countryToString(locale.country())));
        QVariant var(locale);
        ui->comboBoxLanguage->addItem(name,var);
        
    }
}

void MainWindow::logSong(QString song)
{
    ui->textEditSong->append(song);
}

void MainWindow::button_connectLiveRoom()
{
    //获取房间号
    int number = ui->LineEditLiveNumber->text().toInt();
    ui->statusbar->showMessage("当前订阅房间号"+QString::number(number));
    api = new WebSocketAPI_Bili(number);
    
    connect(api,&API_Bili::newChat,this,&MainWindow::TextEdit_newChat);  //解析好的弹幕数据
    connect(api,&API_Bili::newBinray,this,&MainWindow::MessageBox_Binray);  //原始数据
    connect(api,&API_Bili::newChat,thread,&SoundThread::makeSound,Qt::UniqueConnection); //声音播报
    connect(api,&API_Bili::newChat,song,&Song::newDanmu,Qt::QueuedConnection);  //点歌
    api->start();
    
    ui->ButtonConnect->setDisabled(true);
    ui->ButtonDisconnect->setEnabled(true);
}

