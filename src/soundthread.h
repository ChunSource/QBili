#ifndef SOUNDTHREAD_H
#define SOUNDTHREAD_H

#include <QObject>
#include <QThread>
#include <QTextToSpeech>
#include <QStringList>
#include <QVector>
#include <QVoice>

class SoundThread : public QThread
{
    Q_OBJECT
public:
    explicit SoundThread(QObject *parent = nullptr);
    ~SoundThread();
    void run();
    
public slots:
    void makeSound(QString name,QString message);
    void setStatus(bool status);
    void setLanguage(QLocale locate);
private:
    QTextToSpeech* speech;
    void getSupportavailableEngines();
    void getSupportLanguage();
    bool isSpeech = false;
signals:
    void speechSupportLanguage(QVector<QLocale> list);
    void speechSupportEngines(QStringList list);
};

#endif // SOUNDTHREAD_H
