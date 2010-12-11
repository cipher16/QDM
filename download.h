#ifndef DOWNLOAD_H
#define DOWNLOAD_H
#include <QObject>
#include <QTreeWidgetItem>
#include <QUrl>
#include <QFile>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

/*Direct download & parent of all others download systems*/
class Download : public QObject,QTreeWidgetItem
{
    Q_OBJECT
    QUrl url;   //url to download
    QString filename;
    QString filepath;
    int size;   //size of the file
    int range;  //range downloaded
    QNetworkAccessManager *nam;
    QNetworkReply *reply;

private slots:
    void write();
    void save();
    void updateProgress(qint64 received, qint64 total);
public:
    static QList<Download*> downloadList;

    Download(QTreeWidget *parent=0);
    void startDownload();
    void stopDownload();
    QNetworkReply * getReply();

    int getSize() const;
    int getRange() const;
    int getProgress();
    void setFile(const QString &file);
    void setPath(const QString &path);
    void setUrl(const QString &url);
    bool getFileMeta();//to get filename, size, ...
};

#endif // DOWNLOAD_H
