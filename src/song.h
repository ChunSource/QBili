#ifndef SONG_H
#define SONG_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QMediaPlayer>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>

class Song : public QThread
{
    Q_OBJECT
public:
    explicit Song(QObject *parent = nullptr);
    ~Song();
public slots:
    void switchSong(bool ok);
    void newDanmu(QString name,QString message);
    void addSong(QString name,QString song);
    void getID(QString name);
    void backID(QNetworkReply *reply);
    void backSong(QNetworkReply *reply);
    void turnSong(QMediaPlayer::State state);
signals:
    void success(QString log);
private:
    void run();
    bool isRun = false;
    QNetworkAccessManager *manager;
    QNetworkAccessManager *managerTwo;
    QMediaPlayer *play;
    QStringList urlist;
    
    QStringList songName;
    QStringList userName;
};

#endif // SONG_H
