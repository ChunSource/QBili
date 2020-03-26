#include "song.h"
#include <QDebug>

Song::Song(QObject *parent) : QThread(parent)
{
    
}

Song::~Song()
{
    play->stop();
    play->deleteLater();
    manager->deleteLater();
    managerTwo->deleteLater();
    
}

void Song::switchSong(bool ok)
{
    isRun = ok;
    if(ok == false)
    {
        play->stop();
    }
}

void Song::newDanmu(QString name, QString message)
{
    if(!isRun)
    {
        return;
    }
    int pos = message.indexOf("点歌");
    if(pos == -1)
    {
        return;
    }
    else
    {
        QString sname = message.mid(pos+3);
        emit(name+"点歌:"+sname);
        addSong(name,sname);
    }
}

void Song::addSong(QString name, QString song)
{
    songName.push_back(song);
    userName.push_back(name);
    getID(song);
}

void Song::getID(QString name)
{
    qDebug()<<"request roomHost ";
    
    QNetworkRequest request; 
    QString url="http://music.eleuu.com/search?keywords="+name;
    request.setUrl(QUrl(url));
    manager->get(request);
}

void Song::backID(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();
    
    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
        qDebug() << "reason=" << reason.toString();
    
    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    }
    else {
        // 获取返回内容
        QByteArray response = reply->readAll();
        QJsonDocument documen = QJsonDocument::fromJson(response);
        QJsonObject object = documen.object();
        
        QJsonArray ary = object["result"].toObject()["songs"].toArray();
        for(int i=0;i<ary.size();i++)
        {
            //if(ary[i].toObject()["name"].toString())
             for(int j=0;j<songName.size();j++)
             {
                 if(songName.at(j) == ary[i].toObject()["name"].toString())
                 {
                     int id  = ary[i].toObject()["id"].toInt();
                     qDebug()<<"song id"<<id;
                     QNetworkRequest request; 
                     QString url="http://music.eleuu.com/song/url?id="+QString::number(id);
                     request.setUrl(QUrl(url));
                     managerTwo->get(request);
                     return;
                 }
             }
        }
        
    }
}

void Song::backSong(QNetworkReply *reply)
{
    QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    if(statusCode.isValid())
        qDebug() << "status code=" << statusCode.toInt();
    
    QVariant reason = reply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString();
    if(reason.isValid())
        qDebug() << "reason=" << reason.toString();
    
    QNetworkReply::NetworkError err = reply->error();
    if(err != QNetworkReply::NoError) {
        qDebug() << "Failed: " << reply->errorString();
    }
    else {
        // 获取返回内容
        QByteArray response = reply->readAll();
        qDebug()<<"response"<<response;
        QJsonDocument documen = QJsonDocument::fromJson(response);
        QJsonObject object = documen.object();
        
        QString url  = object["data"].toArray()[0].toObject()["url"].toString();
        qDebug()<<"url"<<url;
        urlist.push_back(url);
        if(play->state() == QMediaPlayer::State::StoppedState)
        {
            play->setMedia(QUrl(urlist.first()));
            urlist.pop_front();
            play->play();
        }
        qDebug()<<"没问题";
    }
}

void Song::turnSong(QMediaPlayer::State state)
{
    if(state ==QMediaPlayer::State::StoppedState)
    {
        if(!urlist.empty())
        {
            play->setMedia(QUrl(urlist.first()));
            urlist.pop_front();
            play->play();
        }
    }
}

void Song::run()
{
    manager = new QNetworkAccessManager();
    connect(manager,&QNetworkAccessManager::finished,this,&Song::backID,Qt::DirectConnection);
    managerTwo = new QNetworkAccessManager();
    connect(managerTwo,&QNetworkAccessManager::finished,this,&Song::backSong,Qt::DirectConnection);
    play = new QMediaPlayer();
    connect(play,&QMediaPlayer::stateChanged,this,&Song::turnSong,Qt::DirectConnection);
    
    this->exec();
}
