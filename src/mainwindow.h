#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "api_bili.h"
#include "websocketapi_bili.h"
#include "soundthread.h"
#include "danmu.h"
#include "song.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    API_Bili *api = nullptr;
    SoundThread* thread = nullptr;
    Danmu* danmu = nullptr;
    Song* song = nullptr;
    
private slots:
    void stopWebsocket();
    void button_connectLiveRoom();
    void button_dissconnectLiveRoom();
    void button_showMessagBox(bool check);
    void TextEdit_newChat(QString name,QString mess);
    void switchSpeech(int state);
    void switchSong(int state);
    void comboBox_changeLanguage(int language);
    void MessageBox_Binray(const QByteArray buff);
    void speechSupportLanguage(QVector<QLocale> list);  //获取语音播报支持的语言
    void logSong(QString song);
signals:
    void setSpeech(bool ok);
    void setSong(bool ok);
    void setLanguage(QLocale locale);
};
#endif // MAINWINDOW_H
