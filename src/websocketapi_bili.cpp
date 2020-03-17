#include "websocketapi_bili.h"
#include <QDebug>
#include <QAbstractSocket>

WebSocketAPI_Bili::WebSocketAPI_Bili(int id,QObject *parent) : API_Bili(id,parent)
{
    setJsonData("roomid",id);
}

void WebSocketAPI_Bili::setHost(QString host)
{
    qDebug()<<"charhost "<<host;
    this->charhost = host;
}

void WebSocketAPI_Bili::setToken(QString token)
{
    qDebug()<<"chartoken "<<token;
    this->chartoken = token;
    setJsonData("key",token);
}

void WebSocketAPI_Bili::setPort(int port)
{
    qDebug()<<"charport "<<port;
    this->charport = QString::number(port);
}

void WebSocketAPI_Bili::run()
{
    getRoomHost(QString::number(roomId));
}

void WebSocketAPI_Bili::initWebsocket()
{
    qDebug()<<"run ";
    websocket = new QWebSocket();
    connect(websocket,&QWebSocket::binaryMessageReceived,this,&WebSocketAPI_Bili::processChatRsponse);
    
    
    connect(websocket,&QWebSocket::connected,this,[=](){
        //             //length             //head 16 // 0 1 2  //2 3 5 7 8
        char cdata[] = {0x00,0x00,0x00,0x00,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x07,0x00,0x00,0x00,0x01};
        QByteArray buffer(cdata,16);
        
        buffer = buffer+rootByte;
        
        int length = buffer.length();
        buffer[2] = (length / 256);
        buffer[3] = (length % 256);
        
        websocket->sendBinaryMessage(buffer+rootByte);
        
        //心跳
        timer = new QTimer();
        timer->start(20000);
        connect(timer,&QTimer::timeout,this,[this](){
            pingpong();
            timer->start(20000);
        });
    });
    
    
    connect(websocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),
            [=](QAbstractSocket::SocketError error)
    {
        qDebug()<<error;
    });
    connect(websocket,&QWebSocket::sslErrors,[=](const QList<QSslError> &errors)
    {
        qDebug()<<errors;
    });
    QSslConfiguration config;
    //   config.setPeerVerifyMode(QSslSocket::VerifyNone);  //仅加密，不对证书感兴趣
    config.setProtocol(QSsl::TlsV1_2);  //得看对方用什么协议
    
    websocket->setSslConfiguration(config);
    //  QString url = "wss://"+charhost+"/sub";
    QString url = "wss://broadcastlv.chat.bilibili.com:2245/sub";
    websocket->open(QUrl(url));
}

void WebSocketAPI_Bili::stop()
{
    emit newChat("提示","已断开弹幕服务器");
    timer->stop();
    timer->deleteLater();
    websocket->close();
    websocket->deleteLater();
}

void WebSocketAPI_Bili::getRoomHost(QString strID)
{
    qDebug()<<"request roomHost ";
    manager = new QNetworkAccessManager(this);
    connect(manager,&QNetworkAccessManager::finished,this,&WebSocketAPI_Bili::processHostResponse);
    
    QNetworkRequest request; 
    QString url="https://api.live.bilibili.com/room/v1/Danmu/getConf?room_id="+strID+"&platform=pc&player=web";
    request.setUrl(QUrl(url));
    manager->get(request);
    
}

void WebSocketAPI_Bili::setJsonData(QString key, QString value)
{
    QJsonDocument document = QJsonDocument::fromJson(rootByte);
    QJsonObject obj = document.object();
    obj.insert(key,value);
    rootByte = QJsonDocument(obj).toJson();
    rootByte = rootByte.replace(" ","").replace("\n","");
}

void WebSocketAPI_Bili::setJsonData(QString key, int value)
{
    QJsonDocument document = QJsonDocument::fromJson(rootByte);
    QJsonObject obj = document.object();
    obj.insert(key,value); 
    rootByte = QJsonDocument(obj).toJson();
    rootByte = rootByte.replace(" ","").replace("\n","");
}

void WebSocketAPI_Bili::pingpong()
{
    qDebug()<<"pingpong";
    char cdata[] = {0x00,0x00,0x00,0x10,0x00,0x10,0x00,0x01,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x01};
    QByteArray buf(cdata,16);
    websocket->sendBinaryMessage(buf);
}

void WebSocketAPI_Bili::analysisMessage(const QByteArray &json)
{
    qDebug()<<"处理完的数据包:"<<json;
    QJsonDocument document = QJsonDocument::fromJson(json);
    QJsonObject object = document.object();
    //qDebug()<<"\njson "<<object["info"].toString();
    if(object["cmd"].toString() == "DANMU_MSG") //弹幕
    {
        QJsonArray info = object["info"].toArray();
        QString name = info[2].toArray()[1].toString();  //获取弹幕发言人昵称
        QString danmu = info[1].toString();    //获取弹幕
        qDebug()<<"array2"<<info[2].toArray();
        qDebug()<<"name"<<name;
        qDebug()<<"danmu"<<danmu;
        emit newChat(name,danmu);
    }
    if(object["cmd"].toString() == "SEND_GIFT") //弹幕
    {
        QString name = object["data"].toObject()["uname"].toString();  //获取送礼人的昵称
        QString gift = object["data"].toObject()["giftName"].toString();  //获取礼物名字
        QString num = QString::number(object["data"].toObject()["num"].toInt());  //获取礼物数量
        
        QString lang = name+"送了"+num+"个"+gift;   //拼接语句
        emit newChat("礼物:",lang);
        
    }
    return;
}

void WebSocketAPI_Bili::processHostResponse(QNetworkReply *reply)
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
        
        setHost(object["data"].toObject()["host_server_list"].toArray()[0].toObject()["host"].toString());
        //  setToken(object["data"].toObject()["token"].toString());
        setPort(object["data"].toObject()["port"].toInt());
        this->initWebsocket();
    }
    manager->deleteLater();
}

void WebSocketAPI_Bili::processChatRsponse(const QByteArray& buff)
{
    
    
    QByteArray data;
    data.clear();
    data= buff; //获取数据类型
    
    if(data[11] == 0x03)  //心跳
    {
        qDebug()<<"心跳:"<<data;
        return;
    }
    else if(data[11] == 0x05) //礼物、弹幕、公告
    {
        
        int len = data.length()-16;
        QByteArray json = data.mid(16,len);  //截取json 
        qDebug()<<"\njson "<<json;
        
        if(data[7] ==0x02)   //判断有没有压缩
        { 
        }
        
        char chead[] ={0x00,0x10,0x00,0x00};
        QByteArray head(chead,4);
        
        
        while(1)
        {
            int index = json.indexOf(head)-4;//粘包二进制协议头位置
            if(index < 0) //<0 没有粘包
            {
                analysisMessage(json);
                break;
            }
            
            QByteArray tmp = json.mid(0,index); 
            analysisMessage(tmp);
            json = json.mid(index+16);
        }
        
    }
    else if(data[11] == 0x08)  //认证成功
    {
        bool ok;
        int len = data.mid(0,4).toHex().toInt(&ok,16);
        qDebug()<<"订阅成功"<<data.mid(16,len-16);
        emit newChat("提示","已连接弹幕服务器");
    }
    
}
