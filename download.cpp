#include "download.h"
#include <QDebug>

QList<Download*> Download::downloadList=QList<Download*>();

Download::Download(QTreeWidget *parent) : QTreeWidgetItem(parent)
{
    downloadList.append(this);
    filepath="/home/cipher16/";
    filename="";
    url=QUrl("");
}

void Download::startDownload(){
    qDebug() <<"Started Download";
    QNetworkRequest request(url); //On crée notre requête

    //if name is empty take the last component of the url
    if(filename.size()==0)
        filename=url.path().split("/").takeLast();
    //range download
    QFile file(filepath+filename);
    if(file.open(QIODevice::ReadOnly))
    {
        if(file.size()>0)
        {
            QString tmpContentRange("bytes=");
            tmpContentRange.append(QString::number(file.size())).append("-");
            request.setRawHeader("Range",tmpContentRange.toAscii());
        }
    }
    nam   = new QNetworkAccessManager();
    reply = nam->get(request);

    connect(reply, SIGNAL(finished()), this, SLOT(save()));
    connect(reply, SIGNAL(readyRead()),this, SLOT(write()));
    connect(reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(updateProgress(qint64, qint64)));

    setText(0,filename);
    setText(1,"Downloading");
}

void Download::stopDownload(){
    reply->abort();
}

void Download::save(){
    setText(1,"Finished");
}

void Download::write(){
    QNetworkReply *r = qobject_cast<QNetworkReply*>(QObject::sender());
    QFile file(filepath+filename);
    if(file.open(QIODevice::ReadWrite))
    {
        file.seek(file.size());
        file.write(r->read(r->bytesAvailable()));
        file.close();
    }
}

QNetworkReply * Download::getReply(){
    return reply;
}

void Download::setFile(const QString &file){
    this->filename=file;
}

void Download::setPath(const QString &path){
    this->filepath=path;
}

void Download::setUrl(const QString &url){
    this->url = QUrl(url);
}

bool Download::getFileMeta(){
    return true;
}

void Download::updateProgress(qint64 received, qint64 total){
    if(size!=0&&size==range)
        return; //finished
    this->size=total;
    this->range=received;
}

int Download::getSize() const{return size;}
int Download::getRange() const{return range;}
int Download::getProgress(){qDebug()<<"Appel a progress : "<<size<<" "<<range;return (range!=size ? ((range/size)*100) : 100);}
