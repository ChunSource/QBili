#include "danmu.h"
#include <QDebug>
#include <QThread>
#if _MSC_VER >=1600    // MSVC2015>1899,对于MSVC2010以上版本都可以使用
#pragma execution_character_set("utf-8")
#endif


Danmu::Danmu(QObject *parent) :
    QObject(parent)
{
    eng = new QQmlEngine();
    compent = new QQmlComponent(eng,QUrl(QStringLiteral("qrc:/main.qml")));
    root = compent->create();
    
}

Danmu::~Danmu()
{
  //  root->deleteLater();
    compent->deleteLater();
    eng->deleteLater();
}

void Danmu::addBinray(const QByteArray buffer)
{
    QJsonDocument document = QJsonDocument::fromJson(buffer);
    QJsonObject object = document.object();
    if(object["cmd"].toString() == "DANMU_MSG") //弹幕
    {
        QJsonArray info = object["info"].toArray();
        QString name = info[2].toArray()[1].toString();  //获取弹幕发言人昵称
        QString danmu = info[1].toString();    //获取弹幕
        QString color="#5896de";
        QString title="没有狗牌";
        QString level="0";
        QString userLevelColor = "#5896de";
        QString ulevel = "0";
        bool showTitle = false;
        qDebug()<<"name"<<name;
        qDebug()<<"danmu"<<danmu;
        
        if(!info[3].toArray().empty())  //有狗牌
        {
            QJsonArray gp = info[3].toArray();
            color = "#"+dec2Hex(gp[4].toInt());
            title = gp[1].toString();
            level = QString::number(gp[0].toInt());
            showTitle = true;
        }
        
        if(!info[4].toArray().empty())  //有等级
        {
            QJsonArray dj = info[4].toArray();
            userLevelColor = "#"+dec2Hex(dj[2].toInt());
            ulevel = QString::number(dj[0].toInt());
        }
        
        addMessage(color,title,level,userLevelColor,ulevel,danmu,name,showTitle);
    }
    if(object["cmd"].toString() == "SEND_GIFT") //弹幕
    {
        QString name = object["data"].toObject()["uname"].toString();  //获取送礼人的昵称
        QString gift = object["data"].toObject()["giftName"].toString();  //获取礼物名字
        QString num = QString::number(object["data"].toObject()["num"].toInt());  //获取礼物数量
        
        QString lang = name+"送了"+num+"个"+gift;   //拼接语句
        
    }
    
    return;
}

void Danmu::addMessage(QString color, QString title, QString level, QString userLevelColor, QString ulevel, QString danmu, QString username,bool showTitle)
{
    QMetaObject::invokeMethod(root , "addMessage",
                              Q_ARG(QVariant, QVariant(color)),//color
                              Q_ARG(QVariant, QVariant(title)),//title
                              Q_ARG(QVariant, QVariant(level)),//level
                              Q_ARG(QVariant, QVariant(userLevelColor)),//userLevelColor
                              Q_ARG(QVariant, QVariant(ulevel)),//ulevel
                              Q_ARG(QVariant, QVariant(danmu)),//danmu
                              Q_ARG(QVariant, QVariant(username)),//username  
                              Q_ARG(QVariant, QVariant(showTitle)));//showTitle
}

void Danmu::show()
{
    QMetaObject::invokeMethod(root , "show");//username  
}

void Danmu::hide()
{
    QMetaObject::invokeMethod(root , "hide");//username  
}

QString Danmu::dec2Hex(int num)
{
    return QString("%1").arg(num, 4, 16, QLatin1Char('0')); 
}
