#ifndef WEBSOCKETAPI_BILI_H
#define WEBSOCKETAPI_BILI_H

#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSslConfiguration>
#include <QWebSocket>
#include <QTimer>
#include "api_bili.h"
#include "zlib.h"



class WebSocketAPI_Bili : public API_Bili
{
    Q_OBJECT
public:
    explicit WebSocketAPI_Bili(int id,QObject *parent = nullptr);
    
    void setHost(QString host);
    void setToken(QString token);
    void setPort(int port);
    
    void initWebsocket();
    void stop();
signals:
  //  void newChat(QString name,QString mess);
private:
    QNetworkAccessManager* manager;
    QWebSocket* websocket;
    QTimer* timer = nullptr;
    QString charport = 0;
    QString charhost = "";  //房间域名
    QString chartoken = ""; //websocket Token
 //   QByteArray rootByte = "{\"uid\":19176530,\"roomid\":109216,\"protover\":2,\"platform\":\"web\",\"clientver\":\"1.10.3\",\"type\":2}";
    QByteArray rootByte = "{\"uid\":0,\"roomid\":109216,\"protover\":2}";   //只保留这两个，不然会zlib压缩
    QByteArray rootData;
    
    void run();
    void getRoomHost(QString strID);
    void setJsonData(QString key,QString value);
    void setJsonData(QString key,int value);
    void pingpong();
    void analysisMessage(const QByteArray& json);
    
private slots:
    
    void processHostResponse(QNetworkReply* reply);
    void processChatRsponse(const QByteArray& buff);
};

#endif // WEBSOCKETAPI_BILI_H
