#include "mainwindow.h"
#include <QApplication>
#include <QProcess>
#include <QtCore>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
	QProcess proc;
	proc.setProcessChannelMode(QProcess::MergedChannels);
	proc.start("ffmpeg.exe",QIODevice::ReadWrite);
	if(!proc.waitForStarted()) {
	   qDebug() << "ERROR!!!";
	}
	QByteArray data;
	while(proc.waitForReadyRead())
	   data.append(proc.readAll());
	qDebug() << data;
	qDebug() << "Done!";
    MainWindow w;
    w.show();
    
    return a.exec();
}
