#ifndef BILISETTING_H
#define BILISETTING_H

#include <QObject>
#include <QSettings>

class BiliSetting : public QObject
{
    Q_OBJECT
public:
    explicit BiliSetting(QObject *parent = nullptr);
    
    static void saveValue(QString key,QString text);
    static QString readValue(QString key);
    
};

#endif // BILISETTING_H
