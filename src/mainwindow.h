#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "api_bili.h"
#include "websocketapi_bili.h"
#include "soundthread.h"

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
private slots:
    void stopWebsocket();
    void button_connectLiveRoom();
    void button_dissconnectLiveRoom();
    void TextEdit_newChat(QString name,QString mess);
};
#endif // MAINWINDOW_H
