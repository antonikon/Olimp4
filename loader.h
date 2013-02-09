#pragma once

#include <QObject>
#include <QtCore>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QRegExp>
#include <QQueue>
#include <QFile>
#include <QUrl>
#include <QProcess>

#include "ui_mainwindow.h"
#include "cJSON.h"

class TLoader : public QObject
{
    Q_OBJECT
public:
    explicit TLoader(QObject *parent = 0, Ui::MainWindow *window = 0);
    void Load(const QString &url);
private:
    void Pars();
    void ParsItemList();
    void LoadVideo();
    QString getParams(const QString &params, const QVector<QString> &paramsList);
signals:
    
public slots:
    void LoadEnd(QNetworkReply *reply);    
private:
    QNetworkAccessManager *Loader;
    QByteArray Data;
    Ui::MainWindow *Window;
    QQueue <QString> ItemList;
    QQueue <QString> UrlList;
    int status=0;
    int files=0;
};
