#include "soundthread.h"

SoundThread::SoundThread(QObject *parent) : QThread(parent)
{
}

void SoundThread::run()
{
    speech = new QTextToSpeech();
    
    this->exec();
}

void SoundThread::makeSound(QString name,QString message)
{
    speech->say(message);
}
