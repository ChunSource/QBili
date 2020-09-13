#include "mainwindow.h"

#include "danmu.h"
#include <QApplication>
#include <QtGlobal>


#include <iostream>
#include <cstdlib>
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QMutex>
#include <QDateTime>
#define ZLIB_WINAPI
using namespace std;

#pragma comment(lib,"S:/Qt/c++/bili/zlib.lib")

QMutex mutex;//日志代码互斥锁
QString timePoint;
#pragma execution_character_set("utf-8")
void LogMsgOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
 {	 
//	 mutex.lock();
	 cout << msg.toStdString() << endl;
	 //Critical Resource of Code
     QByteArray localMsg = msg.toLocal8Bit();
	 QString log;
	 
     switch (type) {
     case QtDebugMsg:
         //fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		 log.append(QString("Debug  File:%1 %2  Line:%3  Content:%4").arg(context.file).arg(context.function).arg(context.line).arg(msg));
         break;
     case QtInfoMsg:
         //fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		 log.append(QString("Info: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
         break;
     case QtWarningMsg:
         //fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		 log.append(QString("Warning: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
         break;
     case QtCriticalMsg:
         //fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		 log.append(QString("Critical: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
         break;
     case QtFatalMsg:
         //fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
		 log.append(QString("Fatal: %1  %2  %3  %4").arg(localMsg.constData()).arg(context.file).arg(context.line).arg(context.function));
         abort();
     }
	 
	 QFile file;
	 QString path = QString("./log/log%1.lgt").arg(timePoint);
	 file.setFileName(path);
	 if (!file.open(QIODevice::ReadWrite | QIODevice::Append))
	 {
		 QString erinfo = file.errorString();
		 cout << erinfo.toStdString() << endl;
		 return;
	 }
	 QTextStream out(&file);
	 out << "\n\r" << log;
	 file.close();
 
//	 mutex.unlock();
 }
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    
#ifndef _DEBUG
//	timePoint = QDateTime::currentDateTime().toString("yyyyMMddHHmmss");
//	qInstallMessageHandler(LogMsgOutput);
#endif
    MainWindow w;
    w.show();
    return a.exec();
}
