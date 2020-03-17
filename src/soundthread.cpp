#include "soundthread.h"
#include <QDebug>

SoundThread::SoundThread(QObject *parent) : QThread(parent)
{
}

SoundThread::~SoundThread()
{
    speech->deleteLater();
}

void SoundThread::run()
{
    speech = new QTextToSpeech();
    getSupportLanguage();
    getSupportavailableEngines();
    this->exec();
}

void SoundThread::setStatus(bool status)
{
    isSpeech = status;
}

void SoundThread::setLanguage(QLocale locate)
{
    speech->setLocale(locate);
}

void SoundThread::makeSound(QString name,QString message)
{
    if(!isSpeech)
    {
        return;
    }
    speech->say(message);
}

void SoundThread::getSupportavailableEngines()
{
    QStringList languageList = speech->availableEngines();
    qDebug()<<languageList;
    emit speechSupportEngines(languageList);
}

void SoundThread::getSupportLanguage()
{
    QVector<QLocale> vector = speech->availableLocales();
    emit speechSupportLanguage(vector);
}
