#ifndef DANMU_H
#define DANMU_H

#include <QPoint>
#include <QMouseEvent>
#include <QObject>
#include <QQmlEngine>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QQmlEngine>
#include <QQmlComponent>
#include <QQuickWindow>

namespace Ui {
class Danmu;
}

class Danmu : public QObject
{
    Q_OBJECT
    
public:
    explicit Danmu(QObject *parent = nullptr);
    ~Danmu();
public slots:
    void addBinray(const QByteArray buffer);
    void addMessage(QString color,QString title,QString level,QString userLevelColor,QString ulevel,QString danmu,QString username,bool showTitle);
    void show();
    void hide();
private:
    const QString mu="<p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-family:'SimSun'; font-size:14pt; color:#COLOR;\">DANMU</span></p>";
    QString html = "";
    QQmlEngine* eng = nullptr;
    QQmlComponent* compent = nullptr;
    QObject* root = nullptr;
private slots:
    QString dec2Hex(int);
};

#endif // DANMU_H
