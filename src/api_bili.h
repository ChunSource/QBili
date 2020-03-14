#ifndef API_BILI_H
#define API_BILI_H
#include <QObject>

class API_Bili : public QObject
{
    Q_OBJECT
public:
    explicit API_Bili(int id,QObject *parent = nullptr);
    void start();
    virtual void stop() = 0;
    
signals:
    void newChat(QString name,QString message);
    
protected:
    int     roomId = 0;
    virtual void run() = 0;
};

#endif // API_BILI_H
