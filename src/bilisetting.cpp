#include "bilisetting.h"

BiliSetting::BiliSetting(QObject *parent) : QObject(parent)
{
    
}

void BiliSetting::saveValue(QString key, QString text)
{
    QSettings setting("bili",QSettings::IniFormat);
    setting.setValue(key,text);
}

QString BiliSetting::readValue(QString key)
{
    QSettings setting("bili",QSettings::IniFormat);
    return setting.value(key).toString();
}
