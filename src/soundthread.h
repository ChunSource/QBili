#ifndef SOUNDTHREAD_H
#define SOUNDTHREAD_H

#include <QObject>
#include <QThread>
#include <QTextToSpeech>

class SoundThread : public QThread
{
    Q_OBJECT
public:
    explicit SoundThread(QObject *parent = nullptr);
    
    void run();
    
public slots:
    void makeSound(QString name,QString message);
private:
    QTextToSpeech* speech;

};

#endif // SOUNDTHREAD_H
