#include "loader.h"

TLoader::TLoader(QObject *parent, Ui::MainWindow *window ) :
    QObject(parent),
    Loader(new QNetworkAccessManager(this)),
    Window(window)
{
    //Loader
    connect(Loader, SIGNAL(finished(QNetworkReply*)), this, SLOT(LoadEnd(QNetworkReply*)));
}

void TLoader::Load(const QString &url)
{
    QNetworkRequest req;
    req.setUrl(QUrl(url));
    req.setRawHeader("User-Agent", "Opera/9.80 (Windows NT 6.1; U; ru) Presto/2.8.131 Version/11.10");
    Loader->get(req);

}

void TLoader::LoadEnd(QNetworkReply *reply)
{
    Data = reply->readAll();
    if (status==0)
        Pars();
    else if (status==1)
        ParsItemList();
    else
        LoadVideo();

}

void TLoader::Pars()
{
    QRegExp reg("<li class=\"video-list-item yt-uix-scroller-scroll-unit.*href=\"(.*)\"");
    reg.setMinimal(true);
    qint32 i=reg.indexIn(Data);
    while (i!=-1)
    {
        QString url=reg.cap(1);
        url.insert(0,"http://www.youtube.com");
        ItemList.enqueue(url);
        i=reg.indexIn(Data,i+1);
    }
    status=1;
    files=ItemList.size();
    qDebug() << files;
    Load(ItemList.dequeue());
}

void TLoader::ParsItemList()
{
    QRegExp reg("playerConfig = (\\{.*\\});");
    reg.setMinimal(true);
    reg.indexIn(Data);
    QString data = reg.cap(1);
    cJSON* decodedJs = cJSON_Parse(data.toLocal8Bit().data());
    cJSON* argsJs = cJSON_GetObjectItem(decodedJs, "args");
    argsJs = cJSON_GetObjectItem(argsJs, "url_encoded_fmt_stream_map");
    data = cJSON_Print(argsJs);
    data = QUrl::fromPercentEncoding(data.toLocal8Bit());
    QVector <QString> dataList;
    bool in=true;
    int pos=0;
    for (int q=0;q<data.size();q++)
    {
        if (data[q]=='"') {
            in=!in;
        }
        else if (data[q]==',' & in==false) {
            dataList.push_back(data.mid(pos,q - pos));
            pos=q+1;
        }
    }
    int i=0;
    for (int q=0;q<dataList.size();q++)
    {
        QRegExp re("type=video/mp4;");
        re.setMinimal(true);
        i=re.indexIn(dataList[q]);
        if (i!=-1)
        {
            i=q;
            break;
        }
    }
    data=dataList[i];
    dataList=data.split("&").toVector();
    for (int q=0;q<dataList.size();q++)
        if (dataList[q].indexOf("url=")!=-1)
        {
            QStringList list=dataList[q].split("?");
            dataList.remove(q);
            dataList.push_back(list[0]);
            dataList.push_back(list[1]);
            break;
        }
    QString url;
    QRegExp re("url=");
    for (int i=0; i<dataList.size();i++)
        if (re.indexIn(dataList[i])!=-1)
        {
            url+=dataList[i]+"?";
            url.remove(0,4);
        }
    url+=getParams("key",dataList);
    url+=getParams("ip",dataList);
    url+=getParams("cp",dataList);
    url+=getParams("upn",dataList);
    url+=getParams("gcr",dataList);
    url+=getParams("sparams",dataList);
    url+=getParams("source",dataList);
    url+=getParams("ratebypass",dataList);
    url+=getParams("ipbits",dataList);
    url+=getParams("mt",dataList);
    url+=getParams("mv",dataList);
    url+=getParams("id",dataList);
    url+=getParams("ms",dataList);
    url+=getParams("expire",dataList);
    url+=getParams("newshard",dataList);
    url+=getParams("fexp",dataList);
    url+=getParams("sver",dataList);
    url+=getParams("itag",dataList);
    re.setPattern("sig");
    for (int i=0; i<dataList.size();i++)
        if (re.indexIn(dataList[i])!=-1)
        {
            QString sig=dataList[i];
            sig.remove(0,4);
            sig.insert(0,"signature=");
            url+=sig;
        }
    re.setPattern(" ");
    url.remove(re);
    qDebug() << "Load Item";
    UrlList.enqueue(url);
    if (ItemList.size()>0)
        Load(ItemList.dequeue());
    else
    {
        status=3;
        Load(UrlList.dequeue());
    }
}

void TLoader::LoadVideo()
{
    QFile file(QString("%1.mp4").arg(files));
    file.open(QFile::WriteOnly);
    file.write(Data);
    file.close();
    files--;
	qDebug() << Data.size();
	QProcess proc;
	QStringList params;
	params.append("-i");
	params.append(QString("%1.mp4").arg(files));
	params.append(QString("%1.mp3").arg(files));
	proc.setProcessChannelMode(QProcess::MergedChannels);
	proc.start("ffmpeg.exe",QIODevice::ReadWrite);

    Load(UrlList.dequeue());
}

QString TLoader::getParams(const QString &params,const QVector <QString> &paramsList)
{
    QRegExp re(params+"=");
    re.setMinimal(true);
    for (int i=0; i<paramsList.size();i++)
        if (re.indexIn(paramsList[i])==0)
            return paramsList[i]+"&";
    return " ";
}
